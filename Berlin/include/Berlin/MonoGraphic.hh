/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999 Stefan Seefeld <stefan@fresco.org> 
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
#ifndef _Berlin_MonoGraphic_hh
#define _Berlin_MonoGraphic_hh

#include <Berlin/GraphicImpl.hh>

namespace Berlin
{

  class MonoGraphic : public GraphicImpl
  {
    public:
      MonoGraphic();
      virtual ~MonoGraphic();
      
      virtual Fresco::Graphic_ptr body();
      virtual void body(Fresco::Graphic_ptr);
      virtual void append_graphic(Fresco::Graphic_ptr);
      virtual void prepend_graphic(Fresco::Graphic_ptr);
      virtual void remove_graphic(Fresco::Tag);
      virtual void remove_child_graphic(Fresco::Tag);
      virtual Fresco::GraphicIterator_ptr first_child_graphic();
      virtual Fresco::GraphicIterator_ptr last_child_graphic();
      
      virtual Fresco::Transform_ptr transformation();
      virtual void request(Fresco::Graphic::Requisition &);
      virtual void extension(const Fresco::Allocation::Info &,
                 Fresco::Region_ptr);
      virtual void shape(Fresco::Region_ptr);
      
      virtual void traverse(Fresco::Traversal_ptr);
    protected:
      Edge          my_child;
      Prague::Mutex my_mutex;
  };
  
} // namespace

#endif 
