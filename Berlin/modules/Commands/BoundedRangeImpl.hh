/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999, 2000 Stefan Seefeld <stefan@berlin-consortium.org> 
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
#ifndef _BoundedRangeImpl_hh
#define _BoundedRangeImpl_hh

#include <Warsaw/config.hh>
#include <Warsaw/BoundedRange.hh>
#include <Berlin/SubjectImpl.hh>
#include <Prague/Sys/Thread.hh>
#include <vector>

class BoundedRangeImpl : public virtual POA_Warsaw::BoundedRange,
			 public SubjectImpl
{
 public:
  BoundedRangeImpl(Warsaw::Coord, Warsaw::Coord, Warsaw::Coord, Warsaw::Coord, Warsaw::Coord, Warsaw::Coord);
  virtual ~BoundedRangeImpl();
  virtual Warsaw::BoundedRange::Settings state();
  virtual void state(const Warsaw::BoundedRange::Settings &);
  virtual Warsaw::Coord lower();
  virtual void lower(Warsaw::Coord);
  virtual Warsaw::Coord upper();
  virtual void upper(Warsaw::Coord);
  virtual Warsaw::Coord step();
  virtual void step(Warsaw::Coord);
  virtual Warsaw::Coord page();
  virtual void page(Warsaw::Coord);
  virtual Warsaw::Coord lvalue();
  virtual void lvalue(Warsaw::Coord);
  virtual Warsaw::Coord uvalue();
  virtual void uvalue(Warsaw::Coord);

  virtual void forward();
  virtual void backward();
  virtual void fastforward();
  virtual void fastbackward();
  virtual void begin();
  virtual void end();
  virtual void adjust(Warsaw::Coord);
private:  
  Warsaw::BoundedRange::Settings _settings;
  Warsaw::Coord                  _s;
  Warsaw::Coord                  _p;
  Prague::Mutex                  _mutex;
};

#endif
