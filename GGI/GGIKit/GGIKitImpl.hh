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
#ifndef _GGIKitImpl_hh
#define _GGIKitImpl_hh

#include <Warsaw/config.hh>
#include <Warsaw/GGIKit.hh>
#include <Berlin/KitImpl.hh>
#include <vector>

class GGIKitImpl : public virtual POA_GGI::GGIKit,
		   public KitImpl
{
 public:
  GGIKitImpl(const std::string &, const Warsaw::Kit::PropertySeq &);
  virtual ~GGIKitImpl();
  virtual KitImpl *clone(const Warsaw::Kit::PropertySeq &p) { return new GGIKitImpl(repo_id(), p);}
  virtual GGI::Visual_ptr create_visual(Warsaw::PixelCoord, Warsaw::PixelCoord);
};

#endif
