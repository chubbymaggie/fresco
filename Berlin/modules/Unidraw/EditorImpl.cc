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
#include <Prague/Sys/Tracer.hh>
#include <Fresco/config.hh>
#include <Fresco/Selection.hh>
#include <Fresco/Widget.hh>
#include <Berlin/RefCountVar.hh>
#include "EditorImpl.hh"
#include "Viewer.hh"

using namespace Prague;
using namespace Fresco;
using namespace Unidraw;

using namespace Berlin::UnidrawKit;

class EditorImpl::Observer : public ObserverImpl
{
  public:
    Observer(EditorImpl *e) : my_parent(e) { my_parent->_add_ref(); }
    ~Observer() { my_parent->_remove_ref(); }
    void update(const CORBA::Any &any)
    {
    Fresco::Selection::Item *item;
    if (any >>= item)
    {
        if (item->toggled)
        my_parent->my_current =
            RefCount_var<Tool>::increment(my_parent->my_tools[item->id]);
    }
    else  std::cerr << "wrong message type" << std::endl;
    }
  private:
    EditorImpl *my_parent;
};

EditorImpl::EditorImpl(UnidrawKitImpl *unidraw) :
    my_unidraw(unidraw),
    my_tool_setter(new Observer(this))
{
    my_unidraw->_add_ref();
    WidgetKit_var widgets = my_unidraw->widgets();
    my_choice = widgets->toolbar();
    Fresco::Selection_var selection = my_choice->state();
    selection->attach(Observer_var(my_tool_setter->_this()));
}
EditorImpl::~EditorImpl()
{ my_unidraw->_remove_ref(); }

void EditorImpl::append_tool(Tool_ptr tool, Graphic_ptr graphic)
{
    Tag tag = my_choice->append_item(graphic);
    my_tools[tag] = RefCount_var<Tool>::increment(tool);
}

Unidraw::Tool_ptr EditorImpl::current_tool()
{ return Unidraw::Tool::_duplicate(my_current); }

void EditorImpl::current_tool(Tool_ptr current)
{ my_current = Unidraw::Tool::_duplicate(current); }
Controller_ptr EditorImpl::create_viewer(Unidraw::Model_ptr model,
                     Fresco::Coord width,
                     Fresco::Coord height)
{
    FigureKit_var figures = my_unidraw->figures();
    ToolKit_var tools = my_unidraw->tools();
    Viewer *viewer = new Viewer();
    activate(viewer);
    viewer->init(Editor_var(_this()), model, width, height,
         figures, tools);
    return viewer->_this();
}

Controller_ptr EditorImpl::toolbar()
{ return Widget::Choice::_duplicate(my_choice); }
