/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 2000 Stefan Seefeld <stefan@berlin-consortium.org> 
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
#ifndef _RefCount_var_hh
#define _RefCount_var_hh

#include <Warsaw/config.hh>
#include <Warsaw/Types.hh>

//.RefCount_var is a smart pointer to be used in conjunction with
//.RefCountBase objects.
template <typename T>
class RefCount_var
{
  typedef typename T::_ptr_type T_ptr;
  typedef typename T::_var_type T_var;
public:
  RefCount_var(T_ptr tt = 0) : t(tt) {}
  RefCount_var(const RefCount_var<T> &o) : t(o.t) { if (!CORBA::is_nil(t)) t->increment();}
  ~RefCount_var() { if (!CORBA::is_nil(t)) t->decrement();}
  RefCount_var<T> &operator = (const RefCount_var<T> &o)
    {
      if (&o != this)
        {
          if (!CORBA::is_nil(t)) t->decrement();
          t = o.t;
	  if (!CORBA::is_nil(t)) t->increase();
        }
      return *this;
    }
  RefCount_var<T> &operator = (T_ptr tt)
    {
      if (!CORBA::is_nil(t)) t->decrement();
      t = tt;
      return *this;
    }
  T_ptr operator->() const { return t;}
  operator T_ptr () const { return t;}
  T_ptr _retn() { T_ptr tmp = t._retn(); t = 0; return tmp;}

  static T_ptr increment(T_ptr t, bool dup = true)
  {
    if (!CORBA::is_nil(t)) t->increment();
    return dup ? T::_duplicate(t) : t;
  }
private:
  T_var t;
};

#endif