/*$Id$
 *
 * This source file is a part of the Fresco Project.
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
#ifndef _resolve_hh
#define _resolve_hh

#include <Fresco/Server.hh>
#include <Fresco/exception.hh>
#include <string>
#include <assert.h>
#include <Prague/Sys/GetOpt.hh>
#ifdef ORB_TAO
#include "orbsvcs/orbsvcs/CosNamingC.h"
#endif

// -----------------------------------------------------------------------------
// Wrapper for resolving CORBA initial references
// -----------------------------------------------------------------------------
template <class T>
typename T::_ptr_type resolve_init(CORBA::ORB_ptr orb, 
                                   char const * name)
{
    CORBA::Object_var object;
    try
    {
        object = orb->resolve_initial_references(name);
    }
    catch (const CORBA::ORB::InvalidName &e)
    {
        std::cerr << "Invalid Name : " << name << std::endl;
        throw;
    }
    catch (const CORBA::Exception &e)
    {
        std::cerr << "Cannot get initial reference for " << name << ": " << e 
                  << std::endl;
        throw 0;
    }
    assert(!CORBA::is_nil(object));

    typename T::_var_type reference;
    try
    {
        reference = T::_narrow(object);
    }
    catch (const CORBA::Exception &e)
    {
        std::cerr << "Cannot downcast reference for " << name << ": " << e 
                  << std::endl;
        throw 0;
    }
    if (CORBA::is_nil(reference))
    {
        std::cerr << "Incorrect type of reference for " << name << std::endl;
        throw 0;
    }
    return reference._retn();
}

// -----------------------------------------------------------------------------
// Wrappers for using the CORBA Nameservice
// -----------------------------------------------------------------------------
template <class T>
typename T::_ptr_type resolve_name(CosNaming::NamingContext_ptr context, 
                                   CosNaming::Name const &name)
{
    CORBA::Object_var object;
    try
    {
        object = context->resolve(name);
    }
    catch (const CosNaming::NamingContext::NotFound &e)
    {
        // XXX: insert warning message here? -- neiljp
        throw;
    }
    catch (const CORBA::Exception &e)
    {
        std::cerr << "Cannot resolve binding: " << e << std::endl;
        throw 0;
    }
    if (CORBA::is_nil(object))
    {
        std::cerr << "Nil binding in Naming service" << std::endl;
        throw 0;
    }

    typename T::_var_type reference;
    try
    {
        reference = T::_narrow(object);
    }
    catch (const CORBA::Exception &e)
    {
        std::cerr << "Cannot narrow reference: " << e << std::endl;
        throw 0;
    }
    if (CORBA::is_nil(reference))
    {
        std::cerr << "Reference has incorrect type" << std::endl;
        throw 0;
    }
    return reference._retn();
}

template <class T>
typename T::_ptr_type resolve_name(CosNaming::NamingContext_ptr context, 
                                   char const * name)
{
    CosNaming::Name cosname;
    cosname.length(1);
    cosname[0].id   = name; 
    cosname[0].kind = "Object"; // string_dup ?? 
    return resolve_name<T>(context, cosname);
}

inline void bind_name(CORBA::ORB_ptr orb, 
                      CORBA::Object_ptr object, 
                      char const * name)
{
    CosNaming::NamingContext_var context = 
      resolve_init<CosNaming::NamingContext>(orb, "NameService");
    CosNaming::Name cosname;
    cosname.length(1);
    cosname[0].id   = name; 
    cosname[0].kind = "Object"; // string_dup ?? 
    context->rebind(cosname, object);
}

// -----------------------------------------------------------------------------
// Wrappers for resolving Fresco kits
// XXX Should these be in the Fresco namespace?
// -----------------------------------------------------------------------------
template <class T>
typename T::_ptr_type resolve_kit(Fresco::ServerContext_ptr context, 
                                  char const * name, 
                                  Fresco::Kit::PropertySeq const &props)
{
    CORBA::Object_ptr object;
    try
    {
        object = context->resolve(name, props);
    }
    catch(const CORBA::Exception &e)
    {
        std::cerr << "Cannot resolve reference for " << name << ": " << e 
                  << std::endl;
        return T::_nil();
    }
    typename T::_var_type reference;
    try
    {
        reference = T::_narrow(object);
    }
    catch (const CORBA::Exception &e)
    {
        std::cerr << "Cannot narrow reference: " << e << std::endl;
        throw 0;
    }
    if (CORBA::is_nil(reference))
    {
        std::cerr << "Reference has incorrect type" << std::endl;
        throw 0;
    }
    return reference._retn();
}

template <class T>
typename T::_ptr_type resolve_kit(Fresco::ServerContext_ptr context, 
                                  char const * name)
{
    Fresco::Kit::PropertySeq empty;
    empty.length(0);
    return resolve_kit<T>(context, name, empty);
}

// Simplified wrappers, to avoid writing out the repo-id repeatedly

// need to predeclare core kits
namespace Fresco
{
  class LayoutKit;
  class ToolKit;
  class TextKit;
  class DesktopKit;
  class CommandKit;
  class WidgetKit;
  class RasterKit;
  class FigureKit;
  class GadgetKit;
  class PrimitiveKit;
};

// Kit traits, pre-specialised for core kits
template<typename T> struct Kit_traits{};
template<> struct Kit_traits<Fresco::LayoutKit> {static char const id[];};
template<> struct Kit_traits<Fresco::ToolKit> {static char const id[];};
template<> struct Kit_traits<Fresco::TextKit> {static char const id[];};
template<> struct Kit_traits<Fresco::DesktopKit> {static char const id[];};
template<> struct Kit_traits<Fresco::CommandKit> {static char const id[];};
template<> struct Kit_traits<Fresco::WidgetKit> {static char const id[];};
template<> struct Kit_traits<Fresco::RasterKit> {static char const id[];};
template<> struct Kit_traits<Fresco::FigureKit> {static char const id[];};
template<> struct Kit_traits<Fresco::GadgetKit> {static char const id[];};
template<> struct Kit_traits<Fresco::PrimitiveKit> {static char const id[];};

template <class T>
typename T::_ptr_type resolve_kit(Fresco::ServerContext_ptr context, 
                                  Fresco::Kit::PropertySeq const &props)
{
    return resolve_kit<T>(context, Kit_traits<T>::id, props);
}

template <class T>
typename T::_ptr_type resolve_kit(Fresco::ServerContext_ptr context)
{
    Fresco::Kit::PropertySeq empty;
    empty.length(0);
    return resolve_kit<T>(context, Kit_traits<T>::id, empty);
}

// -----------------------------------------------------------------------------
// Ways to find default settings for server-reference transfer
// -----------------------------------------------------------------------------
char const * default_reference_transfer_method();
char const * default_server_id();
char const * default_ior_file_path();

// -----------------------------------------------------------------------------
// Resolve server using strings directly
// -----------------------------------------------------------------------------

// resolve server using strings directly
Fresco::Server_ptr resolve_server(std::string server_id,
                                  std::string reference_transfer_method,
                                  std::string ior_file_path,
                                  CORBA::ORB_ptr orb);

// -----------------------------------------------------------------------------
// Used to publish the server reference (call in order)
// -----------------------------------------------------------------------------

// check export_method is valid, and set method
void set_server_reference_export_method(std::string export_method);

// get poa to use for creating server
PortableServer::POA_ptr get_server_poa(CORBA::ORB_ptr orb,
                                       PortableServer::POA_ptr default_poa);

// publish server reference
void publish_server(Fresco::Server_ptr server,
                    std::string server_id,
                    std::string ior_file_path,
                    CORBA::ORB_ptr orb);

// -----------------------------------------------------------------------------
// Un-publish the server reference (once server is finished)
// -----------------------------------------------------------------------------

void unpublish_server(CORBA::ORB_ptr orb);

// -----------------------------------------------------------------------------
// Helpers if using external Prague::Getopt
// -----------------------------------------------------------------------------

// add server-publishing/resolving options to a getopt
void add_resolving_options_to_getopt(Prague::GetOpt &getopt);

// resolve server using getopt
Fresco::Server_ptr resolve_server(Prague::GetOpt const &getopt, 
                                  CORBA::ORB_ptr orb);

// publish server using getopt
// NOTE: call set_server_reference_export_method and get_server_poa before this,
//       as with publish_server above.
void publish_server(Fresco::Server_ptr server,
                    Prague::GetOpt const &getopt,
                    CORBA::ORB_ptr orb);

#endif
