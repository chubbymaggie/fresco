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
#ifndef _EditorImpl_hh
#define _EditorImpl_hh

#include <Fresco/config.hh>
#include <Fresco/Widget.hh>
#include <Fresco/FigureKit.hh>
#include <Fresco/ToolKit.hh>
#include <Berlin/RefCountBaseImpl.hh>
#include <Berlin/ObserverImpl.hh>
#include <Berlin/ImplVar.hh>
#include <Berlin/RefCountVar.hh>
#include "UnidrawKitImpl.hh"
#include <map>

class EditorImpl : public virtual POA_Unidraw::Editor,
		   public RefCountBaseImpl
{
  typedef std::map<Fresco::Tag, RefCount_var<Unidraw::Tool> > tdict_t;
  class Observer;
  friend class Observer;
public:
  EditorImpl(UnidrawKitImpl *);
  virtual ~EditorImpl();
  virtual void append_tool(Unidraw::Tool_ptr, Fresco::Graphic_ptr);
  virtual Unidraw::Tool_ptr current_tool();
  virtual void current_tool(Unidraw::Tool_ptr);
  virtual Fresco::Controller_ptr create_viewer(Unidraw::Model_ptr, Fresco::Coord, Fresco::Coord);
  virtual Fresco::Controller_ptr toolbar();
private:
  UnidrawKitImpl         *_unidraw;
  Unidraw::Tool_var       _current;
  Widget::Choice_var      _choice;
  tdict_t                 _tools;
  Impl_var<Observer>      _tool_setter;
};

#endif
