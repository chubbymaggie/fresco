/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999,2000 Tobias Hunger <Tobias@berlin-consortium.org>
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

/*
 * This file contains the headers of various functions used by
 * the Unicode classes.
 */

#ifndef _Babylon_utils_hh
#define _Babylon_utils_hh

#include <Babylon/defs.hh>
#include <Babylon/Char.hh>
#include <Babylon/String.hh>

namespace Babylon {

bool is_graphem_boundary(Babylon::Char, Babylon::Char);


//. This takes a single paragraph for input!
//. Returns a vector of embedding levels and the max. embedding level used
Embedding_Levels analyse(const Babylon::String::const_iterator, //.< start of input string
			 const Babylon::String::const_iterator, //.< end of input string
			 const Babylon::Base_Dir &);

//. This returns the visual ordering of a string.
//. It takes a single paragraph for input!
void log2vis(const Babylon::String::const_iterator, //.< start input string
	     const Babylon::String::const_iterator, //.< end of input string
	     const Babylon::Base_Dir &,             //.< base direction
	     std::basic_string<size_t> &,           //.< logical 2 visual reordering string
	     std::basic_string<size_t> &,           //.< visual 2 logical reordering string
	     std::basic_string<unsigned char> &,    //.< embedding level string
	     size_t);                               //.< position of start in the string
    
} // namespace Babylon

// OPERATORS:
inline std::ostream & operator << (std::ostream & out, Babylon::Char uc) { return out << uc.utf8();}
inline std::ostream & operator << (std::ostream & out, const Babylon::String & us) { return out << us.utf8();}

#endif // _Babylon_utils_hh
