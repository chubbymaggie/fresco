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

#include <Desktop/DesktopKitImpl.hh>
#include <Desktop/WindowImpl.hh>
#include <Desktop/DesktopImpl.hh>
#include <Desktop/Titlebar.hh>
#include <Berlin/Logger.hh>

DesktopKitImpl::DesktopKitImpl(Screen_ptr s, LayoutKit_ptr l, WidgetKit_ptr w)
  : screen(Screen::_duplicate(s)), lk(LayoutKit::_duplicate(l)), wk(WidgetKit::_duplicate(w))
{
  desktop = new DesktopImpl;
  desktop->_obj_is_ready(CORBA::BOA::getBOA());
  screen->body(Desktop_var(desktop->_this()));
  stage = lk->createStage();
  desktop->init(stage);
  screen->appendController(Controller_var(desktop->_this()));
}

DesktopKitImpl::~DesktopKitImpl()
{
  desktop->_dispose();
}

Window_ptr DesktopKitImpl::shell(Graphic_ptr g)
{
  SectionLog section(Logger::traversal, "DesktopKitImpl::shell");
  WindowImpl *window = new WindowImpl(desktop);
  window->_obj_is_ready(_boa());
  Color gray = {0.5, 0.5, 0.5, 1.0};
  Titlebar *tb = new Titlebar(gray);
  tb->_obj_is_ready(_boa());
  Command_var move = window->reposition();
  Graphic_var frame = wk->outset(Graphic_var(tb->_this()), gray);
  Graphic_var dragger = wk->dragger(frame, move);
  Graphic_var box = lk->vbox();
  box->append(dragger);
  box->append(g);
  window->body(box);
  window->insert();
  windows.push_back(window);
  desktop->appendController(Controller_var(window->_this()));
  return window->_this();
}

Window_ptr DesktopKitImpl::transient(Graphic_ptr g)
{
  return Window::_nil();
}