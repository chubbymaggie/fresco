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

#include "Widget/Frame.hh"
#include "Warsaw/Traversal.hh"
#include "Warsaw/Subject.hh"
#include "Berlin/Color.hh"


Frame::Frame(Coord t, const Color &c, type ty)
  : Bevel(t, 0.5, 0.5, true, true), color(c), mode(ty)
{
}

Frame::~Frame() {}

void Frame::draw(DrawTraversal_ptr dt)
{
  Region_var allocation = dt->allocation();
  Vertex u, l;
  allocation->bounds(l, u);
  Color light, dark;
  switch (mode)
    {
    case convex:
      light = brightness(color, 0.5);
      dark = brightness(color, -0.5);
      break;
    case concav:
      light = brightness(color,-0.5);
      dark = brightness(color,  0.5);
      break;
    case flat:
      light = brightness(color,-1.0);
      dark = brightness(color,-1.0);
      break;
    }
  Bevel::rect(dt, thickness, color, light, dark, l.x, u.x, l.y, u.y);
}

DynamicFrame::DynamicFrame(Coord t, const Color &c, type t1, type t2, Telltale::Flag m)
  : Bevel(t, 0.5, 0.5, true, true), Frame(t, c, t1), type1(t1), type2(t2), mask(m)
{
}

DynamicFrame::~DynamicFrame()
{
  if (!CORBA::is_nil(telltale)) telltale->detach(skeletonize(Observer)::_this());
}

void DynamicFrame::attach(Telltale_ptr subject)
{
  if (!CORBA::is_nil(telltale)) telltale->detach(skeletonize(Observer)::_this());
  telltale = subject;
  telltale->attach(skeletonize(Observer)::_this());
}

void DynamicFrame::update(Subject_ptr subject)
{
  bool flag = telltale->test(mask);
  if (flag == on) return;
  on = flag;
  mode = on ? type1 : type2;
  needRedraw();
}
