/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999 Stefan Seefeld <stefan@fresco.org> 
 * http://www.fresco.org
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
#ifndef _LogoDemo_hh
#define _LogoDemo_hh

#include <Fresco/config.hh>
#include <Fresco/Command.hh>
#include <Fresco/BoundedValue.hh>
#include <Berlin/TransformImpl.hh>
#include <Berlin/ObserverImpl.hh>
#include <Berlin/ImplVar.hh>
#include "Demo.hh"

class LogoDemo : public Demo
{
  class Rotator : public ObserverImpl
  {
  public:
    Rotator(Fresco::BoundedValue_ptr,
	    Fresco::Graphic_ptr, Fresco::Graphic_ptr, Fresco::Coord);
    void update(const CORBA::Any &);
  private:
    Fresco::BoundedValue_var value;
    Fresco::Graphic_var child;
    Fresco::Graphic_var parent;
    Fresco::Coord zdegree;
  };
public:
  LogoDemo(Application *);
  Fresco::Graphic_ptr make_controller(Fresco::BoundedValue_ptr, const Fresco::Color &);
private:
  Fresco::BoundedValue_var bv1;
  Fresco::BoundedValue_var bv2;
  Fresco::BoundedValue_var bv3;
  Impl_var<Rotator> rotator1;
  Impl_var<Rotator> rotator2;
  Impl_var<Rotator> rotator3;
};

#endif
