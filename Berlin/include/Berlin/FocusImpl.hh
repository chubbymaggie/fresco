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
#ifndef _FocusImpl_hh
#define _FocusImpl_hh

#include <Warsaw/config.hh>
#include <Warsaw/Region.hh>
#include <Warsaw/Focus.hh>
#include <Berlin/DefaultPOA.hh>
#include <vector>
#include <stack>

class FocusImpl : public virtual POA_Warsaw::Focus,
                  public virtual PortableServer::RefCountServantBase,
                  private DefaultPOA
{
  friend class EventManager;
public:
  FocusImpl(Warsaw::Input::Device device) : _device(device) {}
  virtual ~FocusImpl() {}
  virtual Warsaw::Input::Device device() { return _device;}

  virtual bool request(Warsaw::Controller_ptr) = 0;
  virtual void restore(Warsaw::Region_ptr) = 0;
  virtual void damage(Warsaw::Region_ptr) = 0;
  virtual void dispatch(Warsaw::Input::Event &) = 0;
  PortableServer::POA_ptr _default_POA() { return DefaultPOA::_default_POA();}
protected:
  virtual void activate_composite() {}
private:
  const Warsaw::Input::Device _device;
};

#endif
