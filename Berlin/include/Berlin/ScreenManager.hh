/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999, 2000 Stefan Seefeld <stefan@berlin-consortium.org> 
 * Copyright (C) 2000 Graydon Hoare <graydon@pobox.com> 
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
#ifndef _ScreenManager_hh
#define _ScreenManager_hh

#include <Warsaw/config.hh>
#include <Warsaw/Region.hh>
#include <Warsaw/Input.hh>
#include <Warsaw/DrawingKit.hh>
#include <Prague/Sys/Thread.hh>
#include <Berlin/GGI.hh>
#include <Berlin/ImplVar.hh>
#include <Berlin/EventManager.hh>
#include <Berlin/DrawTraversalImpl.hh>
#include <vector>

class ScreenImpl;
class RegionImpl;

class ScreenManager
{
public:
  ScreenManager(ScreenImpl *, EventManager *, DrawingKit_ptr);
  ~ScreenManager();
  void damage(Region_ptr);
  void repair();
  void nextEvent() { emanager->nextEvent();}
  void run();
private:
  ScreenImpl *screen;
  EventManager *emanager;
  DrawingKit_var drawing;
  GGI::Drawable *drawable;
  Impl_var<RegionImpl> theDamage;
  Impl_var<RegionImpl> tmpDamage;
  Impl_var<DrawTraversalImpl> traversal;
  Prague::Mutex mutex;
};

#endif /* _ScreenManager_hh */
