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
#include <Warsaw/config.hh>
#include <Warsaw/Traversal.hh>
#include <Berlin/ImplVar.hh>
#include <Berlin/Provider.hh>
#include <Berlin/TransformImpl.hh>
#include <Prague/Sys/Tracer.hh>
#include "Layout/Box.hh"
#include "Layout/LayoutManager.hh"
#include "Layout/Placement.hh"
#include <iostream>
#include <strstream>

using namespace Prague;
using namespace Warsaw;

Box::Box(LayoutManager *l)
  : _layout(l),
    _requested(false),
    _cache_allocations(0)
{
  _obj_name = new char[256];
  
  _box_name = "Box";
  std::ostrstream buf;
  buf << _box_name << " " << _layout->name() << std::ends;
  strcpy(_obj_name, buf.str());
}

Box::~Box()
{
  if (_cache_allocations)
    {
      for (CORBA::Long i = 0; i != _cache_size; ++i)
	Provider<RegionImpl>::adopt(_cache_allocations[i]);
      delete[] _cache_allocations;
    }
  delete _obj_name;
  delete _layout;
}

void Box::request(Warsaw::Graphic::Requisition &r)
{
  Trace trace(this, "Box::request");
  if (!_requested)
    {
      GraphicImpl::default_requisition(_requisition);
      GraphicImpl::init_requisition(_requisition);
      long n = num_children();
      if (n > 0)
	{
	  Warsaw::Graphic::Requisition *r = children_requests();
	  _layout->request(n, r, _requisition);
	  deallocate_requisitions(r);
	}
      _requested = true;
    }
  r = _requisition;
}

void Box::extension(const Allocation::Info &info, Region_ptr ext_region)
{
  Trace trace(this, "Box::extension");  
  long size = num_children();
  if (size > 0)
    {
      Allocation::Info child;
      Vertex origin, previous, delta;
      previous.x = previous.y = previous.z = 0;

      Lease_var<TransformImpl> child_tx(Provider<TransformImpl>::provide());
      Lease_var<TransformImpl> tmp_tx(Provider<TransformImpl>::provide());  
      Lease_var<RegionImpl> region(Provider<RegionImpl>::provide());  
      Region_var region_this(region->_this());
      Transform_var tmp_tx_this(tmp_tx->_this());
      tmp_tx->load_identity();
      child_tx->load_identity();
      
      child.transformation = child_tx->_this();
      child.transformation->copy(info.transformation);
      LayoutManager::Allocations result = children_allocations(info.allocation);
      for (long i = 0; i < size; i++)
	{
#if 1
	  *region = *result[i];
	  region->normalize(origin);
	  delta = origin - previous;
// 	  tmp_tx->loadIdentity();
	  tmp_tx->translate(delta);
	  child.allocation = region_this;
	  child.transformation->premultiply(tmp_tx_this);
	  child_extension(i, child, ext_region);
	  previous = origin;
#else
	  result[i]->normalize(origin);
	  delta = origin - previous;
// 	  tmp_tx->loadIdentity();
	  tmp_tx->translate(delta);
	  child.allocation = result[i]->_this();
	  child.transformation->premultiply(Transform_var(tmp_tx->_this()));
	  child_extension(i, child, ext_region);
	  previous = origin;
#endif
	}
#ifndef USE_ALLOCATION_CACHE
      for (CORBA::Long i = 0; i != size; ++i) Provider<RegionImpl>::adopt(result[i]);
      delete [] result;
#endif
    }
}

void Box::traverse(Traversal_ptr traversal)
{
  Trace trace(this, "Box::traverse");
  if (num_children())
    {
      Region_var given = traversal->current_allocation();
      if (!CORBA::is_nil(given))
 	{
	  /*
	   * this cull test is not accurate, it assumes that the children
	   * don't draw outside the box' allocation.
	   * the alternative - using extension - is expensive...
	   *              -stefan
	   */
	  if (traversal->intersects_allocation())
	    traverse_with_allocation(traversal, given);
	}
      else traverse_without_allocation(traversal);
    }
}

void Box::need_resize()
{
  _requested = false;
  if (_cache_allocations)
    {
      for (CORBA::Long i = 0; i != _cache_size; ++i)
	Provider<RegionImpl>::adopt(_cache_allocations[i]);
      delete [] _cache_allocations;
      _cache_allocations = 0;
    }
  {
    std::ostrstream buf;
    buf << _box_name << " " << _layout->name() << " with " << num_children() << " children" << std::ends;
    strcpy(_obj_name, buf.str());
  }
  PolyGraphic::need_resize();
}

void Box::need_resize(Tag)
{
  need_resize();
}

/*
 * this is a method called (but left empty in the superclass) in
 * PolyGraphic::allocate. it is called after a particular child has been
 * located in the child list. It is supposed to "finish off" providing the
 * allocation info for the given child 
 */
void Box::allocate(Tag tag, const Allocation::Info &info)
{
  Trace trace(this, "Box::allocate");
  /*
   * fetch requested (presumably allocated) child regions
   */
  CORBA::Long n = num_children();
  LayoutManager::Allocations result = children_allocations(info.allocation);
  Lease_var<TransformImpl> tx(Provider<TransformImpl>::provide());
  tx->load_identity();

  /*
   * copy transformation and region into allocation
   */
  CORBA::Long idx = child_id_to_index(tag);
  Logger::log(Logger::layout) << "Allocating child " << idx << "\n";
  Lease_var<RegionImpl> region( Provider<RegionImpl>::provide() );
  *region = *result[idx];
  region->normalize(Transform_var(tx->_this()));
  info.transformation->premultiply(Transform_var(tx->_this()));
  info.allocation->copy(Region_var(region->_this()));
}


/*
 * this is called from Box::allocate to resolve the layout of the box's
 * children by (a) asking the children how big they want to be, then (b)
 * delegating the actual allocation to the current layoutManager. It also caches
 * the children's requests so that the real layout (at draw time) will happen
 * faster. 
 */
LayoutManager::Allocations Box::children_allocations(Region_ptr allocation)
{
  Trace trace(this, "Box::children_allocations");
  if (!_cache_allocations)
    {
      CORBA::Long children = num_children();
      Warsaw::Graphic::Requisition *childrenRequisitions = children_requests();
	
      // cache integrated form of children requisitions
      if (!_requested)
	{
	  GraphicImpl::init_requisition(_requisition);
	  _layout->request(children, childrenRequisitions, _requisition);
	  _requested = true;
	}
      // build region array for children
      RegionImpl **childrenRegions = new RegionImpl *[children];
      for (CORBA::Long i = 0; i < children; i++)
	{
	  childrenRegions[i] = Provider<RegionImpl>::provide();
	  childrenRegions[i]->valid = true;
	}
      // fill in children regions which are reasonable matches for the given requesitions
      _layout->allocate(children, childrenRequisitions, allocation, childrenRegions);
      deallocate_requisitions(childrenRequisitions);
      _cache_allocations = childrenRegions;
      _cache_size = children;
    }
#ifndef USE_ALLOCATION_CACHE
  // Return a copy of the region array for old code that changes the regions
  RegionImpl **childrenRegions = _cache_allocations;
//   new RegionImpl *[_cache_size];
//   for (CORBA::Long i = 0; i < _cache_size; i++)
//     {
//       childrenRegions[i] = Provider<RegionImpl>::provide();
//       *childrenRegions[i] = *_cache_allocations[i];
//     }
  _cache_allocations = 0;
  return childrenRegions;
#else
  return _cache_allocations;
#endif
}

void Box::traverse_with_allocation(Traversal_ptr t, Region_ptr r)
{
  Trace trace(this, "Box::traverse_with_allocation");
  LayoutManager::Allocations result = children_allocations(r);

  Lease_var<TransformImpl> tx(Provider<TransformImpl>::provide());
  Lease_var<RegionImpl> region(Provider<RegionImpl>::provide());
  Transform_var tx_this(tx->_this());
  Region_var region_this(region->_this());
  tx->load_identity();

  CORBA::Long size = num_children();
  CORBA::Long begin, end, incr;
  if (t->direction() == Traversal::up)
    {
      begin = 0;
      end = size;
      incr = 1;
    }
  else
    {
      begin = size - 1;
      end = -1;
      incr = -1;
    }
  for (CORBA::Long i = begin; i != end; i += incr)
    {
      if (CORBA::is_nil(_children[i].peer)) continue;
      Vertex origin;
      *region = *result[i];
      region->normalize(origin);
      tx->load_identity();
      /*
       * ok, so we stipulate that Boxes lay out their children 
       * only translating them -stefan
       */
      tx->translate(origin);
      try { t->traverse_child(_children[i].peer, _children[i].localId, region_this, tx_this);}
      catch (const CORBA::OBJECT_NOT_EXIST &) { _children [i].peer = Warsaw::Graphic::_nil ();}
      catch (const CORBA::COMM_FAILURE &) { _children [i].peer = Warsaw::Graphic::_nil ();}
      if (!t->ok()) break;
    }
#ifndef USE_ALLOCATION_CACHE
  for (CORBA::Long i = 0; i != size; ++i) Provider<RegionImpl>::adopt(result[i]);
  delete [] result;
#endif
}

void Box::traverse_without_allocation(Traversal_ptr t)
{
  Trace trace(this, "Box::traverse_without_allocation");
  if (t->direction() == Traversal::up)
    {
      for (glist_t::iterator i = _children.begin(); i != _children.end() && t->ok(); i++)
	{
	  if (CORBA::is_nil(i->peer)) continue;
	  try { t->traverse_child (i->peer, i->localId, Region::_nil (), Transform::_nil ());}
	  catch (const CORBA::OBJECT_NOT_EXIST &) { i->peer = Warsaw::Graphic::_nil ();}
	  catch (const CORBA::COMM_FAILURE &) { i->peer = Warsaw::Graphic::_nil ();}
	}
    }
  else
    {
      for (glist_t::reverse_iterator i = _children.rbegin(); i != _children.rend() && t->ok(); i++)
	{
	  if (CORBA::is_nil (i->peer)) continue;
	  try { t->traverse_child (i->peer, i->localId, Region::_nil(), Transform::_nil());}
	  catch (const CORBA::OBJECT_NOT_EXIST &) { i->peer = Warsaw::Graphic::_nil();}
	  catch (const CORBA::COMM_FAILURE &) { i->peer = Warsaw::Graphic::_nil();}
	}
    }
}

BoxAlignElements::BoxAlignElements(LayoutManager *layout, Axis a, Alignment align)
  : Box(layout), axis(a), alignment(align)
{
  _box_name = "BoxAlignElements";
  std::ostrstream buf;
  buf << _box_name << " " << layout->name() << std::ends;
  strcpy(_obj_name, buf.str());
}

BoxAlignElements::~BoxAlignElements() {}

void BoxAlignElements::append_graphic(Graphic_ptr g)
{
  Placement *placement = new Placement(new LayoutCenter(axis, alignment));
  placement->body(g);
  Box::append_graphic(Graphic_var(placement->_this()));
}

void BoxAlignElements::prepend_graphic(Graphic_ptr g)
{
  Placement *placement = new Placement(new LayoutCenter(axis, alignment));
  placement->body(g);
  Box::prepend_graphic(Graphic_var(placement->_this()));
}
