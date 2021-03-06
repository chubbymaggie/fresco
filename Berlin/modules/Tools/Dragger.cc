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

#include <Prague/Sys/Tracer.hh>
#include <Berlin/Vertex.hh>
#include <Berlin/Logger.hh>
#include "Dragger.hh"

using namespace Prague;
using namespace Fresco;

using namespace Berlin::ToolKit;

Dragger::Dragger(Command_ptr c) :
    ControllerImpl(false),
    my_command(Command::_duplicate(c))
{ Trace trace("Dragger::Dragger"); }

Dragger::~Dragger()
{
  Trace trace("Dragger::~Dragger");
  if (!CORBA::is_nil(my_command))
    try { my_command->destroy(); }
    catch (const CORBA::OBJECT_NOT_EXIST &) { }
    catch (const CORBA::COMM_FAILURE &) { }
}
void Dragger::press(PickTraversal_ptr traversal, const Input::Event &event)
{
    ControllerImpl::press(traversal, event);
    my_offset = event[1].attr.location();
}

void Dragger::drag(PickTraversal_ptr traversal, const Input::Event &event)
{
    Vertex delta = event[0].attr.location() - my_offset;
    OriginatedDelta od;
    od.delta = delta;
    od.origin = my_offset;

    CORBA::Any any;
    any <<= od;
    if (!CORBA::is_nil(my_command))
    try { my_command->execute(any); }
    catch (const CORBA::OBJECT_NOT_EXIST &)
    { my_command = Fresco::Command::_nil(); }
    catch (const CORBA::COMM_FAILURE &)
    { my_command = Fresco::Command::_nil(); }
    my_offset += delta;
}

void Dragger::release(PickTraversal_ptr traversal,
              const Input::Event &event)
{ ControllerImpl::release(traversal, event); }
