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
#ifndef _EditorImpl_hh
#define _EditorImpl_hh

#include <Warsaw/config.hh>
#include <Warsaw/Widget.hh>
#include <Warsaw/FigureKit.hh>
#include <Warsaw/ToolKit.hh>
#include <Berlin/RefCountBaseImpl.hh>
#include <Berlin/ObserverImpl.hh>
#include <Berlin/ImplVar.hh>
#include <Berlin/RefCountVar.hh>
#include <Unidraw/UnidrawKitImpl.hh>
#include <map>

class EditorImpl : public virtual POA_Unidraw::Editor,
		   public RefCountBaseImpl
{
  typedef std::map<Warsaw::Tag, RefCount_var<Unidraw::Tool> > tdict_t;
  class Observer;
  friend class Observer;
public:
  EditorImpl(UnidrawKitImpl *);
  virtual ~EditorImpl();
  virtual void append_tool(Unidraw::Tool_ptr, Warsaw::Graphic_ptr);
  virtual Unidraw::Tool_ptr current_tool();
  virtual void current_tool(Unidraw::Tool_ptr);
  virtual Warsaw::Controller_ptr create_viewer(Unidraw::Model_ptr, Warsaw::Coord, Warsaw::Coord);
  virtual Warsaw::Controller_ptr toolbar();
private:
  UnidrawKitImpl         *_unidraw;
  Unidraw::Tool_var       _current;
  Widget::Choice_var      _choice;
  tdict_t                 _tools;
  Impl_var<Observer>      _tool_setter;
};

#endif
