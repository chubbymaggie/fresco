/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Stefan Seefeld <seefelds@magellan.umontreal.ca> 
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

#include "Warsaw/config.hh"
#include "Warsaw/Transform.hh"
#include "Drawing/openGL/GLDrawingKit.hh"
#include "Warsaw/Text.hh"
#include "Berlin/Logger.hh"

extern "C" {
#include "ggi/ggi.h"
}

#include <GL/glu.h>
#include <strstream>
#include <iostream>

GLDrawingKit::GLDrawingKit()
  : rasters(500)
{
  ggiInit();
  drawable = new GLDrawable();
  drawable->_obj_is_ready(CORBA::BOA::getBOA());
  gnufont = new GLUnifont();
  gnufont->_obj_is_ready(CORBA::BOA::getBOA());
  Color c = {0.0,0.0,0.0,1.0};
  gnufont->setColor(c);
}

GLDrawingKit::~GLDrawingKit()
{
  drawable->_dispose();
  gnufont->_dispose();
  ggiExit();
}

void GLDrawingKit::setFont(const Text::FontDescriptor &fd, const Style::Spec &sty) 
  throw (Text::NoSuchFontException)
{
  MutexGuard guard(mutex);

  // make sure the gnufont tracks color changes
  for (unsigned long i = 0; i < sty.length(); i++) {    
      Color *tmp;
      if (sty[i].a == Style::fillcolor) {
	  sty[i].val >>= tmp;
	  //	  cerr << "set color " << tmp->red << ", " << tmp->green << ", " << tmp->blue << ", " << tmp->alpha << endl;
	  gnufont->setColor(*tmp);
      }
  }
  
  try
      { 
	  //
	  // at this time, there is _no_ way to add new fonts to the runtime.
	  // it uses GNU Unifont, and nothing else.
	  //
	  // it will eventually look like this, once we get the GLFont truetype &
	  // T1 registry alive.
	  //
	  
	  /*      GLFont *newfont = new GLFont(fd,sty); 
		  newfont->_obj_is_ready(_boa());
		  if (font) font->_dispose();
		  font = newfont;*/
      }
  catch (Text::NoSuchFontException &ex)
      {
	  throw ex;
      }
}

Text::Font_ptr GLDrawingKit::currentFont()
{
  MutexGuard guard(mutex);
  if (font) return font->_this();
  else return gnufont->_this();
}

Drawable_ptr GLDrawingKit::getDrawable()
{
  MutexGuard guard(mutex);
  return drawable->_this();
}

Pencil_ptr GLDrawingKit::getPencil(const Style::Spec &sty)
{
  MutexGuard guard(mutex);
  GLPencil *pencil = new GLPencil(sty, drawable);
  pencil->_obj_is_ready(_boa());
  pencils.push_back(pencil);
  return pencil->_this();
}

// lower.x, lower.y, upper.x, upper.y
void GLDrawingKit::clear(Coord l, Coord t, Coord r, Coord b)
{
  glColor4d(0., 0., 0., 1.);      
  //glClearColor(0.0,0.0,0.0,1.0);
  //glClear(GL_COLOR_BUFFER_BIT);

  glRectf(l, t, r, b);
}

void GLDrawingKit::image(Raster_ptr raster, Transform_ptr transform)
{
  GLRaster *glraster = rasters.lookup(Raster::_duplicate(raster));
  glraster->draw(transform);
}
