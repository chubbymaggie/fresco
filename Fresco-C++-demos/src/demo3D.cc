 /*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999, 2000 Stefan Seefeld <stefan@fresco.org> 
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

#include <Fresco/config.hh>
#include <Fresco/resolve.hh>
#include "Application.hh"
#include "PrimitiveDemo.hh"
#include "CubeDemo.hh"
#include <memory>

using namespace Prague;
using namespace Fresco;

int main(int argc, char **argv)
{
   Prague::GetOpt getopt(argv[0], "C++ fresco demo (3D)");
   getopt.add('h', "help", Prague::GetOpt::novalue, "help message");
   add_resolving_options_to_getopt(getopt);
   size_t argo = getopt.parse(argc, argv);
   argc -= argo;
   argv += argo;

   if (getopt.is_set('h'))
   {
     getopt.usage();
     exit(0);
   }

   try
   {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      PortableServer::POA_var poa = 
          resolve_init<PortableServer::POA>(orb, "RootPOA");
      Berlin::DefaultPOA::default_POA(poa);
      PortableServer::POAManager_var pman = poa->the_POAManager();
      pman->activate();
      
      Server_var server = resolve_server(getopt, orb);
      
      ClientContextImpl *client = new ClientContextImpl("Demo3D");
      ClientContext_var client_ref = client->_this();
      
      ServerContext_var server_context = 
          server->create_server_context(client_ref);
      
      Application *application = new Application(server_context, client_ref);
      
      std::auto_ptr<Demo> primitive(create_demo<PrimitiveDemo>(application));
      std::auto_ptr<Demo> cube(create_demo<CubeDemo>(application));
      
      application->run();
      delete application;
      delete client;
   }
   catch (const CORBA::COMM_FAILURE &)
   {
      std::cerr << "Could not connect to the display server "
                << "(CORBA::COMM_FAILURE)." << std::endl;
   }
   catch (const std::exception &e)
   {
      std::cerr << "Exception: " << e.what() << std::endl;
   }
}
