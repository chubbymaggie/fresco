/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Graydon Hoare <graydon@pobox.com> 
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
#ifndef _DrawDecorator_hh
#define _DrawDecorator_hh

#include <Warsaw/config.hh>
#include <Warsaw/DrawTraversal.hh>
#include <Warsaw/DrawingKit.hh>
#include <Berlin/MonoGraphic.hh>

class DrawingKit;

class DrawStateContainer : public MonoGraphic
{
public:
  DrawStateContainer(Graphic_ptr b) {body(b);}
  virtual void draw(DrawTraversal_ptr traversal) 
  {
    traversal->kit()->saveState();
    MonoGraphic::traverse(traversal);
    traversal->kit()->restoreState();    
  }  
};

// this just sets up a rudimentary decorator such that the value is set (in the
// traversing drawingkit) when it is entered. 

template <class T>
class DrawDecorator : public MonoGraphic
{
  typedef void (DrawingKit::*method)(T);
  method meth;
  T myVal;  
public:
  DrawDecorator(T val, method meth, Graphic_ptr b) : 
    myVal(val)
  {
    body(b);
  }
  virtual void draw(DrawTraversal_ptr traversal) 
  {
    (traversal->kit()->*meth)(myVal);
    MonoGraphic::traverse(traversal);
  }
};

#endif
