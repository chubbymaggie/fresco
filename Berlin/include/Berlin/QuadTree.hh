/*$Id$
 *
 * This source file is a part of the Berlin Project.
 * Copyright (C) 1999 Stefan Seefeld <seefelds@magellan.umontreal.ca> 
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
#ifndef QuadTree_hh
#define QuadTree_hh

#include <Berlin/Geometry.hh>
#include <vector>
#include <algorithm>

/*
 * this defines a binary space partitioning (BSP) algorithm
 * in 2D, suitable for object lookup on the screen
 *
 * the parametrization is as follows:
 *   - T is the Domain, most likely Coord or PixelCoord
 *   - I is the Item to store, can be anything which defines a method
 *     const Geometry::Rectangle<T> &I::bbox() returning the bounding box
 *     suitable for use in the quadtree
 */

template <class T, class I>
class QTNode
{
  class move_down;
  friend class move_down;
public:
  enum index
  {
    fence = -1,
    none = 0,
    left = 0,
    right = 1,
    bottom = 0,
    top = 1,
    leftbottom = left|bottom,
    rightbottom = right|bottom,
    lefttop = left|top,
    righttop = right|top
  };
  typedef vector<I *> list;
  QTNode(const Geometry::Rectangle<T> &r)
    : region(r), elements(0) { quadrants[0] = quadrants[1] = quadrants[2] = quadrants[3] = 0;}
  ~QTNode() { free();}
  QTNode<T, I> *node(index i) { return quadrants[i];}
  const Geometry::Rectangle<T> &extent() const { return region;}
  const Geometry::Rectangle<T> &bbox() const { return boundingbox;}
  void insert(I *);
  void remove(I *);
  int  size() const { return elements;}
  void unfold();
  void collaps();
  void adjust(int, int, T, T);
protected:
  bool leaf() { return quadrants[0] == 0;}
  void allocate();
  void free() { if (!leaf()) for (int i = 0; i < 4; i++) { delete quadrants[i]; quadrants[i] = 0;}}
  index where(const Geometry::Point<T> &p)
    { return index ((p.x <= region.cx() ? left : right) | (p.y <= region.cy() ? bottom : top));}
  index where(const Geometry::Rectangle<T> &);
  index where(I *i) { return where(i->bbox());}

  Geometry::Rectangle<T> region;
  Geometry::Rectangle<T> boundingbox;
  int          elements;
  list         items;
  QTNode<T, I> *quadrants[4];
};

template <class T, class I>
class QuadTree
{
public:
  QuadTree() {}
  ~QuadTree() {}

  Geometry::Rectangle<T> bbox() { return node() ? node()->bbox() : Geometry::Rectangle<T>();}
  int size() { return quad ? quad->size() : 0;}
  QTNode<T, I> *node() { return quad;}
protected:
  QTNode<T, I> *quad;
};

template <class T, class I>
void QTNode<T, I>::allocate()
{
  using namespace Geometry;
  if (leaf())
    {
      quadrants[leftbottom] = new QTNode<T, I>(Rectangle<T>(region.l, region.b, region.cx(), region.cy()));
      quadrants[rightbottom] = new QTNode<T, I>(Rectangle<T>(region.cx(), region.b, region.r, region.cy()));
      quadrants[lefttop] = new QTNode<T, I>(Rectangle<T>(region.l, region.cy(), region.cx(), region.t));
      quadrants[righttop] = new QTNode<T, I>(Rectangle<T>(region.cx(), region.cy(), region.r, region.t));
    }
}

/*
 * max is the maximum number of objects desired at each node
 * min is the minimum number of objects desired at each node
 * min_w and min_h are the minimum span desired for a node
 */
template <class T, class I>
inline void QTNode<T, I>::adjust(int min, int max, T min_w, T min_h)
{
  /*
   * If we are over the maximum then unfold
   * and push down as many objects as we can.
   */
  if (items.size() > (uint) max && ((region.w() > (min_w * 2)) || (region.h() > (min_h * 2)))) unfold();
  if (!leaf())
    {
      for (int i = 0; i < 4; ++i) quadrants[i]->adjust(min, max, min_w, min_h);
      /*
       * If our sub-tree is under the minimum then flatten it away.
       */
      if (elements < min || elements - items.size() == 0 || (region.w() < min_w && region.h() < min_h)) collaps();
    }
}

template <class T, class I>
class QTNode<T, I>::move_down : public unary_function<I *, bool>
{
public:
  move_down(QTNode<T, I> *n) : node(n) {}
  bool operator()(I *i)
    {
      QTNode<T, I>::index idx = node->where(i);
      if (idx != QTNode<T, I>::fence) { node->quadrants[idx]->insert(i); return true;}
      else return false;
    };
  QTNode<T, I> *node;
};

template <class T, class I>
inline void QTNode<T, I>::unfold()
{
  if (! leaf()) return;
  allocate();
  list::iterator i = remove_if(items.begin(), items.end(), move_down(this));
  items.erase(i, items.end());
}

template <class T, class I>
inline void QTNode<T, I>::collaps()
{
  if (leaf()) return;
  for (int i = 0; i < 4; i++)
    {
      QTNode<T, I> *node = quadrants[i];
      node->collaps();
      list childItems = node->items;
      items.insert(items.end(), childItems.begin(), childItems.end());
    }
  free();
}

template <class T, class I>
inline QTNode<T, I>::index QTNode<T, I>::where(const Geometry::Rectangle<T> &r)
{
  int idx = fence;
  if (!leaf())
    {
      const T x = region.cx();
      const T y = region.cy();
      if ((r.r <= x) == (r.l < x) && (r.t <= y) == (r.b < y))
	idx = (r.r <= x ? left : right) | (r.t <= y ? bottom : top);
    }
  return static_cast<index>(idx);
}

template <class T, class I>
inline void QTNode<T, I>::insert(I *i)
{
  index idx = where(i);
  if (idx == fence) items.push_back(i);
  else quadrants[idx]->insert(i);
  elements++;

  const Geometry::Rectangle<T> &bb = i->bbox();
  if (elements > 1) boundingbox.merge(bb);
  else boundingbox = bb;
}

template <class T, class I>
inline void QTNode<T, I>::remove(I *i)
{
  index idx = where(i);
  if (idx == fence)
    {
      for (list::iterator j = items.begin(); j != items.end(); j++)
	if (*j == i)
	  {
	    items.erase(j);
	    break;
	  }
    }
  else quadrants[idx]->remove(i);
  elements--;

  if (boundingbox.touches(i->bbox(), epsilon))
    {
      boundingbox.clear();
      bool first = true;
      for (list::iterator j = items.begin(); j != items.end(); j++)
	{
	  if (first) { boundingbox = (*j)->bbox(); first = false;}
	  else boundingbox.merge((*j)->bbox());
	}
      if (!leaf())
	{
	  for (int j = 0; j < 4; j++)
	    {
	      QTNode<T, I> *node = quadrants[j];
	      if (node->elements > 0)
		{
		  if (first) { boundingbox = node->bbox(); first = 0;}
		  else boundingbox.merge(node->bbox());
		}
	    }
	}
    }
}

#endif /* _QuadTree_h */