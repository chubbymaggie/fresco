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
#ifndef _PickTraversalImpl_hh
#define _PickTraversalImpl_hh

#include <Warsaw/config.hh>
#include <Warsaw/Input.hh>
#include <Warsaw/Controller.hh>
#include <Warsaw/PickTraversal.hh>
#include <Warsaw/Transform.hh>
#include <Warsaw/Focus.hh>
#include <Berlin/TraversalImpl.hh>
#include <Berlin/RegionImpl.hh>
#include <Berlin/Vertex.hh>
#include <Prague/Sys/Tracer.hh>

class PickTraversalImpl : public virtual POA_Warsaw::PickTraversal,
                          public TraversalImpl
{
  typedef vector<Warsaw::Controller_var> cstack_t;
  typedef vector<size_t> pstack_t;
public:
  PickTraversalImpl(Warsaw::Graphic_ptr, Warsaw::Region_ptr, Warsaw::Transform_ptr, const Warsaw::Input::Position &, Warsaw::Focus_ptr);
  //. to be used when starting from root level
  ~PickTraversalImpl();
  virtual Warsaw::Region_ptr current_allocation();
  virtual Warsaw::Transform_ptr current_transformation();
  virtual Warsaw::Graphic_ptr current_graphic();
  virtual void traverse_child(Warsaw::Graphic_ptr, Warsaw::Tag, Warsaw::Region_ptr, Warsaw::Transform_ptr);
  virtual void visit(Warsaw::Graphic_ptr);
  virtual Warsaw::Traversal::order direction();
  virtual CORBA::Boolean ok();
  virtual CORBA::Boolean intersects_allocation();
  virtual CORBA::Boolean intersects_region(Warsaw::Region_ptr);
  virtual Warsaw::Input::Device device();
  virtual void enter_controller(Warsaw::Controller_ptr);
  virtual void leave_controller();
  virtual void hit();
  virtual CORBA::Boolean picked();
  virtual void grab();
  virtual void ungrab();
  virtual CORBA::Boolean forward();
  virtual CORBA::Boolean backward();

  void pop_controller();
  Warsaw::Controller_ptr top_controller();
  const vector<Warsaw::Controller_var> &controllerStack() const { return _controllers;}
  PickTraversalImpl   *memento() { PickTraversalImpl *m = _mem; _mem = 0; return m;}
  void reset(const Warsaw::Input::Position &);
  void debug();
private:
  //. to be used to create the memento
  PickTraversalImpl(const PickTraversalImpl &);
  const State &top() const { return get(size() - 1);}
  const State &current() const { return get(_cursor);}
  cstack_t                   _controllers;
  pstack_t                   _positions;
  Warsaw::Input::Position    _pointer;
  Warsaw::Focus_var          _focus;
  PickTraversalImpl         *_mem;
  size_t                     _cursor;
  Warsaw::PickTraversal_var __this;
};

//. remove one controller level from the top, it might have got out of scope
inline void PickTraversalImpl::pop_controller()
{
  Prague::Trace trace("PickTraversal::pop_controller");
  if (_controllers.size())
    {
      while (size() > _positions.back()) pop();
      _controllers.pop_back();
      _positions.pop_back();
    }
}

inline Warsaw::Controller_ptr PickTraversalImpl::top_controller()
{
  return _controllers.size() ? Warsaw::Controller::_duplicate(_controllers.back()) : Warsaw::Controller::_nil();
}

inline void PickTraversalImpl::reset(const Warsaw::Input::Position &p)
//. pop all graphics up to the top most controller and set the pointer
//. so the traversal can be used to start over directly at the top
{
  Prague::Trace trace("PickTraversal::reset");
  pop_controller();
  _pointer = p;
}

#endif 
