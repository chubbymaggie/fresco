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
#ifndef _Primitives_hh
#define _Primitives_hh

#include <Primitive/PrimitiveImpl.hh>

// class PointImpl : public virtual POA_Figure::Point,
//                   public FigureImpl
// {
// public:
//   PointImpl();
//   PointImpl(const Warsaw::Vertex &);
//   PointImpl(const PointImpl &);
//   virtual ~PointImpl();
//   virtual Warsaw::Vertex pt();
//   virtual void pt(const Warsaw::Vertex &);
// };
  
// class LineImpl : public virtual POA_Figure::Line,
//                  public FigureImpl
// {
// public:
//   LineImpl();
//   LineImpl(const Warsaw::Vertex &, const Warsaw::Vertex &);
//   LineImpl(const LineImpl &);
//   virtual ~LineImpl();
//   virtual Warsaw::Vertex pt1();
//   virtual void pt1(const Warsaw::Vertex &);
//   virtual Warsaw::Vertex pt2();
//   virtual void pt2(const Warsaw::Vertex &);
// };

class BoxImpl : public virtual POA_Primitive::Box,
		public PrimitiveImpl
{
public:
  BoxImpl();
  BoxImpl(const Warsaw::Vertex &, const Warsaw::Vertex &);
  BoxImpl(const BoxImpl &);
  virtual ~BoxImpl();
  virtual Warsaw::Vertex pt1();
  virtual void pt1(const Warsaw::Vertex &);
  virtual Warsaw::Vertex pt2();
  virtual void pt2(const Warsaw::Vertex &);
  virtual void request(Warsaw::Graphic::Requisition &);
  virtual void draw(Warsaw::DrawTraversal_ptr);
private:
  Warsaw::Vertex _lower;
  Warsaw::Vertex _upper;
};

// class CircleImpl : public virtual POA_Figure::Circle,
//                    public FigureImpl
// {
// public:
//   CircleImpl();
//   CircleImpl(const Warsaw::Vertex &, Warsaw::Coord);
//   CircleImpl(const CircleImpl &);
//   virtual ~CircleImpl();
//   virtual void resize();
//   virtual Warsaw::Vertex center();
//   virtual void center(const Warsaw::Vertex &);
//   virtual Warsaw::Coord radius();
//   virtual void radius(Warsaw::Coord);
// protected:
//   Warsaw::Vertex _center;
//   Warsaw::Coord  _radius;
// };

// class EllipseImpl : public virtual POA_Figure::Ellipse,
//                     public FigureImpl
// {
// public:
//   EllipseImpl();
//   EllipseImpl(const Warsaw::Vertex &, Warsaw::Coord, Warsaw::Coord);
//   EllipseImpl(const EllipseImpl &);
//   virtual ~EllipseImpl();
//   virtual void resize();
//   virtual Warsaw::Vertex center();
//   virtual void center(const Warsaw::Vertex &);
//   virtual Warsaw::Coord radius1();
//   virtual void radius1(Warsaw::Coord);
//   virtual Warsaw::Coord radius2();
//   virtual void radius2(Warsaw::Coord);
// protected:
//   Warsaw::Vertex _center;
//   Warsaw::Coord  _radius1;
//   Warsaw::Coord  _radius2;
// };

class GeometryImpl : public virtual POA_Primitive::Geometry,
		     public PrimitiveImpl
{
public:
  GeometryImpl(const Warsaw::Mesh &);
  GeometryImpl(const GeometryImpl &);
  virtual ~GeometryImpl();
  virtual Warsaw::Mesh *mesh();
  virtual void mesh(const Warsaw::Mesh &);
};

#endif