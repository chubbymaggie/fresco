/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Stefan Seefeld <stefan@berlin-consortium.org> 
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

#include <Warsaw/Server.hh>
#include "Desktop/DesktopKitImpl.hh"
#include "Desktop/WindowImpl.hh"
#include "Berlin/Logger.hh"

DesktopKitImpl::DesktopKitImpl(KitFactory *f, const PropertySeq &p) : KitImpl(f, p) {}
DesktopKitImpl::~DesktopKitImpl()
{
  for (vector<WindowImpl *>::iterator i = windows.begin(); i != windows.end(); i++)
    (*i)->_dispose();
}

void DesktopKitImpl::bind(ServerContext_ptr sc)
{
  KitImpl::bind(sc);
  CORBA::Object_var object = sc->getSingleton(interface(Desktop));
  desktop = Desktop::_narrow(object);
  
  PropertySeq props;
  props.length(0);
  lk = obtain(sc, LayoutKit, props);
  wk = obtain(sc, WidgetKit, props);
}

Desktop_ptr DesktopKitImpl::desk()
{
  return Desktop::_duplicate(desktop);
}


Window_ptr DesktopKitImpl::shell(Controller_ptr g)
{
  SectionLog section("DesktopKitImpl::shell");
  WindowImpl *window = new WindowImpl;
  window->_obj_is_ready(_boa());
  Color gray = {0.5, 0.5, 0.5, 1.0};

  Graphic::Requisition req;
  req.x.defined = true;
  req.x.minimum = 0.;
  req.x.natural = 0.;
  req.x.maximum = lk->fil();
  req.x.align = 0.;
  req.y.defined = true;
  req.y.minimum = 200.;
  req.y.natural = 200.;
  req.y.maximum = 200.;
  req.y.align = 0;
  Command_var move = window->move();
  Graphic_var tbframe = wk->outset(Graphic_var(lk->glueRequisition(req)), gray, true);
  Graphic_var tbdragger = wk->dragger(tbframe, move);

  req.x.minimum = 200.;
  req.x.natural = 200.;
  req.x.maximum = 200.;
  req.y.minimum = 40.;
  req.y.natural = 40.;
  req.y.maximum = 40.;
  Command_var lresize = window->moveResize(1.0, 0.0, Window::left|Window::bottom);
  Graphic_var lframe = wk->outset(Graphic_var(lk->glueRequisition(req)), gray, true);
  Graphic_var ldragger = wk->dragger(lframe, lresize);

  req.x.minimum = 0.;
  req.x.natural = 0.;
  req.x.maximum = lk->fil();
  req.y.minimum = 40.;
  req.y.natural = 40.;
  req.y.maximum = 40.;
  Command_var bresize = window->moveResize(0.0, 0.0, Window::bottom);
  Graphic_var bframe = wk->outset(Graphic_var(lk->glueRequisition(req)), gray, true);
  Graphic_var bdragger = wk->dragger(bframe, bresize);

  req.x.minimum = 200.;
  req.x.natural = 200.;
  req.x.maximum = 200.;
  req.y.minimum = 40.;
  req.y.natural = 40.;
  req.y.maximum = 40.;
  Command_var rresize = window->moveResize(0.0, 0.0, Window::right|Window::bottom);
  Graphic_var rframe = wk->outset(Graphic_var(lk->glueRequisition(req)), gray, true);
  Graphic_var rdragger = wk->dragger(rframe, rresize);

  Graphic_var vbox = lk->vbox();
  Graphic_var hbox = lk->hbox();
  hbox->append(ldragger);
  hbox->append(bdragger);
  hbox->append(rdragger);
  vbox->append(tbdragger);
  vbox->append(g);
  vbox->append(hbox);
  window->body(vbox);
  /*
   * FIXME: we need to take care to include the window control elements 
   *        into the control graph too...
   *        - stefan
   */
  window->appendController(g);
  window->insert(desktop, true);
  windows.push_back(window);
  desktop->appendController(Controller_var(window->_this()));
  return window->_this();
}

Window_ptr DesktopKitImpl::transient(Controller_ptr g)
{
  SectionLog section("DesktopKitImpl::transient");
  WindowImpl *window = new WindowImpl;
  window->_obj_is_ready(_boa());
  Color gray = {0.5, 0.5, 0.5, 1.0};

  Graphic::Requisition req;
  req.x.defined = true;
  req.x.minimum = 0.;
  req.x.natural = 0.;
  req.x.maximum = lk->fil();
  req.x.align = 0.;
  req.y.defined = true;
  req.y.minimum = 200.;
  req.y.natural = 200.;
  req.y.maximum = 200.;
  req.y.align = 0;
  Command_var move = window->move();
  Graphic_var tbframe = wk->outset(Graphic_var(lk->glueRequisition(req)), gray, true);
  Graphic_var tbdragger = wk->dragger(tbframe, move);

  req.x.minimum = 200.;
  req.x.natural = 200.;
  req.x.maximum = 200.;
  req.y.minimum = 40.;
  req.y.natural = 40.;
  req.y.maximum = 40.;
  Command_var lresize = window->moveResize(1.0, 0.0, Window::left|Window::bottom);
  Graphic_var lframe = wk->outset(Graphic_var(lk->glueRequisition(req)), gray, true);
  Graphic_var ldragger = wk->dragger(lframe, lresize);

  req.x.minimum = 0.;
  req.x.natural = 0.;
  req.x.maximum = lk->fil();
  req.y.minimum = 40.;
  req.y.natural = 40.;
  req.y.maximum = 40.;
  Command_var bresize = window->moveResize(0.0, 0.0, Window::bottom);
  Graphic_var bframe = wk->outset(Graphic_var(lk->glueRequisition(req)), gray, true);
  Graphic_var bdragger = wk->dragger(bframe, bresize);

  req.x.minimum = 200.;
  req.x.natural = 200.;
  req.x.maximum = 200.;
  req.y.minimum = 40.;
  req.y.natural = 40.;
  req.y.maximum = 40.;
  Command_var rresize = window->moveResize(0.0, 0.0, Window::right|Window::bottom);
  Graphic_var rframe = wk->outset(Graphic_var(lk->glueRequisition(req)), gray, true);
  Graphic_var rdragger = wk->dragger(rframe, rresize);

  Graphic_var vbox = lk->vbox();
  Graphic_var hbox = lk->hbox();
  hbox->append(ldragger);
  hbox->append(bdragger);
  hbox->append(rdragger);
  vbox->append(tbdragger);
  vbox->append(g);
  vbox->append(hbox);
  window->body(vbox);
  /*
   * FIXME: we need to take care to include the window control elements 
   *        into the control graph too...
   *        - stefan
   */
  window->appendController(g);
  window->insert(desktop, false);
  windows.push_back(window);
  desktop->appendController(Controller_var(window->_this()));
  return window->_this();
}

extern "C" KitFactory *load()
{
  static string properties[] = {"implementation", "DesktopKitImpl"};
  return new KitFactoryImpl<DesktopKitImpl> (interface(DesktopKit), properties, 1);
}
