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
#include <Berlin/Provider.hh>
#include <Berlin/RegionImpl.hh>
#include <Berlin/CommandImpl.hh>
#include "Scrollbar.hh"

using namespace Prague;
using namespace Fresco;
using namespace Berlin::WidgetKit::Motif;

Scrollbar::Scrollbar(BoundedRange_ptr v, Axis a, const Fresco::Graphic::Requisition &r)
  : _requisition(r),
    _value(RefCount_var<BoundedRange>::increment(v)),
    _axis(a)
{
  Trace trace("Scrollbar::Scrollbar");
  BoundedRange::Settings settings = _value->state();
  _offset.lower = settings.lvalue/(settings.upper - settings.lower);
  _offset.upper = settings.uvalue/(settings.upper - settings.lower);
  _value->attach(Observer_var(observer()));
}

void Scrollbar::init(Controller_ptr t)
{
  Trace trace("Scrollbar::init");
  body(t);
  t->add_parent_graphic(Graphic_var(_this()), 0);
  append_controller(t);
}

void Scrollbar::draw(DrawTraversal_ptr traversal)
{
  traverse_thumb(traversal);
}

void Scrollbar::pick(PickTraversal_ptr traversal)
{
//   if (grabbed(traversal->device()) || traversal->intersects_allocation())
  if (traversal->intersects_allocation())
    {
      traversal->enter_controller(Controller_var(_this()));
      MonoGraphic::traverse(traversal);
//       if (!grabbed(traversal->device())) traverse_thumb(traversal);
      traverse_thumb(traversal);
      if (!traversal->picked()) traversal->hit();
      traversal->leave_controller();
    }
}

void Scrollbar::allocate(Tag, const Allocation::Info &info)
{
  Lease_var<RegionImpl> allocation(Provider<RegionImpl>::provide());
  allocation->copy(info.allocation);
  if (_axis == xaxis)
    {
      Coord lower = allocation->lower.x;
      Coord scale = allocation->upper.x - allocation->lower.x;
      allocation->lower.x = lower + scale*_offset.lower;
      allocation->upper.x = lower + scale*_offset.upper;
    }
  else
    {
      Coord lower = allocation->lower.y;
      Coord scale = allocation->upper.y - allocation->lower.y;
      allocation->lower.y = lower + scale*_offset.lower;
      allocation->upper.y = lower + scale*_offset.upper;
    }
  allocation->lower.z = allocation->upper.z = 0.;
  allocation->normalize(info.transformation);
}

void Scrollbar::adjust(const OriginatedDelta &od)
{
  Vertex origin = od.origin;
  Vertex newpt = od.origin;
  newpt.x += od.delta.x;
  newpt.y += od.delta.y;
  
  _pickTrafo.inverse_transform_vertex(origin);
  _pickTrafo.inverse_transform_vertex(newpt);

  Coord delta;
  
  if (_axis == xaxis) {
    delta = newpt.x - origin.x;
    if (origin.x < 0.) {
      delta = std::max(origin.x + delta, 0.);
    } else if (origin.x > _length) {
      delta = std::min(origin.x + delta, 0.);
    }

  } else if (_axis == yaxis) {
    delta = newpt.y - origin.y;
    if (origin.y < 0.) {
      delta = std::max(origin.y + delta, 0.);
    } else if (origin.y > _length) {
      delta = std::min(origin.y + delta, 0.);
    }
  }
  if (delta != 0.) _value->adjust(_scale*delta);
}

void Scrollbar::update(const CORBA::Any &any)
{
  BoundedRange::Settings *settings;
  any >>= settings;
  _offset.lower = (settings->lvalue - settings->lower)/(settings->upper - settings->lower);
  _offset.upper = (settings->uvalue - settings->lower)/(settings->upper - settings->lower);
  need_redraw();
}

void Scrollbar::traverse_thumb(Traversal_ptr traversal)
{
  Graphic_var child = body();
  if (CORBA::is_nil(child)) return;
  Lease_var<RegionImpl> allocation(Provider<RegionImpl>::provide());
  allocation->copy(Region_var(traversal->current_allocation()));
  Lease_var<TransformImpl> tx(Provider<TransformImpl>::provide());
  tx->load_identity();
  if (_axis == xaxis)
    {
      Coord lower = allocation->lower.x;
      Coord scale = allocation->upper.x - allocation->lower.x;
      _length = scale;
      allocation->lower.x = lower + scale*_offset.lower;
      allocation->upper.x = lower + scale*_offset.upper;
      allocation->lower.z = allocation->upper.z = 0.;
    }
  else if (_axis == yaxis)
    {
      Coord lower = allocation->lower.y;
      Coord scale = allocation->upper.y - allocation->lower.y;
      _length = scale;
      allocation->lower.y = lower + scale*_offset.lower;
      allocation->upper.y = lower + scale*_offset.upper;
    }
  allocation->lower.z = allocation->upper.z = 0.;
  allocation->normalize(Transform_var(tx->_this()));
  _scale = (_value->upper() - _value->lower())/_length;
  _pickTrafo.copy(traversal->current_transformation());
  try { traversal->traverse_child (child, 0, Region_var(allocation->_this()), Transform_var(tx->_this()));}
  catch (const CORBA::OBJECT_NOT_EXIST &) { body(Fresco::Graphic::_nil());}
  catch (const CORBA::COMM_FAILURE &) { body(Fresco::Graphic::_nil());}
}
