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
#include "Command/TelltaleImpl.hh"
#include "Berlin/Logger.hh"

using namespace Prague;

TelltaleImpl::TelltaleImpl(TelltaleConstraint_ptr c, unsigned long m)
  : mask(m), myConstraint(c)
{}

TelltaleImpl::~TelltaleImpl()
{}

void TelltaleImpl::set(Telltale::Mask m)
{
  SectionLog section("TelltaleImpl::set");
  if (!CORBA::is_nil(myConstraint)) myConstraint->trymodify(Telltale_var(_this()), m, true);
  else modify(m, true);
}

void TelltaleImpl::clear(Telltale::Mask m)
{
  SectionLog section("TelltaleImpl::clear");
  if (!CORBA::is_nil(myConstraint)) myConstraint->trymodify(Telltale_var(_this()), m, false);
  else modify(m, false);
}

CORBA::Boolean TelltaleImpl::test(Telltale::Mask m)
{
  MutexGuard guard(mutex);
  return (mask & m) == m;
}

void TelltaleImpl::modify(Telltale::Mask m, CORBA::Boolean on)
{
  unsigned long nf = on ? mask | m : mask & ~m;
  {
    MutexGuard guard(mutex);
    if (nf == mask) return;
    else mask = nf;
  }
  CORBA::Any any;
  any <<= nf;
  notify(any);
}

void TelltaleImpl::constraint(TelltaleConstraint_ptr c)
{
  MutexGuard guard(mutex);
  myConstraint = c;
}


TelltaleConstraint_ptr TelltaleImpl::constraint()
{
  MutexGuard guard(mutex);
  return TelltaleConstraint::_duplicate(myConstraint);
}

void TelltaleConstraintImpl::add(Telltale_ptr t)
{
  MutexGuard guard(mutex);
  telltales.push_back(Telltale::_duplicate(t));
  t->constraint(TelltaleConstraint_var(_this()));
}

void TelltaleConstraintImpl::remove(Telltale_ptr t)
{
  MutexGuard guard(mutex);
  for (vector<Telltale_var>::iterator i = telltales.begin(); i != telltales.end(); i++)
    if ((*i) == t)
      {
	telltales.erase(i);
	break;
      }
}

ExclusiveChoice::ExclusiveChoice(Telltale::Mask m)
  : mask(m)
{}

void ExclusiveChoice::trymodify(Telltale_ptr t, Telltale::Mask m, CORBA::Boolean b)
{
  MutexGuard guard(mutex);
  if (b)
    for (tlist_t::iterator i = telltales.begin(); i != telltales.end(); i++)
      if ((*i)->test(m)) (*i)->modify(m, false);
  t->modify(m, b);
}

SelectionRequired::SelectionRequired()
{}

void SelectionRequired::trymodify(Telltale_ptr t, Telltale::Mask m, CORBA::Boolean b)
{
}