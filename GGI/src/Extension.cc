/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 2000 Stefan Seefeld <stefan@fresco.org> 
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

#include <Berlin/Logger.hh>
#include "Extension.hh"
#include "Drawable.hh"
#include <sstream>
extern "C"
{
#include <ggi/ggi-unix.h>
}

void GGI::Extension::attach(Berlin::Console::Drawable *drawable)
{ my_drawable = dynamic_cast<Drawable *>(drawable); }

void GGI::Renderer::set_color(const Fresco::Color &c)
{ ggiSetGCForeground(drawable()->visual(), drawable()->map(c)); }

void GGI::Renderer::draw_pixel(Fresco::PixelCoord x, Fresco::PixelCoord y)
{ ggiDrawPixel(drawable()->visual(), x, y); }

void GGI::Renderer::draw_hline(Fresco::PixelCoord x, Fresco::PixelCoord y,
                   Fresco::PixelCoord w)
{ ggiDrawHLine(drawable()->visual(), x, y, w); }

void GGI::Renderer::draw_vline(Fresco::PixelCoord x, Fresco::PixelCoord y,
                   Fresco::PixelCoord h)
{ ggiDrawVLine(drawable()->visual(), x, y, h); }

void GGI::Renderer::draw_line(Fresco::PixelCoord x, Fresco::PixelCoord y,
                  Fresco::PixelCoord w, Fresco::PixelCoord h)
{ ggiDrawLine(drawable()->visual(), x, y, x + w, y + h); }

void GGI::Renderer::draw_box(Fresco::PixelCoord x, Fresco::PixelCoord y,
                 Fresco::PixelCoord w, Fresco::PixelCoord h)
{ ggiDrawBox(drawable()->visual(), x, y, w, h); }

GGI::DirectBuffer::Guard GGI::DirectBuffer::read_buffer()
{
    Guard guard(drawable(),
        static_cast<Berlin::Console::Drawable::data_type *>
        (ggiDBGetBuffer(drawable()->visual(), 0)->read));
    return guard;
}

GGI::DirectBuffer::Guard GGI::DirectBuffer::write_buffer()
{
    Guard guard(drawable(),
        static_cast<Berlin::Console::Drawable::data_type *>
        (ggiDBGetBuffer(drawable()->visual(), 0)->write));
    return guard;
}

Berlin::Console::Drawable *
GGI::SHMDrawableFactory::create_drawable(int shmid,
                     Fresco::PixelCoord w,
                     Fresco::PixelCoord h,
                     Fresco::PixelCoord d)
{
    std::ostringstream oss;
    oss << "display-memory:shmid:" << shmid << std::ends;
    GGI::Drawable *drawable;
    try
    {
    drawable = new GGI::Drawable(oss.str().c_str(), w, h, d);
    }
    catch (...)
    {
    Berlin::Logger::log(Berlin::Logger::console)
        << "Error : can't open shm GGI::Drawable" << std::endl;
    throw;
    }
    Berlin::Logger::log(Berlin::Logger::console)
    << "open ggi display with name :'" << oss.str() << '\''
    << std::endl;
  
    static_cast<GGI::Console *>
    (Berlin::Console::instance())->add_drawable(drawable);
    return drawable;
}

Berlin::Console_Extension::GGIDrawable *
GGI::GGIDrawableFactory::create_drawable(int shmid,
                     Fresco::PixelCoord w,
                     Fresco::PixelCoord h,
                     Fresco::PixelCoord d)
{
    std::ostringstream oss;
    oss << "display-memory:-input:shmid:" << shmid << std::ends;
    GGI::Drawable *drawable;
    try
    {
    drawable = new GGI::Drawable(oss.str().c_str(), w, h, d);
    }
    catch (...)
    {
    Berlin::Logger::log(Berlin::Logger::console)
        << "Error : can't open shm GGI::Drawable" << std::endl;
    throw;
    }
    Berlin::Logger::log(Berlin::Logger::console)
    << "open ggi display with name :'" << oss.str() << '\''
    << std::endl;
  
    static_cast<GGI::Console *>
    (Berlin::Console::instance())->add_drawable(drawable);
    return drawable;
}
