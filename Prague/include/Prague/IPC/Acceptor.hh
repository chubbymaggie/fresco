/*$Id$
 *
 * This source file is a part of the Berlin Project.
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
#ifndef _Prague_Acceptor_hh
#define _Prague_Acceptor_hh

#include <Prague/IPC/SocketAgent.hh>

namespace Prague
{

/*
 * an Acceptor is an Agent who's responsability is solely
 * to accept connection requests asynchronously.
 */
template <typename Connection>
class Acceptor : public SocketAgent
{
public:
  Acceptor(sockbuf *socket) : SocketAgent(socket) {}
  virtual void start();
private:
  virtual bool process(int, iomask_t);
};

template <typename Connection>
void Acceptor<Connection>::start()
{
  Trace trace("Acceptor::start");
  mask(out);
  obuf()->listen(1);
  SocketAgent::start();
}

template <typename Connection>
bool Acceptor<Connection>::process(int, iomask_t)
{
  Trace trace("Acceptor::process");
  Connection *connection = new Connection(ibuf()->accept());
  connection->start();
  connection->remove_ref();
  return false;
}

};

#endif