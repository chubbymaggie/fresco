/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Graydon Hoare <graydon@pobox.com> 
 * Copyright (C) 1999, 2000 Stefan Seefeld <stefan@berlin-consortium.org> 
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

#include <Command/CommandKitImpl.hh>
#include <Command/CommandImpl.hh>
#include "Command/TelltaleImpl.hh"
#include "Command/BoundedValueImpl.hh"
#include "Command/BoundedRangeImpl.hh"
#include "Command/TextBufferImpl.hh"
#include "Command/StreamBufferImpl.hh"

CommandKitImpl::CommandKitImpl(KitFactory *f, const PropertySeq &p) : KitImpl(f, p) {}
CommandKitImpl::~CommandKitImpl()
{
  for (vector<CommandImpl *>::iterator i = commands.begin(); i != commands.end(); i++) (*i)->_dispose();
}

Command_ptr CommandKitImpl::log(const char *text)
{
  LogCommand *command = new LogCommand(text);
  command->_obj_is_ready(_boa());
  commands.push_back(command);
  return command->_this();
}

MacroCommand_ptr CommandKitImpl::composite()
{
  MacroCommandImpl *command = new MacroCommandImpl();
  command->_obj_is_ready(_boa());
  commands.push_back(command);
  return command->_this();
}

TelltaleConstraint_ptr CommandKitImpl::exclusive(Telltale::Mask m)
{
  ExclusiveChoice *constraint = new ExclusiveChoice(m);
  constraint->_obj_is_ready(_boa());
//   subjects.push_back(constraint);
  return constraint->_this();
}

TelltaleConstraint_ptr CommandKitImpl::selectionRequired()
{
  SelectionRequired *constraint = new SelectionRequired;
  constraint->_obj_is_ready(_boa());
//   subjects.push_back(constraint);
  return constraint->_this();
}

Telltale_ptr CommandKitImpl::normalTelltale()
{
  TelltaleImpl *telltale = new TelltaleImpl(TelltaleConstraint::_nil());
  telltale->_obj_is_ready(_boa());
  subjects.push_back(telltale);
  return telltale->_this();
}

Telltale_ptr CommandKitImpl::constrainedTelltale(TelltaleConstraint_ptr constraint)
{
    TelltaleImpl *telltale = new TelltaleImpl(constraint);
    telltale->_obj_is_ready(_boa());
    subjects.push_back(telltale);
    constraint->add(telltale->_this());
    return telltale->_this();
}

BoundedValue_ptr CommandKitImpl::bvalue(Coord l, Coord u, Coord v, Coord s, Coord p)
{
  BoundedValueImpl *bounded = new BoundedValueImpl(l, u, v, s, p);
  bounded->_obj_is_ready(_boa());
  subjects.push_back(bounded);
  return bounded->_this();  
}

BoundedRange_ptr CommandKitImpl::brange(Coord l, Coord u, Coord lv, Coord uv, Coord s, Coord p)
{
  BoundedRangeImpl *bounded = new BoundedRangeImpl(l, u, lv, uv, s, p);
  bounded->_obj_is_ready(_boa());
  subjects.push_back(bounded);
  return bounded->_this();  
}

TextBuffer_ptr CommandKitImpl::text()
{
  TextBufferImpl *buffer = new TextBufferImpl();
  buffer->_obj_is_ready(_boa());
  subjects.push_back(buffer);
  return buffer->_this();  
}

StreamBuffer_ptr CommandKitImpl::stream(long b)
{
  StreamBufferImpl *buffer = new StreamBufferImpl(b);
  buffer->_obj_is_ready(_boa());
  subjects.push_back(buffer);
  return buffer->_this();  
}

extern "C" KitFactory *load()
{
  static string properties[] = {"implementation", "CommandKitImpl"};
  return new KitFactoryImpl<CommandKitImpl>(interface(CommandKit), properties, 1);
}
