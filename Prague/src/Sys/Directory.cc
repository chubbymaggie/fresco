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

#include <Prague/Sys/Directory.hh>
#include <Prague/Sys/regex.hh>
#include <dirent.h>

using namespace Prague;

bool compSize(File *a, File *b) { return  (a->Size() > b->Size());}
bool compAccTime(File *a, File *b) { return a->AccTime() < b->AccTime();}
bool compModTime(File *a, File *b) { return a->ModTime() < b->ModTime();}
bool compAlpha(File *a, File *b) { return a->Name() < b->Name();}
bool compDirsFirst(File *a, File *b)
{
  if (a->is(File::dir) && !b->is(File::dir)) return true;
  else if (!a->is(File::dir) && b->is(File::dir)) return false;
  else return compAlpha(a, b);
};

/* @Method{Directory::Directory(const string &name, int order = 0, int filter = unfiltered)}
 *
 * @Description{list all files according to @var{filter} in the order given by @var{order}}
 */
Directory::Directory(const string &n, int order, int filter)
  : File(n)
{
  while (longname.length() > 2 && longname[longname.length() - 1] == '.' && longname[longname.length() - 2] == '.')
    {
      string::size_type i = longname.rfind('/', longname.length() - 3);
      longname.erase(i, longname.length());
    }
  shortname = BaseName(longname);
  if (getStatus() && is(File::dir))
    {
      DIR *dir = opendir(longname.c_str());
      for (struct dirent *entry = readdir(dir); entry; entry = readdir(dir))
	{
	  string childname = longname + '/' + entry->d_name;
	  if (filter == unfiltered ||
	      filter == nohidden && entry->d_name[0] != '.' ||
	      filter == dirs && File(childname).is(File::dir) ||
	      filter == nodirs && !File(childname).is(File::dir))
	    children.push_back(new File(childname));
	}
      closedir(dir);
      switch (order)
	{
	case dirsfirst: sort(children.begin(), children.end(), compDirsFirst); break;
	case size:      sort(children.begin(), children.end(), compSize); break;
	case modtime:   sort(children.begin(), children.end(), compModTime); break;
	case acctime:   sort(children.begin(), children.end(), compAccTime); break;
	case alpha:
	default:        sort(children.begin(), children.end(), compAlpha); break;
	}
    }
}

/* @Method{Directory::Directory(const string &name, int order = 0, const string &pattern)}
 *
 * @Description{list all files according to @var{filter} in the order given by @var{order}}
 */
Directory::Directory(const string &n, int order, const string &pattern)
  : File(n)
{
  while (longname.length() > 2 && longname[longname.length() - 1] == '.' && longname[longname.length() - 2] == '.')
    {
      string::size_type i = longname.rfind('/', longname.length() - 3);
      longname.erase(i, longname.length());
    }
  shortname = BaseName(n);
  if (getStatus() && is(File::dir))
    {
      regex filter(pattern);
      DIR *dir = opendir(longname.c_str());
      for (struct dirent *entry = readdir(dir); entry; entry = readdir(dir))
	{
	  if (filter.search(entry->d_name))
	    {
	      string childname = longname + '/' + entry->d_name;
	      children.push_back(new File(childname));
	    }
	}
      closedir(dir);
      switch (order)
	{
	case dirsfirst: sort(children.begin(), children.end(), compDirsFirst); break;
	case size:      sort(children.begin(), children.end(), compSize); break;
	case modtime:   sort(children.begin(), children.end(), compModTime); break;
	case acctime:   sort(children.begin(), children.end(), compAccTime); break;
	case alpha:
	default:        sort(children.begin(), children.end(), compAlpha); break;
	}
    }
}

/* @Method{Directory::Directory(const Directory &D)}
 *
 * @Description{copy constructor}
 */
Directory::Directory(const Directory &D)
  : File(D)
{
  for (vector<File *>::const_iterator i = D.children.begin(); i != D.children.end(); i++)
    children.push_back(new File((*i)->Name()));
}

/* @Method{Directory::~Directory()}
 *
 * @Description{}
 */
Directory::~Directory()
{
  for (vector<File *>::iterator i = children.begin(); i != children.end(); i++) delete *i;
}

