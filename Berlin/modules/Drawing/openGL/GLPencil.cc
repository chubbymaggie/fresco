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

#include "Drawing/openGL/GLPencil.hh"

GLPencil::GLPencil(GLDrawable *d)
  : drawable(d)
{
};

Coord GLPencil::thickness() { return 0.;}
void GLPencil::thickness(Coord) {}

void GLPencil::drawLine(const Vertex &v1, const Vertex &v2)
{
  glBegin(GL_LINES);
  glVertex3d(v1.x, v1.y, v1.z);
  glVertex3d(v2.x, v2.y, v2.z);
  glEnd();
}

void GLPencil::drawPath(const Path &path)
{
  glBegin(GL_LINE_STRIP);
  for (unsigned int i = 0; i != path.length(); i++)
    {
      const Vertex &v = path[i];
      glVertex3d(v.x, v.y, v.z);
    }
  glEnd();
}

void GLPencil::fillPath(const Path &path)
{
  glBegin(GL_POLYGON);
  for (unsigned int i = 0; i != path.length(); i++)
    {
      const Vertex &v = path[i];
      glVertex3d(v.x, v.y, v.z);
    }
  glEnd();
}
