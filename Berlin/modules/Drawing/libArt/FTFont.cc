/*$Id$
 *
 * This source file is a part of the Berlin Project.
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

#include "Berlin/Logger.hh"
#include <Drawing/libArt/LibArtFTFont.hh>
#include <Prague/Sys/Directory.hh>
#include <fstream>
#include <map>
#include <string>
#include <cstdlib>

#define FLOOR(x)  ((x) & -64)
#define CEIL(x)   (((x)+63) & -64)
#define TRUNC(x)  ((x) >> 6)
#define MAX(X,Y) (X>Y?X:Y)
#define MIN(X,Y) (X<Y?X:Y)

using namespace Prague;

bool LibArtFTFont::chooseFaceInteractively(const map<FamStyle,FT_Face> &faces, const char *env, 
					   Unicode::String &fam, Unicode::String &style)
{
  int idx = -1;
  if (env[0] == '\0')
    {
      cout << "list of available fonts :\n";
      unsigned int i = 0;
      for (map<FamStyle,FT_Face>::const_iterator j = faces.begin(); j != faces.end(); ++i, ++j)
	{
	  cout << i << ' ' << (*j).second->family_name << ' ' << (*j).second->style_name << endl;
	}
      cout << "please choose a number :"; cin >> idx;
    }
  else idx = atoi(env);
  map<FamStyle,FT_Face>::const_iterator j = faces.begin();
  for (unsigned int i = 0; i != idx && j != faces.end(); ++i, ++j);
  if (j == faces.end()) return false;
  fam = Unicode::String((*j).second->family_name);
  style = Unicode::String((*j).second->style_name);
  return true;
}

LibArtFTFont::LibArtFTFont(GGI::Drawable *drawable) :
//   xdpi(drawable->dpi(xaxis)),
//   ydpi(drawable->dpi(yaxis)),
  xdpi(96.),
  ydpi(96.),
  xres(drawable->resolution(xaxis)),
  yres(drawable->resolution(yaxis)),
  mySize(14),
  myGlyphCache(GlyphFactory(this,&myLibrary),256),
  myFaceMetricsCache(FaceMetricsFactory(this,&myLibrary),64),
  myGlyphMetricsCache(GlyphMetricsFactory(this,&myLibrary),256)
{
  if (FT_Init_FreeType( &myLibrary )) {
      cerr << "failed to open freetype library" << endl;
      exit(-1);    
  }
  char *env = getenv("BERLIN_ROOT");
  if (!env)
    {
      cerr << "Please set environment variable BERLIN_ROOT first" << endl;
      exit(-1);
    }
  string fname = string(env) + "/etc/fonts.txt";
  ifstream fontdirlist(fname.c_str());
  if (!fontdirlist)
    {
      Logger::log(Logger::text) << "cannot open " << fname << endl;
    }
  else
    {
      string fontdir;
      while (fontdirlist >> fontdir)
	{
	  Directory directory(fontdir, Directory::alpha);
	  Logger::log(Logger::text) << "LibArtFTFont: scanning font dir " << fontdir << endl;
	  for (Directory::iterator i = directory.begin(); i != directory.end(); i++)
	    {
	      if ((*i)->name() == "." || (*i)->name() == "..") continue;	  
	      string fontfile = (*i)->longName();
	      if (FT_New_Face(myLibrary, fontfile.c_str(), 0, &myFace))
		{
		  Logger::log(Logger::text) << "LibArtFTFont: can't open font " << fontfile << endl;
		  continue;
		}
	      myFamStr = Unicode::String(myFace->family_name);
	      myStyleStr = Unicode::String(myFace->style_name);
	      myFam = atomize(myFamStr);
	      myStyle = atomize(myStyleStr);
	      Logger::log(Logger::text) << "found FT-readable font "
					<< myFamStr << " (" << myFam << ") " << myStyleStr << " (" << myStyle << ") in "
					<< fontfile << endl;
	      myFaceMap[FamStyle(myFam,myStyle)] = myFace;
	    }
	}
      fontdirlist.close();
      Logger::log(Logger::text) << "completed scaning font directories" << endl;
      char *env = getenv("BERLIN_FONT_CHOOSER");
      cout << env << endl;
      Unicode::String tmpFam, tmpStyle;
      if (env && chooseFaceInteractively(myFaceMap, env, tmpFam, tmpStyle))
	{
	  myFamStr = tmpFam;
	  myFam = atomize(myFamStr);
	  myStyleStr = tmpStyle;
	  myStyle = atomize(myStyleStr);
	}
    }
}


LibArtFTFont::~LibArtFTFont() {}
unsigned long LibArtFTFont::size() { return mySize;}
unsigned long LibArtFTFont::weight() { return 100;}
void LibArtFTFont::size(unsigned long sz) { mySize = sz;}
void LibArtFTFont::weight(unsigned long wt) {}

Unistring *LibArtFTFont::family()
{ 
  return new Unistring (Unicode::toCORBA(Unicode::String(myFamStr)));
}

Unistring *LibArtFTFont::subfamily() { return 0;}
void LibArtFTFont::subfamily(const Unistring &fname) {}
void LibArtFTFont::fullname(const Unistring &fname) {}

void LibArtFTFont::family(const Unistring &fname)
{ 
  myFamStr = Unicode::toPrague(fname);
  myFam = atomize(myFamStr);
}

void LibArtFTFont::style(const Unistring &sname)
{ 
  myStyleStr = Unicode::toPrague(sname);
  myStyle = atomize(myStyleStr);
}

Unistring *LibArtFTFont::fullname()
{ 
  Unicode::String str = myFamStr;
  str += Unicode::Char(' ');
  str += myStyleStr;
  return new Unistring(Unicode::toCORBA(str));
}

Unistring *LibArtFTFont::style() { 
  return new Unistring
    (Unicode::toCORBA
     (Unicode::String(myStyleStr)));
}

DrawingKit::FontMetrics LibArtFTFont::metrics()
{
  DrawingKit::FontMetrics fm;
  FaceSpec key(mySize,FamStyle(myFam,myStyle));
  myFaceMetricsCache.get(key,fm);
  return fm;
}


DrawingKit::GlyphMetrics LibArtFTFont::metrics(Unichar &uc)
{
  DrawingKit::GlyphMetrics gm;
  GlyphSpec key(uc,FaceSpec(mySize,FamStyle(myFam,myStyle)));
  myGlyphMetricsCache.get(key,gm);
  return gm;
}

void LibArtFTFont::getPixBuf(const Unichar ch, ArtPixBuf *&pb) {
  GlyphSpec key(ch,FaceSpec(mySize,FamStyle(myFam,myStyle)));
  myGlyphCache.get(key,pb);
}


void LibArtFTFont::setup_face(FT_Face &f)
{
  FamStyle spec(myFam,myStyle);
  if (myFaceMap.find(spec) != myFaceMap.end()) f = myFaceMap[spec];
  else f = myFace;
}

void LibArtFTFont::setup_size(FT_Face &f) {
  FT_Set_Char_Size
    ( f,                // handle to face object           
      mySize << 6,     // char_width in 1/64th of points  
      mySize << 6,     // char_height in 1/64th of points 
      (unsigned int)xdpi,   // horizontal device resolution    
      (unsigned int)ydpi ); // vertical device resolution      
}

bool LibArtFTFont::load_glyph(Unichar c, FT_Face &f) {

  FT_CharMap  found = 0;
  FT_CharMap  charmap;  
  for (int n = 0; n < f->num_charmaps; n++ ){
      charmap = f->charmaps[n];
      if (charmap->encoding == ft_encoding_unicode) {	
	found = charmap;
	break;
      }
  }
  if (!found) { 
    // no way of translating!
    return false; 
  }

  /* now, select the charmap for the face object */
  if (FT_Set_Charmap( f, found )) return false;
  int idx = FT_Get_Char_Index(f,(unsigned char)c);
  if (FT_Load_Glyph (f,idx,0)) {
    return false;
  }
  return true;
}
  
static inline bool render_pixbuf(ArtPixBuf *pb, 
				  FT_GlyphSlot &glyph,
				  FT_Library &library) {
    
  // first, render the glyph into an intermediate buffer  
  if ( glyph->format != ft_glyph_format_outline ) return false;
  int size = pb->width * pb->height;
  memset(pb->pixels,0,size);      
  FT_Bitmap  bit;
  bit.width      = pb->width;
  bit.rows       = pb->height;
  bit.pitch      = pb->width;
  bit.pixel_mode = ft_pixel_mode_grays;
  bit.buffer     = pb->pixels;      

  int left  = FLOOR( glyph->metrics.horiBearingX );
  int bottom = FLOOR( glyph->metrics.horiBearingY - glyph->metrics.height );
  FT_Outline_Translate(&glyph->outline, -left, -bottom );    
  if (FT_Outline_Get_Bitmap( library, &glyph->outline, &bit)) return false;
  return true;
}


// void LibArtFTFont::getMetrics(const Unichar ch, FT_Glyph_Metrics &m) {
//   FT_Face newface;
//   setup_face(newface);
//   setup_size(newface);
//   FT_GlyphSlot glyph = newface->glyph;
//   if (!load_glyph(ch,newface)) return;       
//   m = glyph->metrics;
// }

LibArtFTFont::atom LibArtFTFont::Atomizer::atomize(
Unicode::String &u) {
  map<Unicode::String,atom>::iterator i;
  i = atomMap.find(u);
  if (i == atomMap.end()) {	
    atomMap[u] = ++currAtom;
    return currAtom;
  } else {
    return i->second;
  }
}

void LibArtFTFont::allocateChar(Unichar ch, Graphic::Requisition &r) {
  DrawingKit::GlyphMetrics gm = metrics(ch);

  r.x.natural = r.x.minimum = r.x.maximum = ((double)(gm.horiAdvance >> 6)) / xres; 
  r.x.defined = true;
  r.x.align = 0.;
  r.y.natural = r.y.minimum = r.y.maximum = ((double)(gm.height >> 6)) / yres;
  r.y.defined = true;
  r.y.align = (gm.height == 0) ? 0. : 
    ((double)gm.horiBearingY)/((double)gm.height); 

//   FT_Face newface;
//   setup_face(newface);
//   setup_size(newface);
//   FT_GlyphSlot glyph = newface->glyph;
//   if (!load_glyph(ch,newface)) return;
//   r.x.natural = r.x.minimum = r.x.maximum = ((double)(glyph->metrics.horiAdvance >> 6)) / xres;
//   r.x.defined = true;
//   r.x.align = 0.;
//   r.y.natural = r.y.minimum = r.y.maximum = ((double)(glyph->metrics.height >> 6)) / yres; 
//   r.y.defined = true;
//   r.y.align = (glyph->metrics.height == 0) ? 0. : 
//     ((double)glyph->metrics.horiBearingY)/((double)glyph->metrics.height); 
}


ArtPixBuf * 
LibArtFTFont::GlyphFactory::produce(const LibArtFTFont::GlyphSpec &gs)
{
  FT_Face newface;
  font_->setup_face(newface);
  font_->setup_size(newface);  
  Unichar ch = gs.first;
  DrawingKit::GlyphMetrics gm = font_->metrics(ch);
  int width = (int) (gm.width >> 6);
  int height = (int) (gm.height >> 6);
  art_u8 *pixels = new art_u8[width * height]; 
  // this is a lie -- we're going to use it as a greymap
  ArtPixBuf *pb = art_pixbuf_new_rgb (pixels, width, height, width);  
  font_->load_glyph(ch,newface);
  render_pixbuf(pb,newface->glyph,*lib_);      
  return pb;
}


DrawingKit::FontMetrics 
LibArtFTFont::FaceMetricsFactory::produce(const LibArtFTFont::FaceSpec &cs) 
{
  DrawingKit::FontMetrics fm;
  FT_Face newface;
  font_->setup_face(newface);
  font_->setup_size(newface);
  fm.ascender = newface->size->metrics.ascender;
  fm.descender = newface->size->metrics.descender;
  fm.height = newface->size->metrics.height;
  fm.max_advance = newface->size->metrics.max_advance;
  return fm;
}


DrawingKit::GlyphMetrics 
LibArtFTFont::GlyphMetricsFactory::produce(const LibArtFTFont::GlyphSpec &cs) 
{
  DrawingKit::GlyphMetrics gm;
  FT_Face newface;
  font_->setup_face(newface);
  font_->setup_size(newface);
  Unichar uc = cs.first;
  if (!font_->load_glyph(uc, newface)) return gm;
  FT_GlyphSlot glyph = newface->glyph;
  gm.width = glyph->metrics.width;
  gm.height = glyph->metrics.height;
  gm.horiBearingX = glyph->metrics.horiBearingX;
  gm.horiBearingY = glyph->metrics.horiBearingY;
  gm.horiAdvance =  glyph->metrics.horiAdvance;
  gm.vertBearingX = glyph->metrics.vertBearingX;
  gm.vertBearingY = glyph->metrics.vertBearingY;
  gm.vertAdvance = glyph->metrics.vertAdvance;
  return gm;
}

