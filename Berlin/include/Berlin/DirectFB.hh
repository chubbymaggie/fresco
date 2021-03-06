/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 2001 Tobias Hunger <tobias@berlin-consortium.org> 
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
#ifndef _DirectFB_hh
#define _DirectFB_hh

#include <Warsaw/config.hh>
#include <Warsaw/Types.hh>
#include <Warsaw/Input.hh>
#include <Prague/Sys/Tracer.hh>
#include <Berlin/Logger.hh>
#include <vector>
#include <cstring>

extern "C"
{
#include <directfb.h>
}

class DirectFBConsole;
class DirectFBDrawable;

//////////////////////////////////////////////////////////////////////////////
// DirectFBDrawable declaration
//////////////////////////////////////////////////////////////////////////////

class DirectFBDrawable {
    friend class DirectFBConsole;
    friend class DrawableTie<DirectFBDrawable>;
public:
    DirectFBDrawable(const char *,
             IDirectFB *,
             DFBSurfaceDescription &);
    ~DirectFBDrawable();
    
    typedef unsigned int Pixel; // unsigned int should be 32 bit:-)
    
    Warsaw::Drawable::PixelFormat pixel_format() {
    Prague::Trace("DirectFBDrawable::pixel_format()");
    return m_format;
    }
    Warsaw::Drawable::BufferFormat buffer_format();
    Warsaw::PixelCoord width() const;
    Warsaw::PixelCoord height() const;
    Warsaw::PixelCoord vwidth() const;
    Warsaw::PixelCoord vheight() const;
    Warsaw::Coord resolution(Warsaw::Axis a) const;
    Warsaw::Coord dpi(Warsaw::Axis a) const;
    Warsaw::PixelCoord row_length() const;
    Pixel map(const Warsaw::Color &) const;
    DrawableTie<DirectFBDrawable>::Buffer * read_buffer() const;
    DrawableTie<DirectFBDrawable>::Buffer * write_buffer() const;
    /*
     * read one or more pixels from framebuffer
     */
    void read_pixel(Warsaw::PixelCoord, Warsaw::PixelCoord, Pixel &);
    void read_pixels(Warsaw::PixelCoord, Warsaw::PixelCoord, Warsaw::PixelCoord, Warsaw::PixelCoord, void *);
    /*
     * draw primitives with the current color (Pixel)
     */
    void set_color(Pixel);
    void draw_pixel(Warsaw::PixelCoord,
            Warsaw::PixelCoord);
    void draw_hline(Warsaw::PixelCoord,
            Warsaw::PixelCoord,
            Warsaw::PixelCoord);
    void draw_vline(Warsaw::PixelCoord,
            Warsaw::PixelCoord,
            Warsaw::PixelCoord);
    void draw_line(Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           Warsaw::PixelCoord);
    void draw_box(Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord);
    /*
     * draw primitives with the given color (Pixel)
     */
    void put_pixel(Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           Pixel);
    void put_hline(Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           void *);
    void put_vline(Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           void *);
    void draw_pixels(Warsaw::PixelCoord,
             Warsaw::PixelCoord,
             Warsaw::PixelCoord,
             Warsaw::PixelCoord,
             void *);
    /*
     * fast blits
     */
    void blit(Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord);
    void blit(const DirectFBDrawable &,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord);
    void blit(Warsaw::Drawable_ptr,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord,
          Warsaw::PixelCoord);
    
    void flush();
    void flush(Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           Warsaw::PixelCoord,
           Warsaw::PixelCoord);
    void init() {
    Prague::Trace("DirectFBConsole::init()");
    }
    void finish() {
    Prague::Trace("DirectFBConsole::finish()");
    }

    // DirectFB specific:
    IDirectFBSurface * surface() const { return m_surface; }
    IDirectFB        * dfb() const { return s_dfb; }
private:
    string             m_name;

    DrawableTie<DirectFBDrawable>::PixelFormat 
                       m_format;

    Pixel              m_cur_col;
    static IDirectFB * s_dfb;
    IDirectFBSurface * m_surface;

    unsigned int       m_height,
                       m_width;
};

// Change the Buffer to unlock the surface on release:
template<>
void DrawableTie<DirectFBDrawable>::Buffer_decl<DirectFBDrawable>::release_buffer() {
    IDirectFBSurface * surface = m_drawable->surface();
    surface->Unlock(surface);
}

//////////////////////////////////////////////////////////////////////////////
// DirectFBConsole declaration
//////////////////////////////////////////////////////////////////////////////

class DirectFBConsole {
    typedef DirectFBDrawable Drawable;
    typedef std::vector<DrawableTie<DirectFBDrawable> *> dlist_t;     
public:
    DirectFBConsole(int &, char **, PortableServer::POA_ptr);
    ~DirectFBConsole();
    static DrawableTie<Drawable> * drawable();
    static DrawableTie<Drawable> * create_drawable(Warsaw::PixelCoord,
                           Warsaw::PixelCoord,
                           Warsaw::PixelCoord);
    static DrawableTie<Drawable> * create_drawable(Drawable *);
    static Warsaw::Drawable_ptr activate_drawable(DrawableTie<DirectFBDrawable> *);
    static DrawableTie<Drawable> * reference_to_servant(Warsaw::Drawable_ptr);

    void device_info(std::ostream &);
    Warsaw::Input::Event *next_event();
    void wakeup();
    void activate_autoplay() {
    Prague::Trace("DirectFBConsole::activate_autoplay()");
    m_autoplay = true;
    }

private:
    Warsaw::Input::Event *synthesize(const DFBInputEvent &);

    static IDirectFB     * s_dfb;
    IDirectFBInputDevice * m_mouse;
    IDirectFBInputDevice * m_keyboard;
    IDirectFBInputBuffer * m_mouse_buf;

    long                   m_size[2];
    long                   m_position[2];
    double                 m_resolution[2];

    int                    m_wakeup_pipe[2];

    static dlist_t         s_drawables;

    IDirectFBSurface     * primary;
    
    bool   m_autoplay;
    static PortableServer::POA_var s_poa;
};


//////////////////////////////////////////////////////////////////////////////
// DirectFBDrawable inlines
//////////////////////////////////////////////////////////////////////////////
inline DrawableTie<DirectFBDrawable>::Buffer * DirectFBDrawable::read_buffer() const {
    Prague::Trace("DirectFBDrawable::read_buffer()");
    int pitch;
    void * src;
    m_surface->Lock(m_surface, DSLF_READ, &src, &pitch);
    return (new DrawableTie<DirectFBDrawable>::Buffer(this, static_cast<DrawableTie<DirectFBDrawable>::Buffer::data_type *>(src)));
}

inline DrawableTie<DirectFBDrawable>::Buffer * DirectFBDrawable::write_buffer() const { 
    Prague::Trace("DirectFBDrawable::write_buffer()");
    int pitch;
    void * src;
    m_surface->Lock(m_surface, DSLF_WRITE, &src, &pitch);
    return (new DrawableTie<DirectFBDrawable>::Buffer(this, static_cast<DrawableTie<DirectFBDrawable>::Buffer::data_type *>(src)));
}

inline Warsaw::PixelCoord DirectFBDrawable::width() const { 
    Prague::Trace("DirectFBDrawable::width()");
    return Warsaw::PixelCoord(m_width);
}

inline Warsaw::PixelCoord DirectFBDrawable::height() const { 
    Prague::Trace("DirectFBDrawable::height()");
    return Warsaw::PixelCoord(m_height);
}

inline Warsaw::PixelCoord DirectFBDrawable::vwidth() const { 
    Prague::Trace("DirectFBDrawable::vwidth()"); 
    return Warsaw::PixelCoord(m_width);
}

inline Warsaw::PixelCoord DirectFBDrawable::vheight() const {
    Prague::Trace("DirectFBDrawable::vheight()");
    return Warsaw::PixelCoord(m_height);
}

inline Warsaw::Coord DirectFBDrawable::resolution(Warsaw::Axis a) const {
    /*
     * mode.size is in mm
     * our base unit is 0.1 mm...
     * This assumes a 17" Screen:
     */
    Prague::Trace("DirectFBDrawable::resolution(...)");
    return a == Warsaw::xaxis ?
    3200.0 / double(m_width):
    2400.0 / double(m_height);
}

inline Warsaw::Coord DirectFBDrawable::dpi(Warsaw::Axis a) const {
    Prague::Trace("DirectFBDrawable::dpi(...)");
    return resolution(a) * 254.0;
}


inline Warsaw::PixelCoord DirectFBDrawable::row_length() const {
    // return bytes-per-row
    Prague::Trace("DirectFBDrawable::row_length()");
    return (m_width * (m_format.size >> 3));
}

inline DirectFBDrawable::Pixel
DirectFBDrawable::map(const Warsaw::Color &c) const {
    Prague::Trace("DirectFBDrawable::map(...)");
    return ((static_cast<unsigned char>(c.alpha * 0xFF) << 24) &
        (static_cast<unsigned char>(c.red   * 0xFF) << 16) &
        (static_cast<unsigned char>(c.green * 0xFF) <<  8) &
        (static_cast<unsigned char>(c.blue  * 0xFF)));
}

inline void DirectFBDrawable::set_color(Pixel c) {
    Prague::Trace("DirectFBDrawable::set_color(...)");
    m_surface->SetColor(m_surface,
                        static_cast<__u8>(c),
                        static_cast<__u8>(c >> 8),
                        static_cast<__u8>(c >> 16),
                        static_cast<__u8>(c >> 24));
    m_cur_col = c;
}

inline void DirectFBDrawable::draw_pixel(Warsaw::PixelCoord x,
                     Warsaw::PixelCoord y) {
    Prague::Trace("DirectFBDrawable::draw_pixel(...)");
    m_surface->DrawLine(m_surface, x, y, x, y);  
}

inline void DirectFBDrawable::draw_hline(Warsaw::PixelCoord x,
                     Warsaw::PixelCoord y,
                     Warsaw::PixelCoord w) {
    Prague::Trace("DirectFBDrawable::draw_hline(...)");
    m_surface->DrawLine(m_surface, x, y, x + w, y);
}

inline void DirectFBDrawable::draw_vline(Warsaw::PixelCoord x,
                     Warsaw::PixelCoord y,
                     Warsaw::PixelCoord h) {
    Prague::Trace("DirectFBDrawable::draw_vline(...)");
    m_surface->DrawLine(m_surface, x, y, x, y + h);  
}

inline void DirectFBDrawable::draw_line(Warsaw::PixelCoord x,
                    Warsaw::PixelCoord y,
                    Warsaw::PixelCoord w,
                    Warsaw::PixelCoord h) {
    Prague::Trace("DirectFBDrawable::draw_line(...)");
    m_surface->DrawLine(m_surface, x, y, x + w, y + h);
}

inline void DirectFBDrawable::draw_box(Warsaw::PixelCoord x,
                       Warsaw::PixelCoord y,
                       Warsaw::PixelCoord w,
                       Warsaw::PixelCoord h) {
    Prague::Trace("DirectFBDrawable::draw_box(...)");
    m_surface->FillRectangle(m_surface, x, y, w, h);
}

inline void DirectFBDrawable::put_pixel(Warsaw::PixelCoord x,
                    Warsaw::PixelCoord y,
                    Pixel c) {
    Prague::Trace("DirectFBDrawable::put_pixel(...)");
    Pixel old_col = m_cur_col;
    set_color(c);
    draw_pixel(x, y);
    set_color(m_cur_col);
}

inline void DirectFBDrawable::put_hline(Warsaw::PixelCoord x,
                    Warsaw::PixelCoord y,
                    Warsaw::PixelCoord w,
                    void * src) {
    Prague::Trace("DirectFBDrawable::put_hline(...)");
    IDirectFBSurface * surface;
    DFBSurfaceDescription desc;
    desc.pixelformat = DSPF_ARGB;
    desc.height = 1;
    desc.width = w;
    s_dfb->CreateSurface(s_dfb, &desc, &surface);
    void * dest;
    int pitch;
    surface->Lock(surface, DSLF_WRITE, &dest, &pitch);
    memcpy(src, dest, (w << 2));
    surface->Unlock(surface);
    surface->Release(surface);
}

inline void DirectFBDrawable::put_vline(Warsaw::PixelCoord x,
                    Warsaw::PixelCoord y,
                    Warsaw::PixelCoord h,
                    void * src) {
    Prague::Trace("DirectFBDrawable::put_vline(...)");;
    IDirectFBSurface * surface;
    DFBSurfaceDescription desc;
    desc.pixelformat = DSPF_ARGB;
    desc.height = h;
    desc.width = 1;
    s_dfb->CreateSurface(s_dfb, &desc, &surface);
    void * dest;
    int pitch;
    surface->Lock(surface, DSLF_WRITE, &dest, &pitch);
    memcpy(src, dest, (h << 2));
    surface->Unlock(surface);
    surface->Release(surface);
}

inline void DirectFBDrawable::draw_pixels(Warsaw::PixelCoord x,
                      Warsaw::PixelCoord y,
                      Warsaw::PixelCoord w,
                      Warsaw::PixelCoord h,
                      void * src) {
    Prague::Trace("DirectFBDrawable::draw_pixels(...)");
    IDirectFBSurface * surface;
    DFBSurfaceDescription desc;
    desc.pixelformat = DSPF_ARGB;
    desc.height = h;
    desc.width = w;
    s_dfb->CreateSurface(s_dfb, &desc, &surface);
    void * dest;
    int pitch;
    surface->Lock(surface, DSLF_WRITE, &dest, &pitch);
    memcpy(src, dest, ((h*w) << 2));
    surface->Unlock(surface);
    surface->Release(surface);
}

inline void DirectFBDrawable::blit(Warsaw::PixelCoord x1,
                   Warsaw::PixelCoord y1, 
                   Warsaw::PixelCoord w,
                   Warsaw::PixelCoord h, 
                   Warsaw::PixelCoord x2,
                   Warsaw::PixelCoord y2) {
    Prague::Trace("DirectFBDrawable::blit(PixelCoord, ...)");
    DFBRectangle rect;
    rect.x = x1; rect.y = y1; rect.w = w; rect.h = h;
    m_surface->Blit(m_surface, m_surface, &rect, x2, y2);
}
    
inline void DirectFBDrawable::blit(const DirectFBDrawable & src,
                   Warsaw::PixelCoord x1,
                   Warsaw::PixelCoord y1, 
                   Warsaw::PixelCoord w,
                   Warsaw::PixelCoord h,
                   Warsaw::PixelCoord x2,
                   Warsaw::PixelCoord y2) {
    Prague::Trace("DirectFBDrawable::blit(DirectFBDrawable &, ...)");
    DFBRectangle rect;
    rect.x = x1; rect.y = y1; rect.w = w; rect.h = h;
    m_surface->Blit(m_surface, src.surface(), &rect, x2, y2);
}
    
inline void DirectFBDrawable::blit(Warsaw::Drawable_ptr d, 
                   Warsaw::PixelCoord x1,
                   Warsaw::PixelCoord y1, 
                   Warsaw::PixelCoord w,
                   Warsaw::PixelCoord h, 
                   Warsaw::PixelCoord x2,
                   Warsaw::PixelCoord y2) {
    Prague::Trace("DirectFBDrawable::blit(Drawable_ptr, ...)");
    DrawableTie<DirectFBDrawable> *servant =
    DirectFBConsole::reference_to_servant(d);
    if (servant) blit(servant->impl(), x1, y1, w, h, x2, y2);
    else
    Logger::log(Logger::drawing)
        << "DirectFBDrawable::blit: unable to obtain servant from reference"
        << endl; 
}

inline void DirectFBDrawable::flush() {
    Prague::Trace("DirectFBDrawable::flush()");     
    // m_surface->Flip(m_surface, NULL, DFBSurfaceFlipFlags(0));
}

inline void DirectFBDrawable::flush(Warsaw::PixelCoord x,
                    Warsaw::PixelCoord y,
                    Warsaw::PixelCoord w,
                    Warsaw::PixelCoord h) {
    Prague::Trace("DirectFBDrawable::flush(PixelCoord, ...)");
    DFBRegion region;
    region.x1 = x; region.x2 = x + w; region.y1 = y; region.y2 = y + h;
    // m_surface->Flip(m_surface, &region, DSFLIP_WAITFORSYNC);
}


#endif
