/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Stefan Seefeld <seefelds@magellan.umontreal.ca> 
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
#include "Berlin/PickTraversalImpl.hh"

PickTraversalImpl::PickTraversalImpl(Graphic_ptr g, Region_ptr r, const Event::Pointer &p, EventManager *em)
  : TraversalImpl(g, r), pointer(p), manager(em), memento(0) {}
PickTraversalImpl::PickTraversalImpl(const PickTraversalImpl &t)
  : TraversalImpl(t), pointer(t.pointer), manager(t.manager), memento(0), controller(t.controller) {}
PickTraversalImpl::~PickTraversalImpl() { if (memento) memento->_dispose();}
