/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Stefan Seefeld <seefelds@magellan.umontreal.ca> 
 * http://www.berlin-consortium.org
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
 * MA 02139, USA.
 */

#include "Berlin/ScreenManager.hh"
#include "Berlin/DrawTraversalImpl.hh"
#include "Berlin/ScreenImpl.hh"
#include "Berlin/RegionImpl.hh"
#include "Berlin/EventManager.hh"
#include "Drawing/openGL/GLDrawingKit.hh"
#include "Drawing/openGL/Pointer.hh"
#include "Berlin/Logger.hh"

static Mutex ggi_mutex;
static ggi_event event;

ScreenManager::ScreenManager(ScreenImpl *s, EventManager *em, GLDrawingKit *d)
  : screen(s), emanager(em), drawing(d), visual(drawing->getVisual())
{
  pointer = new Pointer(visual);
}

ScreenManager::~ScreenManager()
{
  delete pointer;
}

void ScreenManager::damage(Region_ptr r)
{
  SectionLog section(Logger::drawing, "ScreenManager::damage");
  MutexGuard guard(mutex);
  RegionImpl *region = new RegionImpl(r, Transform_var(Transform::_nil()));
  region->_obj_is_ready(CORBA::BOA::getBOA());
  damages.push_back(region);
  Logger::log(Logger::drawing) << "ScreenManager::damage region " << *region << endl;
  // this injects a damage notice into the event queue, waking up
  // the sleeping event thread.

  // sadly it appears that GGI's event queue is not threadsafe, so
  // we must use a poll loop with a timeout. this is not totally
  // awful, it just means we are zooming through a short loop once
  // every 20000 microseconds. it doesn't take too much CPU time on my
  // machine, and if it takes too much on yours, hey, go fix GGI. 

  //     ggi_mutex.lock();
  //     ggi_event *damageEvent = new ggi_event;
  //     gettimeofday(&(damageEvent->any.time), NULL);
  //     damageEvent->any.type = evCommand;
  //     ggiEventSend (visual, damageEvent);    
  //     ggi_mutex.unlock();
}

void ScreenManager::repair()
{
  SectionLog section(Logger::drawing, "ScreenManager::repair");
  mutex.lock();
  dlist_t tmp = damages;
  damages.erase(damages.begin(), damages.end());
  mutex.unlock();
  
  for (dlist_t::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      Logger::log(Logger::drawing) << "repairing region "
				   << '(' << (*i)->lower.x << ',' << (*i)->lower.y
				   << ',' << (*i)->upper.x << ',' << (*i)->upper.y << ')' << endl;
      bool ptr = pointer->intersects((*i)->lower.x, (*i)->upper.x, (*i)->lower.y, (*i)->upper.y);
      drawing->clear((*i)->lower.x, (*i)->lower.y, (*i)->upper.x, (*i)->upper.y);
      if (ptr) pointer->restore();
      DrawTraversalImpl *traversal = new DrawTraversalImpl(Graphic_var(screen->_this()),
  							   Region_var((*i)->_this()),
 							   Transform_var(Transform::_nil()),
 							   DrawingKit_var(drawing->_this()));
      traversal->_obj_is_ready(CORBA::BOA::getBOA());
      screen->traverse(Traversal_var(traversal->_this()));
      traversal->_dispose();
      drawing->sync();
      if (ptr)
	{
	  pointer->backup();
	  pointer->draw();
	}
      emanager->damage(Region_var((*i)->_this()));
      (*i)->_dispose();
    }
}

void ScreenManager::nextEvent()
{
  ggi_event_mask mask = ggi_event_mask (emCommand | emKeyboard | emPtrMove | emPtrButtonPress | emPtrButtonRelease);
  timeval t;

  t.tv_sec = 0;
  t.tv_usec = 20000; 
    
  if (ggiEventPoll(visual, mask, &t)) ggiEventRead(visual, &event, mask);
  else return;

  // we are being woken up by the damage subsystem
  if (event.any.origin == GII_EV_ORIGIN_SENDEVENT) return;

  // we can process this, it's a legitimate event.
  switch (event.any.type)
    {
    case evKeyPress:
    case evKeyRepeat:
      {      
	Event::Key key;
	key.theChar = event.key.sym;
	emanager->dispatch(key);
	break;
      }
    case evPtrAbsolute:
      {
	ptrPositionX = event.pmove.x;
	ptrPositionY = event.pmove.y;
	pointer->move(ptrPositionX, ptrPositionY);
	// absence of break statement here is intentional
      }
    case evPtrButtonPress:
    case evPtrButtonRelease:
      {
	Event::Pointer pointer;	  
	pointer.location.x = ptrPositionX;
	pointer.location.y = ptrPositionY;	  
	pointer.location.z = 0; // time being we're using non-3d mice.
	pointer.buttonNumber = event.pbutton.button;	  
	pointer.whatHappened = 
	  event.any.type == evPtrAbsolute ? Event::hold :
	  event.any.type == evPtrButtonPress ? Event::press :
	  event.any.type == evPtrButtonRelease ? Event::release : Event::hold;
	emanager->dispatch(pointer);
	break;
      }
    }
}

void ScreenManager::run()
{
  while (true)
    {
      mutex.lock();
      int amountOfDamage = damages.size();
      mutex.unlock();
      if (amountOfDamage > 0)
	{
	  repair();
	}
      nextEvent();
    }
}
