/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Brent Fulgham <bfulgham@debian.org>
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

#include "Berlin/Logger.hh"
#include "Image/RasterImpl.hh"

RasterImpl::RasterImpl() : rows(0) {}
RasterImpl::RasterImpl(const char *file) : rows(0)
{
  rows = png.read(file);
  if (!rows) rows = png.read("../etc/PNG/berlin-128.png");
}
RasterImpl::~RasterImpl() {}

void RasterImpl::clear()
{
  delete [] rows;
  rows = 0;
  png.clear();
}

Raster::Info RasterImpl::header()
{
  Info info;
  png.header(info);
  return info;
}

void RasterImpl::loadData(const Raster::Data &data)
{
  SectionLog section(Logger::image, "RasterImpl::loadData");
  clear();
  rows = png.demarshal(data);
}

void RasterImpl::storeData(Raster::Data *&data)
{
  SectionLog section(Logger::image, "RasterImpl::storeData");
  delete data;
  data = 0;
  data = png.marshal(rows);
}

void RasterImpl::storePixel(const Index &index, Color &color)
{
  SectionLog section(Logger::image, "RasterImpl::storePixel");
  color = png.pixel(index.x, index.y, rows);
}

void RasterImpl::loadPixel(const Index &index, const Color &color)
{
  SectionLog section(Logger::image, "RasterImpl::loadPixel");
  png.pixel(index.x, index.y, color, rows);
}

void RasterImpl::storePixels(const Index &lower, const Index &upper, Raster::ColorSeq *&pixels)
{
  delete pixels;
  pixels = 0;
  pixels = png.pixels(lower.x, lower.y, upper.x, upper.y, rows);
}

void RasterImpl::loadPixels(const Index &lower, const Index &upper, const Raster::ColorSeq &pixels)
{
  png.pixels(lower.x, lower.y, upper.x, upper.y, pixels, rows);
}

void RasterImpl::write(const char *file)
{
  png.write(file, rows);
}
