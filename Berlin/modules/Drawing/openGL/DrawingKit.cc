/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999, 2000 Stefan Seefeld <stefan@fresco.org> 
 * Copyright (C) 1999 Graydon Hoare <graydon@fresco.org> 
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

#include <Prague/Sys/Profiler.hh>
#include <Fresco/config.hh>
#include <Fresco/Transform.hh>
#include <Fresco/IO.hh>
#include <Berlin/Logger.hh>
#include <Berlin/Color.hh>
#include "Quadric.hh"
#include "DrawingKit.hh"
#include "FTFont.hh"
#include <iostream>

using namespace Prague;
using namespace Fresco;

class openGL::DrawingKit::Light::Init : public virtual GLContext::Callback {
public:
  Init::Init(int *max)
    : my_max(max) {}
  void operator()() {
    glGetIntegerv(GL_MAX_LIGHTS, my_max);  
    delete this;
  }
private:
  int *my_max;
};

openGL::DrawingKit::Light::Light(GLContext *glcontext)
  : _max(-1), _number(-1), my_glcontext(glcontext)
{
  my_glcontext->add_to_queue(new Init(&_max));
}

int openGL::DrawingKit::Light::push()
{
  ++_number;
  if (_number >= _max) return -1;
  glEnable(static_cast<GLenum>(GL_LIGHT0 + _number));
  return GL_LIGHT0 + _number;
}

int openGL::DrawingKit::Light::top() const
{
  return GL_LIGHT0 + _number;
}

void openGL::DrawingKit::Light::pop()
{
  if (_number < _max) glDisable(static_cast<GLenum>(GL_LIGHT0 + _number));
  --_number;
}

openGL::DrawingKit::DrawingKit(const std::string &id, const Fresco::Kit::PropertySeq &p)
  : KitImpl(id, p),
    _drawable(0),
    _tx(0),
    _font(0),
    _light(0),
    _textures(100),
    _images(500)
{
}

KitImpl *openGL::DrawingKit::clone(const Fresco::Kit::PropertySeq &p)
{
  openGL::DrawingKit *kit = new openGL::DrawingKit(repo_id(), p);
  kit->init();
  return kit;
}

void openGL::DrawingKit::save()
{
  DrawingKitBase::save();
  _states.push(DrawState());
  _states.top().lights = _light->top();
}

void openGL::DrawingKit::restore()
{
  DrawingKitBase::restore();
  if (_states.empty()) return; // no state to restore
  DrawState &prev = _states.top();
  while (_light->top() > prev.lights) _light->pop();
  _states.pop();
}

void openGL::DrawingKit::init()
{
  Console *console = Console::instance();
  _drawable = console->drawable();
  my_glcontext = console->get_extension<GLContext>("GLContext");

  _font = new FTFont(my_glcontext);
  _light = new Light(my_glcontext);

  // XXX we should have our own initializer, but it turns out that
  // the Console's mouse pointer doesn't work without executing this
  // code. The two need to be decoupled. Using window_pos extension
  // for the mouse pointer will do that, but I don't yet know if I
  // want to make OpenGL 1.4 support manditory.
#if 0
  glViewport(0, 0, _drawable->width(), _drawable->height());
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  glOrtho(0, _drawable->width()/_drawable->resolution(xaxis), _drawable->height()/_drawable->resolution(yaxis), 0, -5000.0, 5000.0); 
  glTranslatef(0.375, 0.375, 0.);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glShadeModel(GL_SMOOTH);
//   glEnable(GL_LIGHTING);
  glFrontFace(GL_CW);
  glDisable(GL_DITHER);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
}

openGL::DrawingKit::~DrawingKit()
{
  delete _font;
  delete my_glcontext;
}

class openGL::DrawingKit::ResetTrafo : public virtual GLContext::Callback {
public:
  void operator()() {
    glLoadIdentity();
    delete this;
  }
};


class openGL::DrawingKit::SetTrafo : public virtual GLContext::Callback {
public:
  SetTrafo::SetTrafo(const Transform::Matrix &matrix)
  {
    // XXX There exists a GL extension to load transposed matrices.
    // But, we must take a copy anyways.
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        glmatrix[i*4+j] = matrix[j][i];
  }
  void operator()() {
    glLoadMatrixd(glmatrix);
    delete this;
  }
private:
  GLdouble glmatrix[16];
};

void openGL::DrawingKit::set_transformation(Transform_ptr t)
{
  if (CORBA::is_nil(t))
    {
      _tr->load_identity();
      my_glcontext->add_to_queue(new ResetTrafo());
    }
  else
    {
      _tr = Transform::_duplicate(t);

      if (_tr->identity())
        my_glcontext->add_to_queue(new ResetTrafo());
      else {
        Transform::Matrix matrix;
        _tr->store_matrix(matrix);
        my_glcontext->add_to_queue(new SetTrafo(matrix));
      }
    }
}

class openGL::DrawingKit::SetClipping : public virtual GLContext::Callback {
public:
  SetClipping::SetClipping(Fresco::PixelCoord x, Fresco::PixelCoord y,
                           Fresco::PixelCoord w, Fresco::PixelCoord h)
    : my_x(x), my_y(y), my_width(w), my_height(h) {}
  void operator()() {
    glScissor(my_x, my_y, my_width, my_height);
    delete this;
  }
private:
  Fresco::PixelCoord my_x, my_y;
  Fresco::PixelCoord my_width, my_height;
};

void openGL::DrawingKit::set_clipping(Region_ptr r)
{
  _cl = Region::_duplicate(r);
  if (CORBA::is_nil(_cl)) my_glcontext->add_to_queue(new SetClipping(0, 0, _drawable->width(), _drawable->height()));
  else
    {
      Vertex lower, upper;
      _cl->bounds(lower, upper);
      PixelCoord x = static_cast<PixelCoord>(lower.x*_drawable->resolution(xaxis) + 0.5);
      PixelCoord y = static_cast<PixelCoord>((_drawable->height()/_drawable->resolution(yaxis) - upper.y)*_drawable->resolution(yaxis) + 0.5);
      PixelCoord w = static_cast<PixelCoord>((upper.x - lower.x)*_drawable->resolution(xaxis) + 0.5);
      PixelCoord h = static_cast<PixelCoord>((upper.y - lower.y)*_drawable->resolution(yaxis) + 0.5);
      my_glcontext->add_to_queue(new SetClipping(x, y, w, h));
    }
}

class openGL::DrawingKit::SetColor : public virtual GLContext::Callback {
public:
  SetColor::SetColor(const double red, const double green, const double blue,
                     const double alpha)
    : my_red(red), my_green(green), my_blue(blue), my_alpha(alpha) {}
  void operator()() {
    glColor4d(my_red, my_green, my_blue, my_alpha);
    delete this;
  }
private:
  GLdouble my_red, my_blue, my_green, my_alpha;
};

void openGL::DrawingKit::set_foreground(const Color &c)
{
  _fg = c;
  my_glcontext->add_to_queue(new SetColor(_lt.red * _fg.red, _lt.green * _fg.green, _lt.blue * _fg.blue, _fg.alpha));
}

void openGL::DrawingKit::set_lighting(const Color &c)
{
  _lt = c;
  my_glcontext->add_to_queue(new SetColor(_lt.red * _fg.red, _lt.green * _fg.green, _lt.blue * _fg.blue, _fg.alpha));
}

class openGL::DrawingKit::SetPointSize : public virtual GLContext::Callback {
public:
  SetPointSize::SetPointSize(const Coord s)
    : my_size(s) {}
  void operator()() {
    // XXX FIXME !: glPointSize uses pixel units !
    glPointSize(my_size);
    delete this;
  }
private:
  Coord my_size;
};

void openGL::DrawingKit::set_point_size(Coord s)
{
  _ps = s;
  my_glcontext->add_to_queue(new SetPointSize(_ps));
}

class openGL::DrawingKit::SetLineWidth : public virtual GLContext::Callback {
public:
  SetLineWidth::SetLineWidth(const Coord s)
    : my_size(s) {}
  void operator()() {
    // XXX FIXME !: glLineWidth uses pixel units !
    glLineWidth(my_size);
    delete this;
  }
private:
  Coord my_size;
};

void openGL::DrawingKit::set_line_width(Coord w)
{
  _lw = w;
  my_glcontext->add_to_queue(new SetLineWidth(_lw));
}

void openGL::DrawingKit::set_line_endstyle(Fresco::DrawingKit::Endstyle style)
{
  _es = style;
  // XXX NYI (btw, this is a very very hard thing to do.)
}

class openGL::DrawingKit::SetTexture : public virtual GLContext::Callback {
public:
  SetTexture::SetTexture(const bool on)
    : my_on(on) {}
  void operator()() {
    if (my_on) glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);
    delete this;
  }
private:
  bool my_on;
};

class openGL::DrawingKit::SetOutline : public virtual GLContext::Callback {
public:
  SetOutline::SetOutline(const bool on)
    : my_on(on) {}
  void operator()() {
    if (my_on) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    delete this;
  }
private:
  bool my_on;
};

void openGL::DrawingKit::set_surface_fillstyle(Fresco::DrawingKit::Fillstyle style)
{
  Fresco::DrawingKit::Fillstyle old_fs = _fs;
  _fs = style;

  if ((_fs==Fresco::DrawingKit::textured)
      != (old_fs==Fresco::DrawingKit::textured))
    my_glcontext->add_to_queue(new SetTexture(_fs==Fresco::DrawingKit::textured));

  if ((_fs==Fresco::DrawingKit::outlined)
      != (old_fs==Fresco::DrawingKit::outlined))
    my_glcontext->add_to_queue(new SetOutline(_fs==Fresco::DrawingKit::outlined));
}

void openGL::DrawingKit::set_texture(Raster_ptr t)
{
  _tx = CORBA::is_nil(t) ? 0 : _textures.lookup(Fresco::Raster::_duplicate(t));
  if (_tx) _tx->activate(my_glcontext);
}

class openGL::DrawingKit::DrawPath : public virtual GLContext::Callback {
public:
  DrawPath::DrawPath(const Fresco::Path &path)
    : my_path(path)
  {}
  void operator()() {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    if (my_path.shape == convex) {
      glBegin(GL_POLYGON);
      for (CORBA::ULong i = 0; i < my_path.nodes.length(); i++)
        glVertex3f(my_path.nodes[i].x, my_path.nodes[i].y, 0);
      glEnd();
    } else {
      GLUtesselator *tesselator = gluNewTess();
      gluTessProperty(tesselator, GLU_TESS_WINDING_RULE,
                      GLU_TESS_WINDING_NONZERO);
      gluTessCallback(tesselator, GLU_TESS_VERTEX,
                      (GLvoid (*) ( )) &glVertex3dv);
      gluTessCallback(tesselator, GLU_TESS_BEGIN,
                      (GLvoid (*) ( )) &glBegin);
      gluTessCallback(tesselator, GLU_TESS_END,
                      (GLvoid (*) ( )) &glEnd);
      // XXX need glu tess combine callback for truly complex stuff.
      gluTessBeginPolygon(tesselator, NULL);
      gluTessBeginContour(tesselator);
      for (CORBA::ULong i = 0; i < my_path.nodes.length(); i++)
      {
        gluTessVertex(tesselator, (GLdouble *)&my_path.nodes[i], (void *)&my_path.nodes[i]);
      }
      gluTessEndContour(tesselator);
      gluTessEndPolygon(tesselator);
      gluDeleteTess(tesselator);
    }
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    delete this;
  }
private:
  Fresco::Path my_path; 
};


void openGL::DrawingKit::draw_path(const Fresco::Path &path)
{
  // XXX We should be doing the gluTesselation here.
  my_glcontext->add_to_queue(new DrawPath(path));
}

void openGL::DrawingKit::draw_rectangle(const Vertex &lower, const Vertex &upper)
{
  Fresco::Path path;
  path.shape = convex;
  path.nodes.length(4);
#if 1 /* CCW */
  path.nodes[0].x = path.nodes[3].x = lower.x;
  path.nodes[1].x = path.nodes[2].x = upper.x;
  path.nodes[0].y = path.nodes[1].y = lower.y;
  path.nodes[2].y = path.nodes[3].y = upper.y;
#else /* CW */
  path.nodes[0].x = path.nodes[1].x = lower.x;
  path.nodes[3].x = path.nodes[2].x = upper.x;
  path.nodes[0].y = path.nodes[3].y = lower.y;
  path.nodes[2].y = path.nodes[1].y = upper.y;
#endif
  my_glcontext->add_to_queue(new DrawPath(path));
}

void openGL::DrawingKit::draw_quadric(const Fresco::DrawingKit::Quadric, Fresco::Coord, Fresco::Coord)
{
}

// XXX TODO what?
void openGL::DrawingKit::draw_ellipse(const Vertex &lower, const Vertex &upper)
{
#if 0
  glPushMatrix();
  glScaled(upper.x - lower.x, upper.y - lower.y, upper.z - lower.z);
  glTranslated(lower.x, lower.y, lower.z);
  Quadric quadric(_fs, Quadric::out);
  quadric.disk(0., 1., 360, 100);
  glPopMatrix();
#endif
}

void openGL::DrawingKit::draw_image(Raster_ptr raster)
{
  Profiler prf("openGL::DrawingKit::draw_image");
  Image *image = _images.lookup(Fresco::Raster::_duplicate(raster));
  Fresco::Path path;
  path.nodes.length(4);
  path.shape = convex;
  Coord width = image->width * 10;// /_drawable->resolution(xaxis);
  Coord height = image->height * 10;// /_drawable->resolution(yaxis);
  path.nodes[0].x = path.nodes[0].y = path.nodes[0].z = 0.;
  path.nodes[1].x = width, path.nodes[1].y = path.nodes[1].z = 0.;
  path.nodes[2].x = width, path.nodes[2].y = height, path.nodes[2].z = 0.;
  path.nodes[3].x = 0, path.nodes[3].y = height, path.nodes[3].z = 0.;

  my_glcontext->add_to_queue(new SetColor(_lt.red, _lt.green, _lt.blue, 1));
  my_glcontext->add_to_queue(new SetTexture(true));
  my_glcontext->add_to_queue(new SetOutline(false));
  image->activate(my_glcontext);
  my_glcontext->add_to_queue(new DrawPath(path));
  if (_tx) _tx->activate(my_glcontext);
  my_glcontext->add_to_queue(new SetTexture(_fs==Fresco::DrawingKit::textured));
  my_glcontext->add_to_queue(new SetOutline(_fs==Fresco::DrawingKit::outlined));
  my_glcontext->add_to_queue(new SetColor(_lt.red * _fg.red, _lt.green * _fg.green, _lt.blue * _fg.blue, _fg.alpha));
}

void openGL::DrawingKit::set_font_size(CORBA::ULong s) { _font->size(s); }
void openGL::DrawingKit::set_font_weight(CORBA::ULong w) {}
void openGL::DrawingKit::set_font_family(const Unistring &f) {}
void openGL::DrawingKit::set_font_subfamily(const Unistring &sf) {}
void openGL::DrawingKit::set_font_fullname(const Unistring &fn) {}
void openGL::DrawingKit::set_font_style(const Unistring &s) {}
void openGL::DrawingKit::set_font_attribute(const NVPair & nvp) {}

class openGL::DrawingKit::DirectionalLight : public virtual GLContext::Callback
{
public:
  DirectionalLight::DirectionalLight(const Fresco::Color color,
                                     CORBA::Float intensity,
                                     const Fresco::Vertex direction,
                                     Light *light)
    : my_color(color), my_intensity(intensity), my_direction(direction),
      my_light(light)
  {}
  void operator()() {
    int id = my_light->push();
    if (id < GL_LIGHT0) { delete this; return; }

    // RGBA intensities of source are the product of the color and
    // intensity, with 1.0 alpha
    GLfloat black[] = {0., 0., 0., 1.};
    GLfloat light[] = {my_color.red * my_intensity, my_color.green * my_intensity, my_color.blue * my_intensity, 1.};
    glLightfv(static_cast<GLenum>(id), GL_AMBIENT, black);
    glLightfv(static_cast<GLenum>(id), GL_DIFFUSE,  light);
    glLightfv(static_cast<GLenum>(id), GL_SPECULAR, light);

    // "Position" is the direction vector negated with a 0.0 w
    // component. Yet another GL peccadillo.
    GLfloat dir[] = {-my_direction.x, -my_direction.y, -my_direction.z, 0.};
    glLightfv(static_cast<GLenum>(id), GL_POSITION, dir);

    // Make sure no spotlight stuff is on
    glLightf(static_cast<GLenum>(id), GL_SPOT_EXPONENT, 0.0);
    glLightf(static_cast<GLenum>(id), GL_SPOT_CUTOFF, 180.0);

    // Attenuation does not matter for directional sources.
  }
private:
  const Fresco::Color my_color;
  CORBA::Float my_intensity;
  const Fresco::Vertex my_direction;
  Light *my_light;
};

void openGL::DrawingKit::directional_light(const Color &color,
                                     CORBA::Float intensity,
                                     const Vertex &direction)
{
  my_glcontext->add_to_queue(new DirectionalLight(color, intensity, direction,
                                                  _light));
}


class openGL::DrawingKit::PointLight : public virtual GLContext::Callback
{
public:
  PointLight::PointLight(const Fresco::Color color, CORBA::Float intensity,
                         const Fresco::Vertex position, Light *light)
    : my_color(color), my_intensity(intensity), my_position(position),
      my_light(light)
  {}
  void operator()() {
    int id = my_light->push();
    if (id < GL_LIGHT0) { delete this; return;}

    // RGBA intensities of source are the product of the color and
    // intensity, with 1.0 alpha
    GLfloat black[] = {0., 0., 0., 1.};
    GLfloat light[] = {my_color.red * my_intensity, my_color.green * my_intensity, my_color.blue * my_intensity, 1.};
    glLightfv(static_cast<GLenum>(id), GL_AMBIENT, black);
    glLightfv(static_cast<GLenum>(id), GL_DIFFUSE,  light);
    glLightfv(static_cast<GLenum>(id), GL_SPECULAR, light);

    // Set position
    GLfloat dir[] = {my_position.x, my_position.y, my_position.z, 1.0};
    glLightfv(static_cast<GLenum>(id), GL_POSITION, dir);

    // Make sure no spotlight stuff is on
    glLightf(static_cast<GLenum>(id), GL_SPOT_EXPONENT, 0.0);
    glLightf(static_cast<GLenum>(id), GL_SPOT_CUTOFF, 180.0);

    // Attenuation is accessed from the state
    //   const SbVec3f &atten = SoLightAttenuationElement::get(action->getState());
    //   glLightf(static_cast<GLenum>(id), GL_CONSTANT_ATTENUATION,  atten[2]);
    //   glLightf(static_cast<GLenum>(id), GL_LINEAR_ATTENUATION,    atten[1]);
    //   glLightf(static_cast<GLenum>(id), GL_QUADRATIC_ATTENUATION, atten[0]);
  }
private:
  const Fresco::Color my_color;
  CORBA::Float my_intensity;
  const Fresco::Vertex my_position;
  Light *my_light;
};


void openGL::DrawingKit::point_light(const Fresco::Color &color,
                               CORBA::Float intensity,
                               const Fresco::Vertex &position)
{
  my_glcontext->add_to_queue(new PointLight(color, intensity, position,
                                            _light));
}


class openGL::DrawingKit::SpotLight : public virtual GLContext::Callback
{
public:
  SpotLight::SpotLight(const Color color, CORBA::Float intensity,
                       const Vertex position, const Vertex direction,
                       CORBA::Float dropoffrate, CORBA::Float cutoffangle,
                       Light *light)
    : my_color(color), my_intensity(intensity), my_position(position),
      my_direction(direction), my_dropoffrate(dropoffrate),
      my_cutoffangle(cutoffangle), my_light(light)
  {}
  void operator()()
  {
    int id = my_light->push();
    if (id < GL_LIGHT0) { delete this; return; }

    // RGBA intensities of source are the product of the color and
    // intensity, with 1.0 alpha
    GLfloat black[] = {0., 0., 0., 1.};
    GLfloat light[] = {my_color.red * my_intensity, my_color.green * my_intensity, my_color.blue * my_intensity, 1.};
    glLightfv(static_cast<GLenum>(id), GL_AMBIENT, black);
    glLightfv(static_cast<GLenum>(id), GL_DIFFUSE,  light);
    glLightfv(static_cast<GLenum>(id), GL_SPECULAR, light);

    // Set position
    GLfloat pos[] = {my_position.x, my_position.y, my_position.z, 1.0};
    glLightfv(static_cast<GLenum>(id), GL_POSITION, pos);
    
    // Set up spotlight stuff. Note that the GL angle must be specified
    // in degrees, though the field is in radians
    GLfloat dir[] = {my_direction.x, my_direction.y, my_direction.z, 1.0};
    glLightfv(static_cast<GLenum>(id), GL_SPOT_DIRECTION, dir);
    
    if (my_dropoffrate <= 0.0)
      glLightf(static_cast<GLenum>(id), GL_SPOT_EXPONENT, .01);
    else 
      glLightf(static_cast<GLenum>(id), GL_SPOT_EXPONENT,
               my_dropoffrate * 128.0);
    glLightf(static_cast<GLenum>(id), GL_SPOT_CUTOFF,
             my_cutoffangle*(180.0/M_PI));
    
    // Attenuation is accessed from the state
  //   const SbVec3f &atten = SoLightAttenuationElement::get(action->getState());
  //   glLightf(static_cast<GLenum>(id), GL_CONSTANT_ATTENUATION,  atten[2]);
  //   glLightf(static_cast<GLenum>(id), GL_LINEAR_ATTENUATION,    atten[1]);
  //   glLightf(static_cast<GLenum>(id), GL_QUADRATIC_ATTENUATION, atten[0]);
  }
private:
  Color my_color;
  CORBA::Float my_intensity;
  Vertex my_position;
  Vertex my_direction;
  CORBA::Float my_dropoffrate;
  CORBA::Float my_cutoffangle;
  Light *my_light;
};

void openGL::DrawingKit::spot_light(const Color &color,
                              CORBA::Float intensity,
                              const Vertex &position,
                              const Vertex &direction,
                              CORBA::Float dropoffrate,
                              CORBA::Float cutoffangle)
{
  my_glcontext->add_to_queue(new SpotLight(color, intensity, position,
                                           direction, dropoffrate,
                                           cutoffangle, _light));
};



void openGL::DrawingKit::allocate_text(const Unistring &s, Graphic::Requisition &req) {}
void openGL::DrawingKit::draw_text(const Unistring &us) {}
void openGL::DrawingKit::allocate_char(Unichar c, Graphic::Requisition &req) { _font->allocate_char(c, req);}
void openGL::DrawingKit::draw_char(Unichar c)
{
  _font->set_transform(_tr);
  _font->draw_char(c);
}

class openGL::DrawingKit::DrawMesh : public virtual GLContext::Callback {
public:
  DrawMesh::DrawMesh(const Fresco::Mesh &mesh)
    : my_mesh(mesh)
  {} // XXX we need to painters' algorithm sort here.
  void operator()() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    bool normals = my_mesh.normals.length() == my_mesh.triangles.length();
    glBegin(GL_TRIANGLES);
    for (CORBA::ULong i = 0; i < my_mesh.triangles.length(); ++i)
      {
        if (normals)
          {
            const Vertex &n = my_mesh.normals[i];
            glNormal3f(n.x, n.y, n.z);
          }
        const Vertex &a = my_mesh.nodes[my_mesh.triangles[i].a];
        const Vertex &b = my_mesh.nodes[my_mesh.triangles[i].b];
        const Vertex &c = my_mesh.nodes[my_mesh.triangles[i].c];
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
      }
    glEnd();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    delete this;
  }
private:
  Fresco::Mesh my_mesh; 
};


void openGL::DrawingKit::draw_mesh(const Fresco::Mesh &mesh)
{
  my_glcontext->add_to_queue(new DrawMesh(mesh));
}

void openGL::DrawingKit::copy_drawable(Drawable_ptr d, PixelCoord x, PixelCoord y, PixelCoord w, PixelCoord h) {}

extern "C" KitImpl *load()
{
  static std::string properties[] = {"implementation", "GLDrawingKit"};
  return create_kit<openGL::DrawingKit> ("IDL:fresco.org/Fresco/DrawingKit3D:1.0", properties, 2);
}

