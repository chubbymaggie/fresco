/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1998 Graydon Hoare <graydon@pobox.com> 
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
#ifndef _ServerContextManagerImpl_hh
#define _ServerContextManagerImpl_hh

#include "Warsaw/config.hh"
#include "Warsaw/ServerContext.hh"
#include "Warsaw/Graphic.hh"
#include "Berlin/KitImpl.hh"
#include "Prague/Sys/Thread.hh"
#include "Prague/Sys/Plugin.hh"
#include <vector>
#include <string>
#include <multimap.h>

class ServerContextManagerImpl : implements(ServerContextManager)
//. the ServerContextManager just hands out new ServerContexts to
//. people who are connecting.  it might want to do some checking on
//. the incoming ClientContext's credentials, but at the moment it doesn't.
{
  typedef vector<ServerContextImpl *> clist_t;
  typedef multimap<string, Prague::Plugin<KitFactory> *> flist_t;
public:
  ServerContextManagerImpl();
  ServerContext_ptr newServerContext(ClientContext_ptr c) throw (SecurityException);
  void setSingleton(const char *, CORBA::Object_ptr)
    throw (SecurityException, SingletonFailureException);
  void delSingleton(const char *)
    throw (SecurityException, SingletonFailureException);
  CORBA::Object_ptr getSingleton(const char *) 
    throw (SecurityException, SingletonFailureException);
  void ping();
  // this builds the plugin table
  void scan(const string &);
  void clear();
  void start();
protected:
  static void *run(void *);
  map<string, CORBA::Object_var> singletons;
  clist_t contexts;
  flist_t factories;
  Prague::Thread thread;
  Prague::Mutex mutex;
};

#endif
