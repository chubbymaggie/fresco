//
// $Id$
//
// This source file is a part of the Berlin Project.
// Copyright (C) 1998 Graydon Hoare <graydon@pobox.com> 
// http://www.berlin-consortium.org
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//
#ifndef _TextViewer_hh
#define _TextViewer_hh

#include "Warsaw/config.hh"
#include "Warsaw/View.hh"
#include "Warsaw/TextKit.hh"
#include "Berlin/GapBuffer.hh"
#include "Layout/Box.hh"
#include <map>

class TextChunk;
class FontChange;
class Compositor;

declare_corba_ptr_type(DrawingKit)
declare_corba_ptr_type(TextBuffer)
declare_corba_ptr_type(DrawTraversal)

  //!!!FIXME!!! at the moment we are using a non-compensated
  // hbox to layout text. this needs to be improved quite a bit.
  // i.e. at the moment it totally ignores the compositor and canonical
  // DK passed into it, and just uses a LayoutKit LayoutManager
  // by inheritence.

class TextViewer : implements(View), public virtual HBox {

    public:

    TextViewer(TextBuffer_ptr txt, TextKit_ptr tk, DrawingKit_ptr dk, Compositor *);
    void update(Subject_ptr s, const CORBA::Any &a);
    virtual ~TextViewer();

    protected:

    TextBuffer_var myTextBuffer;
    TextKit_var myTextKit;
    DrawingKit_var myCanonicalDK;
    Compositor  *myCompositor;

};

#endif
