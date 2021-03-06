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
#ifndef _ToolKit_Frame_hh
#define _ToolKit_Frame_hh

#include <Fresco/config.hh>
#include <Fresco/Telltale.hh>
#include <Fresco/Region.hh>
#include <Berlin/ViewImpl.hh>
#include <Berlin/MonoGraphic.hh>
#include <Berlin/ImplVar.hh>
#include <Berlin/RefCountVar.hh>

namespace Berlin
{
  namespace ToolKit
  {

    class Frame : public MonoGraphic
    {
      public:
    class Renderer
    {
      public:
        Renderer(Fresco::Coord t, bool f) :
        thickness(t), fill(f)
        { }
        virtual void draw(Fresco::DrawTraversal_ptr) = 0;
        Fresco::Coord thickness;
        bool          fill;
    };  

    Frame(Fresco::Coord, Renderer *);
    virtual ~Frame();
    virtual void request(Fresco::Graphic::Requisition &);
    virtual void traverse(Fresco::Traversal_ptr);
    virtual void extension(const Fresco::Allocation::Info &,
                   Fresco::Region_ptr);
    virtual void allocate(Fresco::Tag,
                  const Fresco::Allocation::Info &);
    
    virtual void draw(Fresco::DrawTraversal_ptr);
    virtual const char *object_name() { return "Frame"; }
      protected:
    void allocate_span(const Fresco::Graphic::Requirement &,
               Fresco::Region::Allotment &,
               Fresco::Coord, Fresco::Alignment);
    Fresco::Coord        my_thickness;
    Impl_var<RegionImpl> my_allocation;
    Renderer            *my_renderer;
    };

    class InvisibleFrame : public Frame::Renderer
    {
      public:
    InvisibleFrame(Fresco::Coord t, bool f) :
        Frame::Renderer(t, f)
    { }
    virtual void draw(Fresco::DrawTraversal_ptr);
    };

    class Bevel : public Frame::Renderer
    {
      public:
    enum type { inset, outset, convex, concav };
    Bevel(Fresco::Coord t, type s, Fresco::Coord b, bool f) :
        Frame::Renderer(t, f), my_style(s), my_bright(b)
    { }
    virtual void draw(Fresco::DrawTraversal_ptr);
      protected:
    type          my_style;
    Fresco::Coord my_bright;
    };

    class ColoredFrame : public Frame::Renderer
    {
      public:
    ColoredFrame(Fresco::Coord t, const Fresco::Color &c, bool f) :
        Frame::Renderer(t, f), my_color(c) { }
    virtual void draw(Fresco::DrawTraversal_ptr);
      protected:
    Fresco::Color my_color;
    };

  } // namespace
} // namespace

#endif
