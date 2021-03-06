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

#include "TextDemo.hh"

using namespace Fresco;

TextDemo::TextDemo(Application *a) : Demo(a)
{
    TextKit_var text = my_application->resolve<TextKit>("IDL:fresco.org/Fresco/TextKit:1.0");
    ToolKit_var tools = my_application->resolve<ToolKit>("IDL:fresco.org/Fresco/ToolKit:1.0");

    Babylon::Char chars[] =
    {
    0x004d, 0x0061, 0x0067, 0x0079, 0x0061, 0x0072, 0x0020, 0x0420,
    0x0443, 0x0441, 0x0441, 0x043a, 0x0438, 0x0439, 0x0020, 0x0395,
    0x039b, 0x039b, 0x0397, 0x039d, 0x0399, 0x039a, 0x0391, 0x0020,
    0x65e5, 0x672c, 0x8a9e, 0x0020, 0x4e2d, 0x6587, 0x0020, 0xd55c,
    0xad6d, 0xc5b4
    };

    Babylon::String str(chars, 34);
    Graphic_var txt = text->chunk(Unicode::to_CORBA(str));
    Controller_var group = tools->group(Graphic_var(tools->rgb(txt, 0.2, 0.3, 0.5)));
    my_application->append(group, Babylon::String("text"));
};
