/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999 Stefan Seefeld <stefan@fresco.org> 
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
#ifndef _ToolKit_Beveler_hh
#define _ToolKit_Beveler_hh

#include <Fresco/config.hh>
#include <Fresco/DrawTraversal.hh>

namespace Berlin
{
  namespace ToolKit
  {

    namespace Beveler 
    {

      void rect(Fresco::DrawTraversal_ptr, Fresco::Coord,
		const Fresco::Color &, const Fresco::Color &,
		const Fresco::Color&, Fresco::Coord, Fresco::Coord,
		Fresco::Coord, Fresco::Coord, bool);
      void leftArrow(Fresco::DrawTraversal_ptr, Fresco::Coord,
		     const Fresco::Color &, const Fresco::Color &,
		     const Fresco::Color&, Fresco::Coord, Fresco::Coord,
		     Fresco::Coord, Fresco::Coord, bool);
      void rightArrow(Fresco::DrawTraversal_ptr, Fresco::Coord,
		      const Fresco::Color &, const Fresco::Color &,
		      const Fresco::Color&, Fresco::Coord, Fresco::Coord,
		      Fresco::Coord, Fresco::Coord, bool);
      void upArrow(Fresco::DrawTraversal_ptr, Fresco::Coord,
		   const Fresco::Color &, const Fresco::Color &,
		   const Fresco::Color&, Fresco::Coord, Fresco::Coord,
		   Fresco::Coord, Fresco::Coord, bool);
      void downArrow(Fresco::DrawTraversal_ptr, Fresco::Coord,
		     const Fresco::Color &, const Fresco::Color &,
		     const Fresco::Color&, Fresco::Coord, Fresco::Coord,
		     Fresco::Coord, Fresco::Coord, bool);
      void diamond(Fresco::DrawTraversal_ptr, Fresco::Coord,
		   const Fresco::Color &, const Fresco::Color &,
		   const Fresco::Color&, Fresco::Coord, Fresco::Coord,
		   Fresco::Coord, Fresco::Coord, bool);
      void circle(Fresco::DrawTraversal_ptr, Fresco::Coord,
		  const Fresco::Color &, const Fresco::Color &,
		  const Fresco::Color&, Fresco::Coord, Fresco::Coord,
		  Fresco::Coord, Fresco::Coord, bool);
    } // namespace
    
  } // namespace
} // namespace

#endif
