/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Graydon Hoare <graydon@pobox.com> 
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
#include <Warsaw/config.hh>
#include <Warsaw/Unicode.hh>
#include <Warsaw/StreamBuffer.hh>
#include <Warsaw/DrawingKit.hh>
#include "Text/TerminalView.hh"
#include <Prague/Sys/Tracer.hh>
#include <Prague/Unicode/Unicode.hh>
#include <algorithm>

using namespace Prague;

TerminalView::TerminalView(StreamBuffer_ptr s, TextKit_ptr tk, DrawingKit_ptr dk, Compositor *l, Compositor *p)
  : Composition(dk, p),
    stream(StreamBuffer::_duplicate(s)),
    kit(TextKit::_duplicate(tk)),
    canonicalDK(DrawingKit::_duplicate(dk)),
    compositor(l),
    locked(false)
{
  Trace trace("TerminalView::TerminalView");
}

TerminalView::~TerminalView() {}
void TerminalView::request(Requisition &r)
{
  r.x.defined = true;
  r.x.minimum = r.x.natural = r.x.maximum = 4000.;
  r.x.align   = 0.;
  r.y.defined = true;
  r.y.minimum = r.y.natural = r.y.maximum = 10000.;
  r.y.align   = 0.;
}

void TerminalView::needResize()
{
  if (!locked) Composition::needResize();
}

void TerminalView::update(const CORBA::Any &)
{
  Trace trace("TerminalView::update");  
  {
//     MutexGuard guard(childMutex);
    begin();
    if (!lines.size())
      {
	lines.push_back(activate(new Composition(canonicalDK, compositor)));
	append(Graphic_var(lines.back()->_this()));
      }
    StreamBuffer::Data_var data = stream->read();
    char *begin = (char *)data->get_buffer();
    char *end   = begin + data->length();
    for (char *i = begin; i != end; i++)
      {
	if (isprint(*i))
	  {
	    Unicode::Char uc(*i);
	    Unistring us;
	    us.length(1);
	    us[0] = Unicode::toCORBA(uc);
	    Graphic_var child = kit->chunk(us);
	    lines.back()->append(child);
	  }
	else switch(*i)
	  {
	  case '\r':
	  case '\n':
	    lines.push_back(activate(new Composition(canonicalDK, compositor)));
	    lines.back()->append(Graphic_var(kit->strut()));
	    append(Graphic_var(lines.back()->_this()));
	    break;
	  case '\b':
	    break;
	  }
      }
    this->end();
  }
  needResize();
  needRedraw();
}

void TerminalView::begin()
{
  locked = true;
}

void TerminalView::end()
{
  locked = false;
}
