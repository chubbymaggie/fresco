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
#include <Warsaw/DrawTraversal.hh>
#include <Warsaw/DrawingKit.hh>
#include <Warsaw/Subject.hh>
#include <Berlin/TransformImpl.hh>
#include <Berlin/RegionImpl.hh>
#include <Berlin/Logger.hh>
#include <Berlin/Color.hh>
#include "Tool/Diamond.hh"
#include "Tool/Beveler.hh"

using namespace Warsaw;

void InvisibleDiamond::draw(DrawTraversal_ptr traversal)
{
  Region_var allocation = traversal->current_allocation();
  Vertex u, l;
  allocation->bounds(l, u);
  DrawingKit_var drawing = traversal->drawing();

  drawing->save();
  if (drawing->surface_fillstyle() == DrawingKit::outlined)
    drawing->surface_fillstyle(DrawingKit::solid);

  Vertex center = {(u.x + l.x)/2, (u.y + l.y)/2, 0.};
  Vertex length = {(u.x - l.x), (u.y - l.y), 0.};
  Coord dx = _thickness * sqrt(length.x * length.x / (length.y * length.y) + 1.0);
  Coord dy = _thickness * sqrt(length.y * length.y / (length.x * length.x) + 1.0);

  Path path;
  path.shape = convex;
  path.nodes.length(5);
  if (_fill)
    {
      path.nodes[0].x = center.x, path.nodes[0].y = l.y, path.nodes[0].z = 0;
      path.nodes[1].x = l.x, path.nodes[1].y = center.y, path.nodes[1].z = 0;
      path.nodes[2].x = center.x, path.nodes[2].y = u.y, path.nodes[2].z = 0;
      path.nodes[3].x = u.x, path.nodes[3].y = center.y, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
    }
  else
    {
      /*
       * top left
       */
      path.nodes[0].x = center.x, path.nodes[0].y = u.y, path.nodes[0].z = 0;
      path.nodes[1].x = l.x, path.nodes[1].y = center.y, path.nodes[1].z = 0;
      path.nodes[2].x = l.x + dx, path.nodes[2].y = center.y, path.nodes[2].z = 0;
      path.nodes[3].x = center.x, path.nodes[3].y = l.y + dy, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
      /*
       * bottom left
       */
      path.nodes[0].x = l.x, path.nodes[0].y = center.y, path.nodes[0].z = 0;
      path.nodes[1].x = center.x, path.nodes[1].y = u.y, path.nodes[1].z = 0;
      path.nodes[2].x = center.x, path.nodes[2].y = u.y - dy, path.nodes[2].z = 0;
      path.nodes[3].x = l.x + dx, path.nodes[3].y = center.y, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
      /*
       * bottom right
       */
      path.nodes[0].x = center.x, path.nodes[0].y = u.y, path.nodes[0].z = 0;
      path.nodes[1].x = u.x, path.nodes[1].y = center.y, path.nodes[1].z = 0;
      path.nodes[2].x = u.x - dx, path.nodes[2].y = center.y, path.nodes[2].z = 0;
      path.nodes[3].x = center.x, path.nodes[3].y = u.y - dy, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
      /*
       * top right
       */
      path.nodes[0].x = center.x, path.nodes[0].y = l.y, path.nodes[0].z = 0;
      path.nodes[1].x = center.x, path.nodes[1].y = l.y + dy, path.nodes[1].z = 0;
      path.nodes[2].x = u.x - dx, path.nodes[2].y = center.y, path.nodes[2].z = 0;
      path.nodes[3].x = u.x, path.nodes[3].y = center.y, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
    }
  drawing->restore();
}

void BeveledDiamond::draw(DrawTraversal_ptr traversal)
{
  Region_var allocation = traversal->current_allocation();
  Vertex u, l;
  allocation->bounds(l, u);
  DrawingKit_var drawing = traversal->drawing();
  Color color = drawing->foreground();
  Color light = brightness(color,-_bright);
  Color dark  = brightness(color, _bright);

  drawing->save();
  if (drawing->surface_fillstyle() == DrawingKit::outlined)
    drawing->surface_fillstyle(DrawingKit::solid);

  switch (_style)
    {
    case inset:
      Beveler::diamond(traversal, _thickness, color, dark, light, l.x, u.x, l.y, u.y, _fill);
      break;
    case outset:
      Beveler::diamond(traversal, _thickness, color, light, dark, l.x, u.x, l.y, u.y, _fill);
      break;
    case convex:
      Beveler::diamond(traversal, _thickness/2, color, light, dark, l.x, u.x, l.y, u.y, false);
      l.x += _thickness/2, u.x -= _thickness/2, l.y += _thickness/2, u.y -= _thickness/2;
      Beveler::diamond(traversal, _thickness/2, color, dark, light, l.x, u.x, l.y, u.y, _fill);
      break;
    case concav:
      Beveler::diamond(traversal, _thickness/2, color, dark, light, l.x, u.x, l.y, u.y, false);
      l.x += _thickness/2, u.x -= _thickness/2, l.y += _thickness/2, u.y -= _thickness/2;
      Beveler::diamond(traversal, _thickness/2, color, light, dark, l.x, u.x, l.y, u.y, _fill);
      break;
    }
  drawing->restore();
}

void ColoredDiamond::draw(DrawTraversal_ptr traversal)
{
  Region_var allocation = traversal->current_allocation();
  Vertex u, l;
  allocation->bounds(l, u);
  DrawingKit_var drawing = traversal->drawing();

  drawing->save();
  if (drawing->surface_fillstyle() == DrawingKit::outlined)
    drawing->surface_fillstyle(DrawingKit::solid);
  Color tmp = drawing->foreground();
  tmp.red = _color.red;
  tmp.green = _color.green;
  tmp.blue = _color.blue;
  drawing->foreground(tmp);

  Vertex center = {(u.x + l.x)/2, (u.y + l.y)/2, 0.};
  Vertex length = {(u.x - l.x), (u.y - l.y), 0.};
  Coord dx = _thickness * sqrt(length.x * length.x / (length.y * length.y) + 1.0);
  Coord dy = _thickness * sqrt(length.y * length.y / (length.x * length.x) + 1.0);

  Path path;
  path.shape = convex;
  path.nodes.length(5);
  if (_fill)
    {
      path.nodes[0].x = center.x, path.nodes[0].y = l.y, path.nodes[0].z = 0;
      path.nodes[1].x = l.x, path.nodes[1].y = center.y, path.nodes[1].z = 0;
      path.nodes[2].x = center.x, path.nodes[2].y = u.y, path.nodes[2].z = 0;
      path.nodes[3].x = u.x, path.nodes[3].y = center.y, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
    }
  else
    {
      /*
       * top left
       */
      path.nodes[0].x = center.x, path.nodes[0].y = l.y, path.nodes[0].z = 0;
      path.nodes[1].x = l.x, path.nodes[1].y = center.y, path.nodes[1].z = 0;
      path.nodes[2].x = l.x + dx, path.nodes[2].y = center.y, path.nodes[2].z = 0;
      path.nodes[3].x = center.x, path.nodes[3].y = l.y + dy, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
      /*
       * bottom left
       */
      path.nodes[0].x = l.x, path.nodes[0].y = center.y, path.nodes[0].z = 0;
      path.nodes[1].x = center.x, path.nodes[1].y = u.y, path.nodes[1].z = 0;
      path.nodes[2].x = center.x, path.nodes[2].y = u.y - dy, path.nodes[2].z = 0;
      path.nodes[3].x = l.x + dx, path.nodes[3].y = center.y, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
      /*
       * bottom right
       */
      path.nodes[0].x = center.x, path.nodes[0].y = u.y, path.nodes[0].z = 0;
      path.nodes[1].x = u.x, path.nodes[1].y = center.y, path.nodes[1].z = 0;
      path.nodes[2].x = u.x - dx, path.nodes[2].y = center.y, path.nodes[2].z = 0;
      path.nodes[3].x = center.x, path.nodes[3].y = u.y - dy, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
      /*
       * top right
       */
      path.nodes[0].x = center.x, path.nodes[0].y = l.y, path.nodes[0].z = 0;
      path.nodes[1].x = center.x, path.nodes[1].y = l.y + dy, path.nodes[1].z = 0;
      path.nodes[2].x = u.x - dx, path.nodes[2].y = center.y, path.nodes[2].z = 0;
      path.nodes[3].x = u.x, path.nodes[3].y = center.y, path.nodes[3].z = 0;
      path.nodes[4] = path.nodes[0];
      drawing->draw_path(path);
    }
  drawing->restore();
}

