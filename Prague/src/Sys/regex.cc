/*$Id$
 *
 * This source file is a part of the Berlin Project.
 *
 * Copyright (C) 1999 Stefan Seefeld <seefelds@magellan.umontreal.ca> 
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
static char *rcsid = "$Id$";

#include "Prague/Sys/regex.hh"

/* @Method{string regex::error(int errcode)}
 *
 * @Description{convert @var{errcode} into a string}
 */
string regex::error(int errcode)
{
  if (errcode == 0) return string();
  size_t length = regerror(errcode, rx, 0, 0);
  char *buffer = new char[length];
  regerror(errcode, rx, buffer, length);
  string text = buffer;
  delete[] buffer;
  return text;
}

/* @Method{regex::regex(const string &p, int flags)}
 *
 * @Description{}
 */
regex::regex(const string &p, int flags)
{
    rx = new regex_t;
    int errcode = regcomp(rx, p.c_str(), flags);
    if (errcode) { delete rx; rx = 0;}
}

/* @Method{regex::~regex()}
 *
 * @Description{}
 */
regex::~regex()
{
  if (rx) regfree(rx);
  delete rx;
}

/*
 * @Method{rxmatch regex::search(const string &s, int i) const}
 *
 * @Description{ search in s; return position of first occurrence. If i is positive, start search from that position. If i is negative, perform reverse search from that position and return last occurrence.}
 */
rxmatch regex::search(const string &s, int i) const
{
  int n = rx->re_nsub + 1;
  regmatch_t *rm = new regmatch_t [n];
  if (i >= 0)
    while (s[i] != '\0')
      {
 	if (regexec(rx, s.c_str() + i, n, rm, i ? REG_NOTBOL : 0) == 0) return rxmatch(s, i, n, rm);
	i++;
      }
  else
    {
      i = s.length() - 1;
      do
        {
	  if (regexec(rx, s.c_str() + i, n, rm, i ? REG_NOTBOL : 0) == 0) return rxmatch(s, i, n, rm);
          i--;
        }
      while (i >= 0);
    }
  return rxmatch(s, -1, n, rm);
}

/*
 * @Method{string::size_type regex::match(const string &s, int i) const}
 *
 * @Description{return length of matched string iff this matches s at i, -1 otherwise.}
 */
string::size_type regex::match(const string &s, int i) const
{
  string substr;
  if (i < 0) i += s.length();
  if (i > (int) s.length()) return string::npos;
  regmatch_t rm;  
  int errcode = regexec(rx, s.c_str() + i, 1, &rm, 0);
  if (errcode == 0 && rm.rm_so >= 0) return rm.rm_eo - rm.rm_so;
  return string::npos;
}

const regex rxwhite("[ \n\t\r\v\f]+");
const regex rxint("-?[0-9]+");
const regex rxdouble("-?(([0-9]+\\.[0-9]*)|([0-9]+)|(\\.[0-9]+))"
		     "([eE][---+]?[0-9]+)?");
const regex rxalpha("[A-Za-z]+");
const regex rxlowercase("[a-z]+");
const regex rxuppercase("[A-Z]+");
const regex rxalphanum("[0-9A-Za-z]+");
const regex rxidentifier("[A-Za-z_$][A-Za-z0-9_$]*");