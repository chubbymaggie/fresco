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

#ifndef _Berlin_Console_SHMDrawableFactory_hh
#define _Berlin_Console_SHMDrawableFactory_hh

#include <Berlin/config.hh>
#include <Berlin/Console.hh>

namespace Berlin
{
  namespace Console_Extension
  {

    class SHMDrawableFactory : virtual public Console::Extension
    {
      public:
    //. Creates a new Drawable of the given size (x, y) and depth. It
    //. is accessable under the given shm-id.
    virtual Console::Drawable *create_drawable(int shmid,
                           Fresco::PixelCoord,
                           Fresco::PixelCoord,
                           Fresco::PixelCoord) = 0;
    };

  } // namespace
} // namespace

#endif
