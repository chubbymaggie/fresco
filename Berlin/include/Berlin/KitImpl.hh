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
#ifndef _Berlin_KitImpl_hh
#define _Berlin_KitImpl_hh

#include <Prague/Sys/Tracer.hh>
#include <Fresco/config.hh>
#include <Fresco/Kit.hh>
#include <Berlin/DefaultPOA.hh>

class ServerImpl;
class ServantBase;

class KitImpl : public virtual POA_Fresco::Kit,
		public virtual PortableServer::RefCountServantBase,
		private DefaultPOA
{
  friend class ServerImpl;
public:
  KitImpl(const std::string &, const Fresco::Kit::PropertySeq &);
  ~KitImpl();
  virtual KitImpl *clone(const Fresco::Kit::PropertySeq &) = 0;
  const std::string &repo_id() const { return _repo_id;}
  virtual Fresco::Kit::PropertySeq *properties() { return new Fresco::Kit::PropertySeq(*_props);}
  virtual void bind(Fresco::ServerContext_ptr) {};
  virtual CORBA::Boolean supports(const Fresco::Kit::PropertySeq &);

  void activate(::ServantBase *);
  void deactivate(::ServantBase *);
  PortableServer::POA_ptr _default_POA() { return DefaultPOA::_default_POA();}

  virtual void increment();
  virtual void decrement();
private:
  void activate(PortableServer::POA_ptr);
  void deactivate();
  PortableServer::POA_var         _poa;
  const std::string               _repo_id;
  const Fresco::Kit::PropertySeq *_props;
  int                             _refcount;
};

template <typename T>
inline T *create_kit(const std::string &repo, std::string props[], size_t n)
{
  Fresco::Kit::PropertySeq properties;
  properties.length(n/2);
  for (size_t i = 0; i != n/2; ++i)
    {
      properties[i].name = CORBA::string_dup((props++)->c_str());
      properties[i].value = CORBA::string_dup((props++)->c_str());
    }
  return new T(repo, properties);
}


#endif
