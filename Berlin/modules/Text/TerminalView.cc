/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999 Graydon Hoare <graydon@fresco.org> 
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
#include <Prague/Sys/Tracer.hh>
#include <Fresco/config.hh>
#include <Fresco/Unicode.hh>
#include <Fresco/StreamBuffer.hh>
#include <Fresco/DrawingKit.hh>
#include "TerminalView.hh"

using namespace Prague;
using namespace Fresco;

using namespace Berlin::TextKit;

TerminalView::TerminalView(StreamBuffer_ptr s, TextKit_ptr tk,
                       DrawingKit_ptr dk, Compositor *l,
               Compositor *p) :
    Composition(dk, p),
    my_stream(StreamBuffer::_duplicate(s)),
    my_kit(Fresco::TextKit::_duplicate(tk)),
    my_canonicalDK(DrawingKit::_duplicate(dk)),
    my_compositor(l),
    my_locked(false)
{ Trace trace("TerminalView::TerminalView"); }

TerminalView::~TerminalView() { }
void TerminalView::request(Fresco::Graphic::Requisition &r)
{
    r.x.defined = true;
    r.x.minimum = r.x.natural = r.x.maximum = 4000.;
    r.x.align   = 0.;
    r.y.defined = true;
    r.y.minimum = r.y.natural = r.y.maximum = 10000.;
    r.y.align   = 0.;
}

void TerminalView::need_resize()
{ if (!my_locked) Composition::need_resize(); }

void TerminalView::update(const CORBA::Any &)
{
    Trace trace("TerminalView::update");  
    {
//       Prague::Guard<Mutex> guard(childMutex);
    begin();
    if (!my_lines.size())
    {
        Composition *composition =
        new Composition(my_canonicalDK, my_compositor);
        activate(composition);
        my_lines.push_back(composition);
        append_graphic(Graphic_var(my_lines.back()->_this()));
    }
    StreamBuffer::Data_var data = my_stream->read();
    char *begin = (char *)data->get_buffer();
    char *end   = begin + data->length();
    for (char *i = begin; i != end; i++)
    {
        if (isprint(*i))
        {
        Babylon::Char uc(*i);
        Unistring us;
        us.length(1);
        us[0] = Unicode::to_CORBA(uc);
        Graphic_var child = my_kit->chunk(us);
        my_lines.back()->append_graphic(child);
        }
        else switch(*i)
        {
        case '\r':
        case '\n':
        {
        Composition *composition =
            new Composition(my_canonicalDK, my_compositor);
        activate(composition);
        my_lines.push_back(composition);
        my_lines.back()->
            append_graphic(Graphic_var(my_kit->strut()));
        append_graphic(Graphic_var(my_lines.back()->_this()));
        }
        break;
        case '\b':
        break;
        }
    }
    this->end();
    }
    need_resize();
    need_redraw();
}

void TerminalView::begin() { my_locked = true;}

void TerminalView::end() { my_locked = false;}
