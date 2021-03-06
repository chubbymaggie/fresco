/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 2003 Nick Lewycky <nicholas@fresco.org>
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
#ifndef _Fonts_Glyph_hh
#define _Fonts_Glyph_hh

#include <Fresco/config.hh>
#include <Fresco/Font.hh>
#include <Fresco/Transform.hh>
#include <Fresco/Unicode.hh>
#include <Berlin/RefCountBaseImpl.hh>
#include <Berlin/IdentifiableImpl.hh>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TRUETYPE_TABLES_H
#include FT_OUTLINE_H

class Fresco::Raster;

namespace Berlin
{
namespace FontKit
{

  int moveto(FT_Vector *, void*);
  int lineto(FT_Vector *, void*);
  int conicto(FT_Vector *, FT_Vector *, void*);
  int cubicto(FT_Vector *, FT_Vector *, FT_Vector *, void*);

class GlyphImpl : public virtual POA_Fresco::Glyph,
                  public virtual RefCountBaseImpl,
                  public virtual IdentifiableImpl
{
public:
  GlyphImpl(const FT_Face face, const CORBA::ULong size,
            const Fresco::Unichar char_index);
  virtual ~GlyphImpl();

  virtual Fresco::Raster_ptr bitmap(CORBA::ULong xdpi, CORBA::ULong ydpi);
  virtual Fresco::FontShape *decompose();
  virtual void char_info(Fresco::GlyphMetrics &gm);
  virtual void transformation(Fresco::Transform_ptr);
private:
  FT_Face my_face;
  FT_Matrix my_tr;
  CORBA::ULong my_size;
  Fresco::Unichar my_uc;
};

} // namespace
} // namespace

#endif // header guard
