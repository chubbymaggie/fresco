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
#ifndef _Prague_Dispatcher_hh
#define _Prague_Dispatcher_hh

#include <Prague/Sys/Signal.hh>
#include <Prague/Sys/FdSet.hh>
#include <Prague/Sys/ThreadPool.hh>
#include <Prague/IPC/Agent.hh>
#include <vector>
#include <map>
#include <unistd.h>

namespace Prague
{

class Dispatcher
{
  typedef vector<Agent *> alist_t;
  struct task
  {
    task() : fd(-1), agent(0), mask(Agent::none) {}
    task(int ffd, Agent *a, Agent::iomask_t m) : fd(ffd), agent(a), mask(m) {}
    bool operator < (const task &t) const { return fd < t.fd;}
    int fd;
    Agent *agent;
    Agent::iomask_t mask;
    mutable Mutex mutex;
  };
  typedef map<int, task *> repository_t;
  struct Handler
  //. Handler is responsible for calling a specific method
  //. (determined by the mask) on the agent
  {
    Handler(const task &tt) : t(tt) {}
    void process() { dispatcher->process(t);}
    const task &t;
  };
  friend struct Handler;
  struct Acceptor { Handler *consume(const task *t) const { return new Handler(*t);}};
  struct Cleaner { ~Cleaner();};
  friend struct Cleaner;
public:
  static Dispatcher *instance();
  void bind(Agent *, int, Agent::iomask_t);
  void release(Agent *, int = -1);
private:
  Dispatcher();
  virtual ~Dispatcher();
  void wait();
  void notify() { char *c = "c"; write(wakeup[1], c, 1);}
  static void *run(void *);
  void dispatch(task *);
  void process(const task &);
  void deactivate(const task &);
  void activate(const task &);
  static Dispatcher *dispatcher;
  static Mutex  singletonMutex;
  static Cleaner cleaner;
  Signal::Notifier *notifier;
  Mutex         mutex;
  FdSet         rfds;
  FdSet         wfds;
  FdSet         xfds;
  alist_t       agents;
  repository_t  rchannel;
  repository_t  wchannel;
  repository_t  xchannel;
  int           wakeup[2];
  Thread::Queue<task *> tasks;
  Acceptor      acceptor;
  ThreadPool<task *, Acceptor, Handler> workers;
  Thread        server;
};

};

#endif /* _Prague_Dispatcher_hh */

