/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Brent Fulgham <bfulgham@debian.org>
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

#include <Prague/Sys/Tracer.hh>
#include "Image/RasterImpl.hh"
#include <string>

using namespace Prague;
using namespace Warsaw;

RasterImpl::RasterImpl() : rows(0) {}
RasterImpl::RasterImpl(const char *file) : rows(0)
{
  rows = png.read(file);

  if (!rows)
    {
      string pngfile = getenv("BERLIN_ROOT");
      pngfile += "/etc/PNG/berlin-128.png";
      if (pngfile.empty())
	{
	  cerr << "Please set environment variable BERLIN_ROOT first" << endl;
	  exit(-1);
	}
     rows = png.read(pngfile);
  }
}
RasterImpl::~RasterImpl() { Trace trace("RasterImpl::~RasterImpl");}
void RasterImpl::clear()
{
  delete [] rows;
  rows = 0;
  png.clear();
}

Warsaw::Raster::Info RasterImpl::header()
{
  Warsaw::Raster::Info info;
  png.header(info);
  return info;
}

void RasterImpl::loadData(const Warsaw::Raster::Data &data)
{
  Trace trace("RasterImpl::loadData");
  clear();
  rows = png.demarshal(data);
}

void RasterImpl::storeData(Warsaw::Raster::Data_out data)
{
  Trace trace("RasterImpl::storeData");
  delete data;
  data = 0;
  data = png.marshal(rows);
}

void RasterImpl::storePixel(const Warsaw::Raster::Index &index, Color &color)
{
  Trace trace("RasterImpl::storePixel");
  color = png.pixel(index.x, index.y, rows);
}

void RasterImpl::loadPixel(const Warsaw::Raster::Index &index, const Color &color)
{
  Trace trace("RasterImpl::loadPixel");
  png.pixel(index.x, index.y, color, rows);
}

void RasterImpl::storePixels(const Warsaw::Raster::Index &lower, const Warsaw::Raster::Index &upper, Warsaw::Raster::ColorSeq_out pixels)
{
  delete pixels;
  pixels = 0;
  pixels = png.pixels(lower.x, lower.y, upper.x, upper.y, rows);
}

void RasterImpl::loadPixels(const Warsaw::Raster::Index &lower, const Warsaw::Raster::Index &upper, const Warsaw::Raster::ColorSeq &pixels)
{
  png.pixels(lower.x, lower.y, upper.x, upper.y, pixels, rows);
}

void RasterImpl::write(const char *file)
{
  png.write(file, rows);
}
