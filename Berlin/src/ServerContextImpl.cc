/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1998 Graydon Hoare <graydon@fresco.org> 
 * Copyright (C) 2000 Stefan Seefeld <stefan@fresco.org>
 * http://www.fresco.org
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

#include <Prague/Sys/Plugin.hh>
#include <Prague/Sys/Directory.hh>
#include <Prague/Sys/Tracer.hh>
#include <Fresco/config.hh>
#include <Fresco/ClientContext.hh>
#include <Berlin/Logger.hh>
#include <Berlin/KitImpl.hh>
#include <Fresco/Unicode.hh>
#include "ServerContextImpl.hh"
#include "ServerImpl.hh"
#include <sstream>

using namespace Prague;
using namespace Fresco;

unsigned long ServerContextImpl::_counter = 0;

// this is the thing which holds all the references to things a client allocates
// (for garbage collection), does security checks for new allocation, and
// occasionally pings the client to make sure it's still there.

ServerContextImpl::ServerContextImpl(ServerImpl *s, const CORBA::PolicyList &policies, ClientContext_ptr c)
    : _server(s), _policies(policies), _client(ClientContext::_duplicate(c))
{
  Trace trace("ServerContextImpl::ServerContextImpl");
  Babylon::String title = Unicode::to_internal(*_client->application_title());
  Logger::log(Logger::corba) << "ServerContext for application \""
			     << title.utf8() << "\" created."
			     << std::endl;
}

ServerContextImpl::~ServerContextImpl()
{
  Trace trace("ServerContextImpl::~ServerContextImpl");
  for (klist_t::iterator i = _kits.begin(); i != _kits.end(); ++i)
    (*i).second->decrement();
  Babylon::String title = Unicode::to_internal(*_client->application_title());
  Logger::log(Logger::corba) << "ServerContext for application \""
			     << title.utf8() << "\" destructed."
			     << std::endl;
}

ClientContext_ptr ServerContextImpl::client() { return ClientContext::_duplicate(_client);}

// will fill the rest of these in as needs arise. They should do something reasonably obvious.

void ServerContextImpl::set_singleton(const char *name, CORBA::Object_ptr singleton)
  throw (SecurityException, SingletonFailureException)
{
  _server->set_singleton(name, singleton);
}

void ServerContextImpl::remove_singleton(const char *name)
  throw (SecurityException, SingletonFailureException)
{
  _server->remove_singleton(name);
}

CORBA::Object_ptr ServerContextImpl::get_singleton(const char *name) 
  throw (SecurityException, SingletonFailureException)
{
  return _server->get_singleton(name);
}

// this method should eventually do some kind of checking on the
// ClientContext to make sure they are allowed to construct objects on
// this server.

Kit_ptr ServerContextImpl::resolve(const char *type,
				   const Kit::PropertySeq &properties)    
  throw (SecurityException, CreationFailureException)
{
  /*
   * look for loaded kits first
   */
  klist_t::iterator k1 = _kits.lower_bound(type), k2 = _kits.upper_bound(type);
  for (klist_t::iterator i = k1; i != k2; ++i)
    if ((*i).second->supports(properties))
      {
	(*i).second->increment();
	return (*i).second->_this();
      }
  /*
   * now try the factories
   */
  PortableServer::POA_var root = _default_POA();
  std::ostringstream oss;
  oss << '#' << _counter++ << std::ends;
  PortableServer::POAManager_var manager = root->the_POAManager();
  PortableServer::POA_var poa =
    root->create_POA(oss.str().c_str(), manager, _policies);
  Logger::log(Logger::lifecycle) << "created new POA for kit of type "
	  << type << " (id is " << oss.str() << ')' << std::endl;
  KitImpl *kit = _server->create(type, properties, poa);
  if (!kit) throw CreationFailureException();
  kit->bind(ServerContext_var(_this()));
  _kits.insert(klist_t::value_type(type, kit));
  return kit->_this();
}

// this will nuke all allocated objects if the client has died, and then tell the caller 
// (Server) who will most likely destroy this ServerContext.

bool ServerContextImpl::ping()
{
  Trace trace("ServerContextImpl::ping");
  Prague::Guard<Mutex> guard(_mutex);
  bool alive = true;
  if (CORBA::is_nil(_client)) alive = false;
  else
    try { _client->ping();}
    catch (...) { alive = false;}
  return alive;
}
