 /*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999, 2000 Stefan Seefeld <stefan@berlin-consortium.org> 
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

#include <Warsaw/config.hh>
#include <Warsaw/resolve.hh>
#include "Application.hh"
#include "PrimitiveDemo.hh"

using namespace Prague;
using namespace Warsaw;

bool running;

int main(int argc, char **argv)
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CosNaming::NamingContext_var context = resolve_init<CosNaming::NamingContext>(orb, "NameService");
    PortableServer::POA_var poa = resolve_init<PortableServer::POA>(orb, "RootPOA");
    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    ClientContextImpl *client = new ClientContextImpl;

    Server_var s = resolve_name<Server>(context, "IDL:Warsaw/Server:1.0");
    ServerContext_var server = s->create_server_context(ClientContext_var(client->_this()));

    Application *application = new Application(server);

    Demo *primitive = new PrimitiveDemo(application);
    application->run();
    delete primitive;
    delete application;
  } catch (CORBA::COMM_FAILURE c) {
    std::cerr << "Could not connect to the berlin server (CORBA::COMM_FAILURE)." << std::endl;
  }
}