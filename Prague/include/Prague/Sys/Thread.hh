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
#ifndef _Thread_hh
#define _Thread_hh

#include <Prague/Sys/Time.hh>
#include <pthread.h>
#include <vector>
#include <pair.h>

namespace Prague
{

class Mutex
{
  friend class Condition;
public:
  enum type { fast = PTHREAD_MUTEX_FAST_NP,
	      recursive = PTHREAD_MUTEX_RECURSIVE_NP};
  Mutex() { pthread_mutex_init(&mutex, 0);}
  Mutex(type t);
  ~Mutex() { pthread_mutex_destroy(&mutex);}
  void lock() { pthread_mutex_lock(&mutex);}
  void unlock() { pthread_mutex_unlock(&mutex);}
  bool trylock() { return pthread_mutex_trylock(&mutex);}
private:
  pthread_mutex_t mutex;
};

class MutexGuard
{
public:
  MutexGuard(Mutex &m) : mutex(m) { mutex.lock();}
  ~MutexGuard() { mutex.unlock();}
private:
  MutexGuard(const MutexGuard &);
  MutexGuard &operator = (const MutexGuard &);
  Mutex &mutex;
};

class Condition
{
public:
  Condition(Mutex &m) : mutex(m) { pthread_cond_init(&condition, 0);}
  ~Condition() { pthread_cond_destroy(&condition);}
  void broadcast() { pthread_cond_broadcast(&condition);}
  void signal() { pthread_cond_signal(&condition);}
  void wait() { pthread_cond_wait(&condition, &mutex.mutex);}
  void wait(const Time &t) { timespec ts = t; pthread_cond_timedwait(&condition, &mutex.mutex, &ts);}
private:
  pthread_cond_t condition;
  Mutex &mutex;
};

class Semaphore
{
public:
  Semaphore(unsigned int v) : condition(mutex), value(v) {}
  ~Semaphore() {}
  void wait() { MutexGuard guard(mutex); while (value == 0) condition.wait(); value--;}
  bool trywait() { MutexGuard guard(mutex); if (value == 0) return false; value--; return true;}
  void post() { { MutexGuard guard(mutex); value++;} condition.signal();}
private:
  Semaphore(const Semaphore &);
  Semaphore &operator = (const Semaphore &);
  Mutex mutex;
  Condition condition;
  int value;
};

class SemaphoreGuard
{
public:
    SemaphoreGuard(Semaphore &s) : semaphore(s) { semaphore.wait();}
    ~SemaphoreGuard() { semaphore.post();}
private:
  SemaphoreGuard(const SemaphoreGuard &);
  SemaphoreGuard &operator = (const SemaphoreGuard &);
  Semaphore &semaphore;
};

class Thread
{
  typedef vector<pair<pthread_t, Thread *> > table;
public:
  template <class T> class Data;
  template <class T> class Queue;
  typedef void *(*proc)(void *);
  Thread(proc pp, void *a) : p(pp), arg(a), running(false) {}
  virtual ~Thread() { cancel(); join();}
  void  start() { if (pthread_create(&thread, 0, &start, this) == 0) running = true;}
  void *join() { void *ret; pthread_join(thread, &ret); return ret;}
  void  cancel() { if (running) pthread_cancel(thread);}
  void  exit() { if (running) pthread_exit(0);}
  void  detach() { pthread_detach(thread);}
  static bool delay(const Time &);
  static Thread *self() { return find(pthread_self());}
private:
  proc p;
  void *arg;
  pthread_t thread;
  bool running;
  static void *start(void *);
  static void append(pthread_t, Thread *);
  static void remove(pthread_t);
  static Thread *find(pthread_t);
  static table threads;
  static Mutex mutex;
};

};

#endif /* _Thread_hh */
