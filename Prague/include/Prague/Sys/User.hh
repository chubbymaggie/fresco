/*$Id$
 *
 * This source file is a part of the Berlin Project.
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
#ifndef _User_hh
#define _User_hh

#include <string>
#include <pwd.h>

namespace Prague
{

/* @Class{User}
 *
 * @Description{}
 */
class User
{
public:
  User(int id = -1);
  User(const string &);
  ~User() {}
  int Uid() const { return pwd->pw_uid;}
  int Gid() const { return pwd->pw_gid;}
  const char *Name() const { return pwd->pw_name;}
  const char *RealName() const { return pwd->pw_gecos;}
  const char *Home() const { return pwd->pw_dir;}
protected:
  passwd *pwd;
private:
};

};

#endif /* _User_hh */
