/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 2000 graydon hoare <graydon@fresco.org>
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
#ifndef _Raster_hh
#define _Raster_hh

#include <Fresco/config.hh>
#include <Fresco/Raster.hh>
#include <libart_lgpl/art_misc.h>
#include <libart_lgpl/art_pixbuf.h>

namespace Berlin {
namespace DrawingKit {
namespace libArt {

struct Raster
{
  Raster(Fresco::Raster_var r);
  virtual Raster::~Raster();  
  Fresco::Raster_var remote;
  art_u8 *pixels;
public:
  ArtPixBuf *pixbuf;
};

} // namespace
} // namespace
} // namespace

#endif 
