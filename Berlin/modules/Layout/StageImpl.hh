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
#ifndef _StageImpl_hh
#define _StageImpl_hh

#include <Warsaw/config.hh>
#include <Warsaw/Stage.hh>
#include <Warsaw/Traversal.hh>
#include <Berlin/ImplVar.hh>
#include <Berlin/GraphicImpl.hh>
#include <Berlin/RegionImpl.hh>
#include <Berlin/Geometry.hh>
#include <Prague/Sys/Thread.hh>
#include <list>

class StageHandleImpl;

class StageImpl : public virtual POA_Layout::Stage,
		  public GraphicImpl
{
  class Sequence;
  class Finder;
  class Quad;
  class QuadTree;
 public:
  StageImpl();
  ~StageImpl();

  virtual void request(Warsaw::Graphic::Requisition &);

  virtual void traverse(Warsaw::Traversal_ptr);

  virtual void allocate(Warsaw::Tag, const Warsaw::Allocation::Info &);
  virtual void need_redraw();
  virtual void need_redraw_region(Warsaw::Region_ptr);
  virtual void need_resize();
  //. relayout the children. If the bounding box changes call needResize on the parent
  
  virtual Warsaw::Region_ptr bbox();
  virtual CORBA::Long layers();
  virtual Layout::StageHandle_ptr layer(Layout::Stage::Index);
  /*
   * begin() and end() 'lock' the stage
   * in that only after the last end() conditions for needRedraw() & needResize() are done
   */
  virtual void begin();
  virtual void end();
  virtual Layout::StageHandle_ptr insert(Warsaw::Graphic_ptr, const Warsaw::Vertex &, const Warsaw::Vertex &, Layout::Stage::Index);
  virtual void remove(Layout::StageHandle_ptr);

  void move(StageHandleImpl *, const Warsaw::Vertex &);
  void resize(StageHandleImpl *, const Warsaw::Vertex &);
  void relayer(StageHandleImpl *, Layout::Stage::Index);
private:
  Warsaw::Tag tag();
  StageHandleImpl *tag_to_handle(Warsaw::Tag);
  void damage(StageHandleImpl *);

  Sequence *children;
  QuadTree *tree;
  long nesting;
  Impl_var<RegionImpl> _damage;
  Impl_var<RegionImpl> bbregion;
  bool _need_redraw : 1;
  bool _need_resize : 1;
  Prague::Mutex childMutex;
};

class StageHandleImpl : public virtual POA_Layout::StageHandle
{
 public:
  StageHandleImpl(StageImpl *, Warsaw::Graphic_ptr, Warsaw::Tag, const Warsaw::Vertex &, const Warsaw::Vertex &, Layout::Stage::Index);
  virtual Layout::Stage_ptr parent() { return stage->_this();}
  virtual Warsaw::Graphic_ptr child() { return Warsaw::Graphic::_duplicate(c);}
  virtual void remove();
  virtual Warsaw::Vertex position() { Prague::MutexGuard guard(mutex); return p;}
  virtual void position(const Warsaw::Vertex &);
  virtual Warsaw::Vertex size() { Prague::MutexGuard guard(mutex); return s;}
  virtual void size(const Warsaw::Vertex &);
  virtual Layout::Stage::Index layer() { Prague::MutexGuard guard(mutex); return l;}
  virtual void layer(Layout::Stage::Index);

  const Geometry::Rectangle<Warsaw::Coord> &bbox() { return boundingbox;}
  void bbox(RegionImpl &region)
    {
      region.valid   = true;
      region.lower.x = boundingbox.l;
      region.upper.x = boundingbox.r;
      region.xalign  = xalign;
      region.lower.y = boundingbox.t;
      region.upper.y = boundingbox.b;
      region.yalign  = yalign;
    }
//  private:
  void cache_bbox();
  StageImpl *stage;
  Warsaw::Graphic_var c;
  Warsaw::Tag tag;
  Warsaw::Vertex p;
  Warsaw::Vertex s;
  Layout::Stage::Index l;
  Geometry::Rectangle<Warsaw::Coord> boundingbox;
  Warsaw::Alignment xalign;
  Warsaw::Alignment yalign;
  Prague::Mutex mutex;
};

#endif
