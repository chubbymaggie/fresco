/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 2000 Stefan Seefeld <stefan@fresco.org> 
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
#ifndef _daVinci_ServantBase_hh
#define _daVinci_ServantBase_hh

#include <Prague/Sys/Tracer.hh>
#include <Fresco/config.hh>
#include <Fresco/Types.hh>
#include <cassert>

namespace daVinci
{
class ServantBase : public virtual PortableServer::ServantBase
{
public:
  ServantBase() {}
  virtual ~ServantBase() {}
  virtual void deactivate();
  static void poa(PortableServer::POA_ptr);
  static PortableServer::POA_ptr poa();
  static void activate(ServantBase *);
  static void deactivate(ServantBase *);
protected:
  //.if the servant is a composite, it needs to implement this method
  //.to activate the child servants
  virtual void activate_composite() {}
private:
  static PortableServer::POA_var _poa;
};
};

#endif
