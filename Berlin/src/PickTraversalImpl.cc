/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999, 2000 Stefan Seefeld <stefan@berlin-consortium.org> 
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
#include <Prague/Sys/Tracer.hh>
#include <Warsaw/config.hh>
#include <Warsaw/IO.hh>
#include "Berlin/PickTraversalImpl.hh"
#include "Berlin/PositionalFocus.hh"

using namespace Prague;
using namespace Warsaw;

PickTraversalImpl::PickTraversalImpl(Graphic_ptr g, Region_ptr r, Transform_ptr t, PositionalFocus *f)
  : TraversalImpl(g, r, t),
    _focus(f),
    _memento(0),
    _picked(false),
    _cursor(0)
{
  Trace trace("PickTraversalImpl::PickTraversalImpl");
  __this = POA_Warsaw::PickTraversal::_this();
}

PickTraversalImpl::~PickTraversalImpl() {}

PickTraversal_ptr PickTraversalImpl::_this()
{
  return Warsaw::PickTraversal::_duplicate(__this);
}

Region_ptr PickTraversalImpl::current_allocation()
{
  Trace trace("PickTraversalImpl::current_allocation");
  return Region::_duplicate(current().allocation);
}

Transform_ptr PickTraversalImpl::current_transformation() 
{
  Trace trace("PickTraversalImpl::current_transformation");
  return current().transformation->_this();
}

Graphic_ptr PickTraversalImpl::current_graphic()
{
  Trace trace("PickTraversalImpl::current_graphic");
  return Graphic::_duplicate(current().graphic);
}

void PickTraversalImpl::traverse_child(Graphic_ptr child, Tag tag, Region_ptr region, Transform_ptr transform)
{
  Trace trace("PickTraversalImpl::traverse_child");
  if (CORBA::is_nil(region)) region = Region_var(current_allocation());
  Lease_var<TransformImpl> cumulative(Provider<TransformImpl>::provide());
  cumulative->copy(Transform_var(current_transformation()));
  if (!CORBA::is_nil(transform)) cumulative->premultiply(transform);
  push(child, tag, region, cumulative._retn());
  _cursor++;
  try { child->traverse(__this);}
  catch (...) { _cursor--; pop(); throw;}
  _cursor--;
  pop(); 
}

void PickTraversalImpl::visit(Warsaw::Graphic_ptr g) { g->pick(__this);}
Warsaw::Traversal::order PickTraversalImpl::direction() { return Warsaw::Traversal::down;}
CORBA::Boolean PickTraversalImpl::ok() { return !_picked;}

CORBA::Boolean PickTraversalImpl::intersects_region(Region_ptr region)
{
  Transform::Matrix matrix;
  Transform_var transform = current_transformation();
  transform->store_matrix(matrix);
  Coord d = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  if (d == 0.) return false;
  Coord x = _pointer.x - matrix[0][3];
  Coord y = _pointer.y - matrix[1][3];
  Vertex local;
  local.x = (matrix[1][1] * x - matrix[0][1] * y)/d;
  local.y = (matrix[0][0] * y - matrix[1][0] * x)/d;
  Vertex lower, upper;
  region->bounds(lower, upper);
  return lower.x <= local.x && local.x <= upper.x && lower.y <= local.y && local.y <= upper.y;
}

CORBA::Boolean PickTraversalImpl::intersects_allocation()
{
  Region_var region = current_allocation();
  return intersects_region(region);
}

Warsaw::Input::Device PickTraversalImpl::device() { return _focus->device();}
void PickTraversalImpl::enter_controller(Controller_ptr c)
{
  Trace trace("PickTraversal::enter_controller");
  _controllers.push_back(Controller::_duplicate(c));
  _positions.push_back(size());
}

void PickTraversalImpl::leave_controller()
{
  Trace trace("PickTraversal::leave_controller");
  _controllers.pop_back();
  _positions.pop_back();
}

void PickTraversalImpl::hit()
{
  Trace trace("PickTraversal::hit");
  _memento->copy(this);
  _picked = true;
}

CORBA::Boolean PickTraversalImpl::picked() { return _picked;}
void PickTraversalImpl::grab() { _focus->grab();}
void PickTraversalImpl::ungrab() { _focus->ungrab();}
CORBA::Boolean PickTraversalImpl::forward()
{
  if (_cursor + 1 < size()) { ++_cursor; return true;}
  return false;
}

CORBA::Boolean PickTraversalImpl::backward()
{
  if (_cursor > _positions.back()) { --_cursor; return true;}
  return false;
}

void PickTraversalImpl::copy(const PickTraversalImpl *traversal)
{
  TraversalImpl::copy(traversal);
  _controllers = traversal->_controllers;
  _positions = traversal->_positions;
  _pointer = traversal->_pointer;
//   _focus = traversal->_focus;
//   _memento = traversal;
  _cursor = traversal->_positions.back() - 1; // the 'current' graphic is the picked controller
  
}

void PickTraversalImpl::debug()
{
  cout << "PickTraversal::debug : stack size = " << size() << '\n';
  cout << "Controllers at ";
  for (size_t i = 0; i != _positions.size(); i++) cout << _positions[i] << ' ';
  cout << endl;
  Region_var r = current_allocation();
  Transform_var t = current_transformation();
  RegionImpl region(r, t);
  cout << "current allocation is " << region << endl;
  cout << "pointer is " << _pointer << endl;
  Vertex local = _pointer;
  Transform::Matrix matrix;
  t->store_matrix(matrix);
  cout << "current trafo \n" << matrix;
  t->inverse_transform_vertex(local);
  region.copy(r);
  cout << "local CS: current allocation is " << region << endl;
  cout << "local CS: pointer is " << local << endl;      
}
