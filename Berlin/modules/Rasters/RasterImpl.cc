/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Brent Fulgham <bfulgham@debian.org>
 * Copyright (C) 1999 Stefan Seefeld 
 * http://www.berlin-consortium.org
 *
 * this code is based on code from Fresco.
 * Copyright (c) 1987-91 Stanford University
 * Copyright (c) 1991-94 Silicon Graphics, Inc.
 * Copyright (c) 1993-94 Fujitsu, Ltd.
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

#include "Image/RasterImpl.hh"
#include <fstream>

RasterImpl::RasterImpl()
	: data(0), totBytes(0)
{
	rpng = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	rinfo = png_create_info_struct(rpng);
}

RasterImpl::RasterImpl(const char* file)
{
	rpng = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	rinfo = png_create_info_struct(rpng);
	ifstream ifs(file);
	PNGDecoder decoder(ifs.rdbuf());
	totBytes = decoder.decode(rpng, rinfo, data);
}

RasterImpl::~RasterImpl()
{
	// delete data;
	png_destroy_read_struct(&rpng, &rinfo, 0);
	delete[] data;
}

void RasterImpl::load(const Data& buffer)
{
	/*
	 * free the old data
	 */
	delete[] data;
	data = NULL;
	totBytes = 0;
	
	/*
	 * create streambuf and associate it with the sequence
	 */
	rasterbuf rbuf;
	unsigned long length = buffer.length();
	rbuf.setg ((char*)(buffer.NP_data()), (char*)(buffer[length]),
			   (char*)(buffer[length]));

	/*
	 * read it in
	 */
 	PNGDecoder decoder(&rbuf);
	totBytes = decoder.decode(rpng, rinfo, data);
}

void RasterImpl::export(Data*& buffer)
{
	/*
	 * set up buffer to hold new data
	 */
	rasterbuf rbuf;
	unsigned long length = buffer->length();
	rbuf.setp((char*)(buffer->NP_data()), (char*)(buffer+length));	
	/*
	 * create temporary write structures here...
	 */
	PNGEncoder encoder(&rbuf);
	encoder.encode(rpng, rinfo, data);
}

void RasterImpl::getData(Data*& buffer)
{
	/*
	 * Note:  This differs from 'export' in that it returns
	 * the actual manipulated block of memory that represents
	 * the image.
	 *
	 * In addition, because of OpenGL's sematics, we need to
	 * reverse the order of the pixel bytes to get the image
	 * to come out right-side-up.
	 */
	// Make buffer correct size
	buffer->length(totBytes);
	
	for (int i=0; i < totBytes; i++)
		buffer->operator[](i) = *(data+((totBytes-1)-i));
}