/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 2000 Stefan Seefeld <stefan@berlin-consortium.org> 
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
#include <Warsaw/Input.hh>
#include <Prague/Unicode/Unicode.hh>
#include <Warsaw/Unicode.hh>
#include <Prague/Sys/Tracer.hh>
#include "Tool/Terminal.hh"

using namespace Prague;

void Terminal::keyPress(const Input::Event &event)
{
  Trace trace("Terminal::keyPress");
  StreamBuffer::Data data;
  data.length(1);
  const Input::Toggle &toggle = event[0].attr.kselection();
  Unicode::Char uc(static_cast<Unicode::_Char>(toggle.number));
  char ascii = uc.myUnicode();
  if (isprint(ascii) || ascii == '\r' || ascii == '\n') data[0] = ascii;
  else if(toggle.number == Unicode::UC_HORIZONTAL_TABULATION) data[0] = '\t';
  buffer->write(data);
}