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
#include "Berlin/Memory.hh"
#include "Drawing/openGL/Pointer.hh"
extern "C"
{
#include "ggi/ggi.h"
}
#include <iostream>
#include <algorithm>


static unsigned char pointerImg[256] = 

{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,
  1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,
  1,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,
  1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,
  1,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,
  1,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,
  1,2,2,2,2,2,1,1,1,1,0,0,0,0,0,0,
  1,2,2,1,2,2,1,0,0,0,0,0,0,0,0,0,
  1,1,0,1,2,2,2,1,0,0,0,0,0,0,0,0,
  1,0,0,0,1,2,2,1,0,0,0,0,0,0,0,0,
  0,0,0,0,1,2,2,2,1,0,0,0,0,0,0,0,
  0,0,0,0,0,1,2,2,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0 };

  
Pointer::Pointer(ggi_visual_t visual)
{
  origin[0] = origin[1] = 0;
  position[0] = position[1] = 8;
  size[0] = size[1] = 16;

  if (!(dbuf = ggiDBGetBuffer (visual, 0)) )
    cerr << "Error getting display buffer" << endl;
  else if (dbuf->layout != blPixelLinearBuffer)
    cerr << "Error: nonlinear display buffer" << endl;
  else if (! (dbuf->type & GGI_DB_SIMPLE_PLB))
    cerr << "Error: non-standard display buffer" << endl;
  depth = dbuf->buffer.plb.pixelformat->size/8;
  stride = dbuf->buffer.plb.stride;

  /*
   * create the pointer image
   */
  image = new unsigned char[size[0]*size[1]*depth];
  for (unsigned short y = 0; y != size[1]; y++)
    for (unsigned short x = 0; x != size[0]; x++)
	for (unsigned short d = 0; d != depth; d++)
	    image[y*depth*size[0] + depth*x + d] = pointerImg[y*size[0] +x] * 127;
  
  /*
   * create the pointer mask
   */
  mask = new unsigned char[size[0]*size[1]*depth];
  for (unsigned short y = 0; y != size[1]; y++)
      for (unsigned short x = 0; x != size[0]; x++)
	  for (unsigned short d = 0; d != depth; d++)
	      mask[y*depth*size[0] + depth*x + d] = pointerImg[y*size[0] +x] > 0 ? ~0 : 0;

  backup = new unsigned char[size[0]*size[1]*depth];
  save();
}

Pointer::~Pointer()
{
  delete [] image;
  delete [] mask;
  delete [] backup;
}

void Pointer::move(PixelCoord x, PixelCoord y)
{
  restore();
  position[0] = max(x, origin[0]);
  position[1] = max(y, origin[1]);
  save();
  write();
};

void Pointer::save()
{
  unsigned char *from = static_cast<unsigned char *>(dbuf->read) + (position[1]-origin[1])*stride + (position[0]-origin[0])*depth;
  unsigned char *to = backup;
  for (PixelCoord y = 0; y != size[1]; y++, from += stride, to += depth*size[0])
    Memory::copy(from, to, depth*size[0]);
}

void Pointer::restore()
{
  unsigned char *from = backup;
  unsigned char *to = static_cast<unsigned char *>(dbuf->write) + (position[1]-origin[1])*stride + (position[0]-origin[0])*depth;
  for (PixelCoord y = 0; y != size[1]; y++, from += depth*size[0], to += stride)
    Memory::copy(from, to, depth*size[0]);
}

void Pointer::write()
{
  unsigned char *from = image;
  unsigned char *bits = mask;
  unsigned char *to = static_cast<unsigned char *>(dbuf->write) + (position[1]-origin[1])*stride + (position[0]-origin[0])*depth;

  for (PixelCoord y = 0; y != size[1]; y++, to += stride - size[0]*depth)
    for (PixelCoord x = 0; x != size[0]*depth; x++, from++, bits++, to++)
	*to = (*from & *bits) | (*to & ~*bits);
	    
	    //	    *to = *from & *bits;
}
