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

#include "Berlin/Console.hh"

#ifdef CONSOLE_IMPL
#  if CONSOLE_IMPL == GGI
#  include "GGI.cc"

GGIConsole *Console::t = 0;

#  elif CONSOLE_IMPL == SDL
#  include "SDL.cc"

SDLConsole *Console::t = 0;

#  else
#  warning "unknown console type defined"
#  endif
#else
#  warning "no console type defined"
#endif

Console::Reaper Console::reaper;
