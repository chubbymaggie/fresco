/*+P
 * This file is part of OffiX,
 * a C++ API for the X Window System and Unix
 * Copyright (C) 1995-98  Stefan Seefeld
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
 -P*/
/* $Id$ */
#ifndef _bzbuf_h
#define _bzbuf_h

#include <streambuf.h>
#include <stdio.h>
extern "C" 
{
#include <bzlib.h>
}

/* @Class {bzbuf : public streambuf}
 *
 * @Description {filter using bzip2 library}
 */
class bzbuf : public streambuf, private bz_stream
{
public:
  typedef char          char_type;
  typedef streampos     pos_type;
  typedef streamoff     off_type;
  typedef int           int_type;
  bzbuf(streambuf *, int);
  virtual ~bzbuf();
  virtual int sync();
protected:
  virtual int_type underflow();
  virtual int_type uflow();
  virtual int_type overflow(int = EOF);
  char_type  *cbase() const { return comp;}
  char_type  *ecptr() const { return comp + BUFSIZ;}
  char_type  *cin() const { return next_in + avail_in;}
  char_type  *cout() const { return pout;}
private:
  char_type *comp;
  char_type *pout;
  streambuf *back;
};

#endif /* _bzbuf_h */