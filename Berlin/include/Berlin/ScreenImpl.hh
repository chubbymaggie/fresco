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
#ifndef _ScreenImpl_hh
#define _ScreenImpl_hh

#include <Warsaw/config.hh>
#include <Warsaw/Screen.hh>
#include <Berlin/MonoGraphic.hh>
#include <Berlin/ControllerImpl.hh>
#include <Berlin/ImplVar.hh>

class ScreenManager;
class EventManager;
class RegionImpl;
class GLDrawingKit;

class ScreenImpl : implements(Screen), public ControllerImpl
{
public:
  ScreenImpl(DrawingKit_ptr);
  virtual ~ScreenImpl();

  virtual void pick(PickTraversal_ptr);
  virtual void allocations(Allocation_ptr);

  virtual Coord width();
  virtual Coord height();
  virtual DrawingKit_ptr kit();
  virtual void damage(Region_ptr);

  virtual void insertController(Controller_ptr) {}
  virtual void replaceController(Controller_ptr) {}
  virtual void removeController() {}
  virtual void setControllerLinks(Controller_ptr, Controller_ptr, Controller_ptr) {}
  virtual CORBA::Boolean requestFocus(Controller_ptr, Input::Device);
  virtual CORBA::Boolean receiveFocus(Focus_ptr) { return true;}
  virtual void loseFocus(Input::Device) {}
  virtual CORBA::Boolean handlePositional(PickTraversal_ptr, const Input::Event &) { return false;}
  virtual CORBA::Boolean handleNonPositional(const Input::Event &) { return false;}

  ScreenManager *manager();
  Region_ptr getRegion();
protected:
  DrawingKit_var  drawing;
  ScreenManager *smanager;
  Impl_var<RegionImpl> region;
  EventManager  *emanager;
};

#endif /* _ScreenImpl_hh */
