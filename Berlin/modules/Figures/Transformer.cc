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
#include <Warsaw/Region.hh>
#include <Warsaw/Traversal.hh>
#include <Warsaw/DrawTraversal.hh>
#include <Warsaw/DrawingKit.hh>
#include <Warsaw/IO.hh>
#include <Berlin/TransformImpl.hh>
#include <Berlin/Providers.hh>
#include <Berlin/RegionImpl.hh>
#include "Figure/Transformer.hh"
#include <Prague/Sys/Tracer.hh>

using namespace Prague;

Transformer::Transformer()
{
  transform = new TransformImpl;
}

Transformer::~Transformer()
{
//  transform->_dispose();
}

Transform_ptr Transformer::transformation()
{
  return transform->_this();
}

void Transformer::request(Requisition &requisition)
{
  Trace trace("Transformer::request");
  Allocator::request(requisition);
//   cout << "request before trafo " << requisition << endl;
  GraphicImpl::transformRequest(requisition, Transform_var(transform->_this()));
//   cout << "request after trafo " << requisition << endl;
}

void Transformer::traverse(Traversal_ptr traversal)
{
  Trace trace("Transformer::traverse");
  Graphic_var child = body();
  if (CORBA::is_nil(child)) return;
  if (!transform->Identity())
    {
      Lease<RegionImpl> rr;
      Providers::region.provide(rr);
      rr->copy(traversal->allocation());
	  
      Requisition r;
      GraphicImpl::initRequisition(r);
	  
      Allocator::request(r);

      Lease<TransformImpl> tx;
      Providers::trafo.provide(tx);
      tx->loadIdentity();
//       cout << "allocation before trafo " << *rr << endl;
      Vertex delta = GraphicImpl::transformAllocate(*rr, r, Transform_var(transform->_this()));
//       cout << "allocation after trafo " << *rr << endl;
      tx->copy(Transform_var(transform->_this()));
      traversal->traverseChild(child, 0, Region_var(rr->_this()), Transform_var(tx->_this()));
    }
  else Allocator::traverse(traversal);
}

void Transformer::allocate(Tag, const Allocation::Info &info)
{
  Trace trace("Transformer::allocate");
  if (!transform->Identity())
    {
      if (!CORBA::is_nil(info.allocation))
	{
	  Lease<RegionImpl> rr;
	  Providers::region.provide(rr);
	  rr->copy(info.allocation);
	  Requisition r;
	  GraphicImpl::initRequisition(r);
	  Allocator::request(r);
	  Lease<TransformImpl> tx;
	  Providers::trafo.provide(tx);
	  tx->loadIdentity();
	  Vertex delta = GraphicImpl::transformAllocate(*rr, r, Transform_var(transform->_this()));
 	  tx->copy(Transform_var(transform->_this()));
	  info.transformation->premultiply(Transform_var(tx->_this()));
	  info.allocation->copy(Region_var(rr->_this()));
        }
      else
	{
	  info.transformation->premultiply(Transform_var(transform->_this()));
	  Allocator::allocate(0, info);
        }
    }
  else Allocator::allocate(0, info);
}
