/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Graydon Hoare <graydon@pobox.com> 
 * Copyright (C) 1999 Stefan Seefeld <stefan@berlin-consortium.org> 
 * Copyright (C) 2000 Nathaniel Smith <njs@berlin-consortium.org>
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
#ifndef _TextViewer_hh
#define _TextViewer_hh

#include <Warsaw/config.hh>
#include <Warsaw/View.hh>
#include <Warsaw/TextKit.hh>
#include "Text/Composition.hh"
#include <map>

class TextViewer : public virtual POA_View, public Composition
{
 public:
  TextViewer(TextBuffer_ptr txt, TextKit_ptr tk, DrawingKit_ptr dk, Compositor *);
  void init(); // FIXME: collapse into constructor when we go to POA
  virtual ~TextViewer();
  virtual void update(const CORBA::Any &);
 protected:
  TextKit_var kit;
  TextBuffer_var buffer;
};

#endif
