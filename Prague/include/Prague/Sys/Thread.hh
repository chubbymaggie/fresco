/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999-2003 Stefan Seefeld <stefan@fresco.org>
 * Copyright (C) 2001 Tobias Hunger <tobias@fresco.org>
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
#ifndef _Prague_Thread_hh
#define _Prague_Thread_hh

#include <Prague/Sys/Time.hh>
#include <pthread.h>
#include <semaphore.h>
#include <cerrno>
#include <vector>
#include <string>
#include <utility>
#include <stdexcept>

namespace Prague
{

  class Mutex
  {
    public:
      class Attribute
      {
        public:
          Attribute() { pthread_mutexattr_init (&impl); }
          ~Attribute() { pthread_mutexattr_destroy(&impl); }
          pthread_mutexattr_t impl;
      };
      // enum type { fast = PTHREAD_MUTEX_FAST_NP,
      //             recursive = PTHREAD_MUTEX_RECURSIVE_NP };
      Mutex() { pthread_mutex_init(&impl, 0); }
      //  Mutex(type t);
      ~Mutex() { pthread_mutex_destroy(&impl); }
      void lock() { pthread_mutex_lock(&impl); }
      void unlock() { pthread_mutex_unlock(&impl); }
      bool trylock() { return pthread_mutex_trylock(&impl); }
      pthread_mutex_t impl;
  };

  //. Semaphores are used to limit the number of concurrent accesses to some resource.
  class Semaphore
  {
    public:
      Semaphore(unsigned int v = 0) { sem_init(&impl, 0, v); }
      ~Semaphore() { sem_destroy(&impl); }
      void wait() { sem_wait(&impl); }
      bool trywait() { return sem_trywait(&impl); }
      void post() { sem_post(&impl); }
      int value() { int v; sem_getvalue(&impl, &v); return v; }
      sem_t impl;
    private:
      Semaphore(const Semaphore &);
      Semaphore &operator = (const Semaphore &);
  };

  template<class T>
  class Lock_Trait
  {
    public:
      void lock(T & t) { t.lock(); }
      void unlock(T & t) { t.unlock(); }
  };

  template<>
  class Lock_Trait<Semaphore>
  {
    public:
      void lock(Semaphore & t) { t.wait(); }
    void unlock(Semaphore & t) { t.post(); }
  };

  template<class T>
  class WLock_Trait
  {
    public:
      void lock(T & t) { t.wlock(); }
      void unlock(T & t) { t.unlock(); }
  };

  // A helperclass
  template <class T, class L_TRAIT = Lock_Trait<T> >
  class Guard
  {
    public:
      Guard(T & t) : vip(t), trait() { trait.lock(vip); }
      ~Guard() { trait.unlock(vip); }
    private:
      Guard(const Guard<T> &);
      Guard & operator = (const Guard &);
      T & vip; // Who else get's guarded?
      L_TRAIT trait;
  };

  //. condition variables are used to commuicate changed conditions between threads.
  class Condition
  {
    public:
      class Attribute
      {
        public:
          Attribute() { pthread_condattr_init(&impl); }
          ~Attribute() { pthread_condattr_destroy(&impl); }
          pthread_condattr_t impl;
      };
      Condition(Mutex &m) : mutex(m) { pthread_cond_init(&impl, 0); }
      ~Condition() { pthread_cond_destroy(&impl); }
      void broadcast() { pthread_cond_broadcast(&impl); }
      void signal() { pthread_cond_signal(&impl); }
      void wait() { pthread_cond_wait(&impl, &mutex.impl); }
      void wait(const Time &t) { timespec ts = t; pthread_cond_timedwait(&impl, &mutex.impl, &ts); }
      pthread_cond_t impl;
    private:
      Mutex &mutex;
  };

  class RWLock
  {
    public:
      class Attribute
      {
        public:
          Attribute() { pthread_rwlockattr_init (&impl); }
          ~Attribute() { pthread_rwlockattr_destroy(&impl); }
          pthread_rwlockattr_t impl;
      };
      RWLock() { pthread_rwlock_init(&impl, 0); }
      RWLock(const Attribute &a) { pthread_rwlock_init(&impl, &a.impl); }
      ~RWLock() { pthread_rwlock_destroy(&impl); }
      void rlock() { pthread_rwlock_rdlock(&impl); }
      void wlock() { pthread_rwlock_wrlock(&impl); }
      void unlock() { pthread_rwlock_unlock(&impl); }
      bool tryrlock() { return pthread_rwlock_tryrdlock(&impl); }
      bool trywlock() { return pthread_rwlock_trywrlock(&impl); }
      pthread_rwlock_t impl;
  };

  class RLockTrait
  {
    public:
      static void lock(RWLock &l) { l.rlock(); }
      static void unlock(RWLock &l) { l.unlock(); }
  };

  class WLockTrait
  {
    public:
      static void lock(RWLock &l) { l.wlock(); }
      static void unlock(RWLock &l) { l.unlock(); }
  };

  //. a thread housekeeping class.
  class Thread
  {
      struct Guard
      {
          Guard();
          ~Guard();
      };
      friend struct Guard;
    public:
      enum priority_t { LOW, NORMAL, HIGH };
      enum state_t { READY, RUNNING, TERMINATED };
      static const void *CANCELED;
      template <class T> class Data;
      template <class T> class Queue;
      typedef void *(*proc)(void *);
      class Attribute
      {
        public:
          Attribute() { pthread_attr_init(&impl); }
          ~Attribute() { pthread_attr_destroy(&impl); }
          pthread_attr_t impl;
      };
      class Exception : public std::logic_error
      {
        public:
          Exception(const std::string &m) : std::logic_error(m) { }
      };
      Thread(proc, void *, priority_t = NORMAL);
      ~Thread();
      //. return the thread's priority
      priority_t priority() { Prague::Guard<Mutex> g(my_mutex); return my_priority; }
      //. return the thread's current state
      state_t state() { Prague::Guard<Mutex> g(my_mutex); return my_state; }
      //. start the thread
      void start() throw (Exception);
      //. wait for the thread to finish returning its return value
      void join(void **) throw (Exception);
      //. cancel the thread
      void cancel();
      //. detach the thread
      void detach();
      //. exit the calling thread with return value r
      static void exit(void *r);
      //. delay execution of the calling thread
      static bool delay(const Time &);
      //. cancellation point
      static void testcancel() { pthread_testcancel();}
      //. return a Thread pointer for the calling thread or 0
      //. if it wasn't created by this Thread class
      static Thread *self();
      //. return a thread id. This gives a valid number even for third party threads
      static unsigned long id();
    private:
      Thread(pthread_t);
      static void *start(void *);

      static unsigned long counter;
      static Thread *main;
      static Guard guard;
      static pthread_key_t self_key;
      static pthread_key_t id_key;
      static Mutex         id_mutex;

      proc       my_proc;
      void      *my_arg;
      pthread_t  my_thread;
      priority_t my_priority;
      state_t    my_state;
      bool       my_detached;
      Mutex      my_mutex;
  };

  inline Thread *Thread::self()
  {
      return reinterpret_cast<Thread *>(pthread_getspecific(self_key));
  }

  inline unsigned long Thread::id()
  {
      unsigned long *_id = reinterpret_cast<unsigned long *>(pthread_getspecific(id_key));
      if (!_id)
      {
          Prague::Guard<Mutex> guard(id_mutex);
          if (!_id)
          {
              _id = new unsigned long (counter++);
              pthread_setspecific(id_key, _id);
          }
      }
      return *_id;
  }

} // namespace Prague

#endif
