/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Stefan Seefeld <seefelds@magellan.umontreal.ca> 
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
#include <Warsaw/PickTraversal.hh>
#include <Warsaw/DrawTraversal.hh>
#include <Warsaw/DrawingKit.hh>
#include <Warsaw/Pencil.hh>
#include <Berlin/Geometry.hh>
#include <Berlin/TransformImpl.hh>
#include <Berlin/RegionImpl.hh>
#include <Figure/FigureImpl.hh>

using namespace Geometry;

TransformFigure::TransformFigure()
{
  tx    = new TransformImpl;
  tx->_obj_is_ready(CORBA::BOA::getBOA());
  ext = new RegionImpl;
  ext->_obj_is_ready(CORBA::BOA::getBOA());
}

TransformFigure::~TransformFigure()
{
  ext->_dispose();
  tx->_dispose();
}

Transform_ptr TransformFigure::transformation() { return Transform::_duplicate(tx);}
void TransformFigure::request(Requisition &r)
{
  Allocation::Info info;
  RegionImpl *region = new RegionImpl;
  region->_obj_is_ready(_boa());
  extension(info, region);
  needRedrawRegion(region);
  if (region->valid)
    {
      Coord x_lead = -region->lower.x, x_trail = region->upper.x;
      Coord y_lead = -region->lower.y, y_trail = region->upper.y;
      GraphicImpl::requireLeadTrail(r.x, x_lead, x_lead, x_lead, x_trail, x_trail, x_trail);
      GraphicImpl::requireLeadTrail(r.y, y_lead, y_lead, y_lead, y_trail, y_trail, y_trail);
      r.z.defined = false;
    }
  else
    {
      r.x.defined = false;
      r.y.defined = false;
      r.z.defined = false;
    }
  region->_dispose();
}

void TransformFigure::extension(const Allocation::Info &info, Region_ptr region)
{
  if (ext->valid)
    {
      RegionImpl *tmp = new RegionImpl(ext, Transform_var(Transform::_nil()));
      tmp->_obj_is_ready(_boa());
      tmp->xalign = tmp->yalign = tmp->zalign = 0.;
      TransformImpl *transformation = new TransformImpl;
      transformation->_obj_is_ready(_boa());
      if (!CORBA::is_nil(info.transformation)) transformation->copy(info.transformation);
      transformation->premultiply(tx);
      tmp->applyTransform(transformation);
      region->mergeUnion(tmp);
      transformation->_dispose();
      tmp->_dispose();
    }
}

void TransformFigure::pick(PickTraversal_ptr traversal)
{
  if (ext->valid && traversal->intersectsRegion(ext))
    traversal->hit();
}

void TransformFigure::needRedraw()
{
  Allocation::Info info;
  RegionImpl *region = new RegionImpl;
  region->_obj_is_ready(_boa());
  extension(info, region);
  needRedrawRegion(region);
  region->_dispose();
}

void TransformFigure::resize() {}
void TransformFigure::copy(const TransformFigure &tf)
{
  mode = tf.mode;
  fg = tf.fg;
  bg = tf.bg;
  tx->copy(tf.tx);
  if (tf.ext->valid) ext->copy(tf.ext);
}

FigureImpl::FigureImpl() { path = new Figure::Vertices;}
FigureImpl::~FigureImpl() {}

void FigureImpl::addPoint(Coord x, Coord y)
{
  if (path->length() == 0)
    {
      ext->lower.x = x;
      ext->upper.x = x;
      ext->lower.y = y;
      ext->upper.y = y;
      ext->lower.z = Coord(0);
      ext->upper.z = Coord(0);
      ext->valid = true;
    }
  else
    {
      ext->lower.x = Math::min(ext->lower.x, x);
      ext->upper.x = Math::max(ext->upper.x, x);
      ext->lower.y = Math::min(ext->lower.y, y);
      ext->upper.y = Math::max(ext->upper.y, y);
    }
  Vertex v;
  v.x = x;
  v.y = y;
  v.z = 0.;
//   FigureImpl::growVertices(v_);
  CORBA::ULong n = path->length();
  path->length(n + 1);
  path[n] = v;
}

void FigureImpl::extension(const Allocation::Info &info, Region_ptr region)
{
  if (path->length() > 0)
    {
      RegionImpl *tmp = new RegionImpl(ext, Transform_var(Transform::_nil()));
      tmp->xalign = tmp->yalign = tmp->zalign = 0.;
      tmp->_obj_is_ready(_boa());
      TransformImpl *transformation = new TransformImpl;
      transformation->_obj_is_ready(_boa());
      if (!CORBA::is_nil(info.transformation)) transformation->copy(info.transformation);
      transformation->premultiply(tx);
      tmp->applyTransform(transformation);
      if (mode |= Figure::stroke)
	{
	  Coord w = 1.;
// 	  if (is_not_nil(style_))
// 	    {
// 	      Brush_var b = style_->brush_attr();
// 	      if (is_not_nil(b))
// 		{
// 		  XfBrush::Info* i;
// 		  b->brush_info(i);
// 		  if (!Math::equal(i->width, float(0), float(1e-2))) w = i->width;
//                 }
//             }
	  tmp->lower.x -= w; tmp->upper.x += w;
	  tmp->lower.y -= w; tmp->upper.y += w;
	  tmp->lower.z -= w; tmp->upper.z += w;
	}
      region->mergeUnion(tmp);
    }
}

void FigureImpl::draw(DrawTraversal_ptr traversal)
{
  if (path->length() > 0)
    {
      // bounding box culling, use extension(...) to add brush effect into extension.
      Allocation::Info info;
      RegionImpl *region = new RegionImpl;
      region->_obj_is_ready(_boa());
      extension(info, region);
      if (traversal->intersectsRegion(region))
	{
	  ::Path p;
	  p.p = path;
	  CORBA::ULong n = p.p.length();
	  Transform_var transformation = traversal->transformation();
	  for (CORBA::ULong i = 0; i != n; i++) transformation->transformVertex(p.p[i]);
	  Style::Spec style;
	  if (mode == stroke | fill) style.length(2);
	  else style.length(1);
	  CORBA::ULong i = 0;
	  if (mode |= stroke) style[i].a = Style::linecolor, style[i].val <<= fg, i++;
	  if (mode |= fill) style[i].a = Style::fillcolor, style[i].val <<= bg;
	  DrawingKit_var drawing = traversal->kit();
	  Pencil_var pencil = drawing->getPencil(style);
	  pencil->drawPath(p);
	}
      region->_dispose();
    }
}

/*
 * Picking just does a bounding box test for now.
 */

void FigureImpl::pick(PickTraversal_ptr traversal)
{
//   if (ext->defined)
//     {
//       if (traversal->intersectsRegion(ext))
// 	{
// 	  Vertex lower, upper, center;
// 	  Region_var visible = p->visible();
// 	  visible->bounds(lower, upper);
// 	  visible->center(center);
// 	  bool hit = false;

// 	  // FvD - define the surrounding box of the figure in terms
// 	  // of display pixels rather than coordinates ... cause if
// 	  // you scale > 1 you figure your picking rect will grow too
// 	  TransformImpl::Matrix mat;
// 	  Transform_var(p->current_matrix())->store_matrix(mat);
// 	  // FvD I agree, the old code was buggy on rotated figures ... -sigh
// 	  // so we have to pay the price - sniff (I know, it's still a crapy
// 	  Coord xspace = 4.0 / (mat[0][0]*mat[0][0] + mat[0][1]*mat[0][1]);
// 	  Coord yspace = 4.0 / (mat[1][0]*mat[1][0] + mat[1][1]*mat[1][1]);
// 	  xspace = sqrt(xspace);
// 	  yspace = sqrt(yspace);
// 	  BoxObj box(lower.x-xspace, lower.y-yspace, upper.x+xspace, upper.y+yspace);
// 	  //BoxObj box(lower.x-Coord(2), lower.y-Coord(2), upper.x+Coord(2), upper.y+Coord(2));
// 	  if (!closed_ && !curved_)
// 	    {
// 	      if (mode_ == FigureKit::stroke)
// 		{
// 		  MultiLineObj ml(v_);
// 		  PointObj pt(center.x, center.y);
// 		  if (ml.intersects(box)) hit = true;
//                 }
// 	      else if (mode_ == FigureKit::fill_stroke ||
// 		       mode_ == FigureKit::fill)
// 		{
// 		  FillPolygonObj fp(v_);
// 		  PointObj pt(center.x, center.y);
// 		  if (fp.intersects(box)) hit = true;
// 		}
//             }
// 	  else if (closed_ && !curved_)
// 	    {
// 	      if (mode_ == FigureKit::stroke)
// 		{
// 		  grow_vertices(v_);
// 		  (*v_)[v_->_length] = (*v_)[0];
// 		  v_->_length++;
// 		  MultiLineObj ml(v_);
// 		  if (ml.intersects(box)) hit = true;
// 		  v_->_length--;
//                 }
// 	      else if (mode_ == FigureKit::fill_stroke ||
// 		       mode_ == FigureKit::fill)
// 		{
// 		  FillPolygonObj fp(v_);
// 		  PointObj pt(center.x, center.y);
// 		  if (fp.intersects(box)) hit = true;
//                 }
//             }
// 	  else if (!closed_ && curved_)
// 	    {
// 	      if (mode_ == FigureKit::stroke)
// 		{
// 		  MultiLineObj ml;
// 		  ml.spline_to_multiline(*v_);
// 		  PointObj pt(center.x, center.y);
// 		  if (ml.intersects(box)) hit = true;
//                 }
// 	      else if (mode_ == FigureKit::fill_stroke ||
// 		       mode_ == FigureKit::fill)
// 		{
// 		  FillPolygonObj fp;
// 		  fp.closed_spline_to_polygon(*v_);
// 		  if (fp.intersects(box)) hit = true;
//                 }
//             }
// 	  else
// 	    {
// 	      if (mode_ == FigureKit::stroke)
// 		{
// 		  MultiLineObj ml;
// 		  ml.closed_spline_to_polygon(*v_);
// 		  if (ml.intersects(box)) hit = true;
//                 }
// 	      else if (mode_ == FigureKit::fill_stroke ||
// 		       mode_ == FigureKit::fill)
// 		{
// 		  FillPolygonObj fp;
// 		  fp.closed_spline_to_polygon(*v_);
// 		  if (fp.intersects(box)) hit = true;
//                 }
//             }
// 	  if (hit) t->hit();
// 	}
//     }
}

/*
 * Reset the figure's list of vertices
 */

void FigureImpl::resize()
{
  ext->valid = false;
  if (path->length() > 0)
    {
      ext->valid = true;
      ext->lower = path[0];
      ext->upper = path[0];
      CORBA::ULong n = path->length();
      for (CORBA::ULong i = 1; i < n; i++)
	{
	  ext->lower.x = Math::min(ext->lower.x, path[i].x);
	  ext->upper.x = Math::max(ext->upper.x, path[i].x);
	  ext->lower.y = Math::min(ext->lower.y, path[i].y);
	  ext->upper.y = Math::max(ext->upper.y, path[i].y);
        }
//       // in case of vertical/horizontal line with nil brush, 
//       // painter->is_visible will be return false, so add 1
//       if ((ext_.lower_.x == ext_.upper_.x) ||
// 	  (ext_.lower_.y == ext_.upper_.y)) 
// 	ext_.upper_.x +=1; ext_.upper_.y +=1;
    }	
}

void FigureImpl::reset()
{
  path = new Vertices;
  ext->valid = false;
}

void FigureImpl::copy (const FigureImpl &f)
{
  TransformFigure::copy(f);
  path = new Vertices(f.path);
}
