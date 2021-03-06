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
#include <Prague/Sys/Tracer.hh>
#include "Berlin/IdentifiableImpl.hh"
#include <cassert>

using namespace Prague;
using namespace Fresco;

CORBA::Boolean Berlin::IdentifiableImpl::is_identical(Identifiable_ptr id)
{
    Trace trace(this, "IdentifiableImpl::is_identical");
    PortableServer::POA_var poa = my_poa;
    if (CORBA::is_nil(poa)) poa = _default_POA();
    try
    {
    PortableServer::ServantBase_var s = poa->reference_to_servant(id);
    if (s.in() == this) return 1;
    }
    catch (const PortableServer::POA::ObjectNotActive &) { }
    catch (const PortableServer::POA::WrongAdapter &) { }
    catch (const CORBA::OBJECT_NOT_EXIST &) { }
    return 0;
}
