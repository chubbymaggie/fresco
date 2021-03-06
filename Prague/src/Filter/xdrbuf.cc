/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999 Stefan Seefeld <stefan@fresco.org>
 * http://www.fresco.org
 *
 * this code is based on binio from Dietmar Kuehl:
 *
 * Copyright (C) 1996 Dietmar Kuehl
 * Universitaet Konstanz, Lehrstuhl fuer praktische Informatik I
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
#include "Prague/Filter/xdrbuf.hh"

using namespace Prague;

// The current implementation in libg++ does not define the method
// 'pubsync()' for 'streambuf' but has no access restrictions on 'sync()
// => Use this function.

// The following somewhat defeats the naming of 'long' but then...

#if LONG_BIT == 64
  typedef int  long_t;
#else
  typedef long long_t;
#endif

extern "C" bool_t xdrbuf_getlong(XDR *, long_t *);
extern "C" bool_t xdrbuf_putlong(XDR *, const long_t *);
extern "C" bool_t xdrbuf_getbytes(XDR *, caddr_t, unsigned int);
extern "C" bool_t xdrbuf_putbytes(XDR *, const char *, unsigned int);
extern "C" unsigned int  xdrbuf_getpostn(const XDR *);
extern "C" bool_t xdrbuf_setpostn(XDR *, unsigned int);
extern "C" long_t *xdrbuf_inline(XDR *, int);
extern "C" void   xdrbuf_destroy(XDR *);

#ifdef __sgi__
static xdr_ops xdrsb_ops =
#else
static XDR::xdr_ops xdrsb_ops =
#endif
{
    xdrbuf_getlong,
    xdrbuf_putlong,
    xdrbuf_getbytes,
    xdrbuf_putbytes,
    xdrbuf_getpostn,
    xdrbuf_setpostn,
    xdrbuf_inline,
    xdrbuf_destroy,
};

void xdrbuf_create(XDR *xdrs, std::streambuf *sb, xdr_op op)
{
    xdrs->x_op      = op;
    xdrs->x_ops     = &xdrsb_ops;
    xdrs->x_public  = 0; // bring into a defined state
    xdrs->x_private = reinterpret_cast<caddr_t> (sb);
    xdrs->x_base    = 0; // what is this for?
    xdrs->x_handy   = 0; // not used
}

void xdrbuf_destroy(XDR *xdrs)
{
    // like for 'iostream' the 'streambuf' is owned by someone else.
    // However, synchronization would be nice:
    std::streambuf *sb = reinterpret_cast<std::streambuf *> (xdrs->x_private);
    if (sb != 0) sb->pubsync();
}

void xdrbuf_reseat(XDR *xdrs, std::streambuf *sb)
{
    // A method to implement the 'rdbuf()' mechanism of 'binios'
    reinterpret_cast<std::streambuf *> (xdrs->x_private)->pubsync();
    xdrs->x_private = reinterpret_cast<caddr_t> (sb);
}

bool_t xdrbuf_getlong(XDR *xdrs, long_t *ptr)
{
    // Why is 'x_getlong()' defined to work on 'long'? 'ntohl' and family works
    // on 'unsigned long'!
    std::streambuf *sb = reinterpret_cast<std::streambuf *> (xdrs->x_private);
    if (sb->sgetn(reinterpret_cast<char *> (ptr), 4) == 4)
    {
        *ptr = ntohl(*ptr);
        return TRUE;
    }
    else return FALSE;
}

bool_t xdrbuf_putlong(XDR *xdrs, const long_t *ptr)
{
    std::streambuf *sb = reinterpret_cast<std::streambuf *> (xdrs->x_private);
    long l = htonl(*ptr);
    if (sb->sputn(reinterpret_cast<char *> (&l), 4) == 4) return TRUE;
    else return FALSE;
}

bool_t xdrbuf_getbytes(XDR *xdrs, caddr_t ptr, unsigned int len)
{
    std::streambuf *sb = reinterpret_cast<std::streambuf *> (xdrs->x_private);
    if (sb->sgetn(reinterpret_cast<char *> (ptr), len) == (int) len) return TRUE;
    else return FALSE;
}

bool_t xdrbuf_putbytes(XDR *xdrs, const char *ptr, unsigned int len)
{
    std::streambuf *sb = reinterpret_cast<std::streambuf *> (xdrs->x_private);
    if (sb->sputn(const_cast<char *> (ptr), len) == (int) len) return TRUE;
    else return FALSE;
}

unsigned int xdrbuf_getpostn(const XDR *xdrs)
{
    std::streambuf *sb = reinterpret_cast<std::streambuf *> (xdrs->x_private);
    std::streampos pos =
      sb->pubseekoff(0, std::ios::cur, xdrs->x_op == XDR_ENCODE? std::ios::out : std::ios::in);
    return static_cast<unsigned int> (pos);
}

bool_t xdrbuf_setpostn(XDR *xdrs, unsigned int p)
{
    std::streambuf *sb = reinterpret_cast<std::streambuf *> (xdrs->x_private);
    std::streampos  pos = static_cast<std::streampos> (p);
    std::ios::openmode which = xdrs->x_op == XDR_ENCODE? std::ios::out : std::ios::in;
    if (sb->pubseekpos(pos, which) != static_cast<std::streampos> (streamoff(-1))) return TRUE;
    else return FALSE;
}

long_t *xdrbuf_inline(XDR * /* xdrs */, int /* len */)
{
    /*
     * To implement this, it would be necessary to gain specific access
     * to the 'streambuf' which cannot be guaranteed by all
     * 'streambuf's.  I'm not sure whether this method can usefully be
     * implemented... I hope, that it is at least not necessary to
     * implement this method to make the XDR conversions work!
     */
    return 0;
}
