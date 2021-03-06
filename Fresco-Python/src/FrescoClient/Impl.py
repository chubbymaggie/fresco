#
# This source file is a part of the Fresco Project.
# Copyright (C) 2000 by Stephen Davies
# http://www.fresco.org
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
# MA 02139, USA.
#
# Tabstop is 8, SoftTabStop is 4, ShiftWidth is 4 (as per last line)
#
"""
This module contains Python implementations of various Fresco interfaces. This
allows you to implement, for example, Graphics or Controllers and embed them
in the remove display. Of course, its really slow, but also really useful for
prototyping purposes!
"""
import math, string, traceback

# Import CORBA module
import omniORB
from omniORB import CORBA, PortableServer

# Import the Fresco stubs
import Fresco
import Fresco__POA

true, false = 1, 0

# Fake Babylon keys
class Babylon:
    KEY_CURSOR_UP = 0xE032
    KEY_CURSOR_DOWN = 0xE033
    KEY_CURSOR_LEFT = 0xE034
    KEY_CURSOR_RIGHT = 0xE035
    UC_HORIZONTAL_TABULATION = 0x0009

def min(a,b):
    if a < b: return a
    return b

def max(a,b):
    if a > b: return a
    return b

# ---------------
# -- Python base classes

class Identifiable (Fresco__POA.Identifiable):
    def is_identical(self, other):
	try:
	    me_id = self._default_POA().servant_to_id(self)
	    other_id = self._default_POA().reference_to_id(other)
	    return me_id == other_id
	except:
	    print "An error occurred in Identifiable.is_identical():"
	    traceback.print_exc()
	    return 0

class RefCountBase (Fresco__POA.RefCountBase):
    def __init__(self):
	self.__refcount = 0
    def increment(self):
	#print "RefCountBase.increment()"
	self.__refcount = self.__refcount + 1
    def decrement(self):
	self.__refcount = self.__refcount - 1
	if self.__refcount < 1:
	    print "RefCountBase.decrement(): RefCount reached",self.__refcount

 # (PortableServer::RefCountServantBase):
class ServantBase:
    def deactivate(self):
	# Something to do with poa here
	pass

def dump_region(region):
    xspan = region.span(Fresco.xaxis)
    yspan = region.span(Fresco.yaxis)
    print "Region: X=[%.1f,%.1f]@%.1f Y=[%.1f,%.1f]@%.1f"%(
	xspan.begin, xspan.end, xspan.align,
	yspan.begin, yspan.end, yspan.align
    )

class Region (Fresco__POA.Region):
    def __init__(self):
	self.__lower = Fresco.Vertex(0, 0, 0)
	self.__upper = Fresco.Vertex(0, 0, 0)
	self.__align = Fresco.Vertex(0, 0, 0) # align isnt a vertex but...
	self.__valid = 0
    def defined(self): # --> boolean
	return self.__valid
    def contains(self, v): # --> boolean
	return self.__valid and \
	    v.x >= self.__lower.x and v.x <= self.__upper.x and \
	    v.y >= self.__lower.y and v.y <= self.__upper.y and \
	    v.z >= self.__lower.z and v.z <= self.__upper.z
    def contains_plane(self, v, axis): # --> boolean
	if not self.__valid: return 0
	if axis == Fresco.xaxis:
	    return  v.y >= self.__lower.y and v.y <= self.__upper.y and \
		    v.z >= self.__lower.z and v.z <= self.__upper.z
	if axis == Fresco.yaxis:
	    return  v.x >= self.__lower.x and v.x <= self.__upper.x and \
		    v.z >= self.__lower.z and v.z <= self.__upper.z
	if axis == Fresco.zaxis:
	    return  v.x >= self.__lower.x and v.x <= self.__upper.x and \
		    v.y >= self.__lower.y and v.y <= self.__upper.y
	return 0
    def intersects(self, region): # --> boolean
	l, u = region.bounds()
	return self.__valid and \
	    self.__lower.x <= u.x and self.__upper.x >= l.x and \
	    self.__lower.y <= u.y and self.__upper.y >= l.y and \
	    self.__lower.z <= u.z and self.__upper.z >= l.z
    def copy(self, region): # region is a corba var
	if not region.defined(): return
	allot_x = region.span(Fresco.xaxis)
	allot_y = region.span(Fresco.yaxis)
	allot_z = region.span(Fresco.zaxis)
	self.__valid = 1
	self.__lower.x = allot_x.begin
	self.__lower.y = allot_y.begin
	self.__lower.z = allot_z.begin
	self.__upper.x = allot_x.end
	self.__upper.y = allot_y.end
	self.__upper.z = allot_z.end
	self.__align.x = allot_x.align
	self.__align.y = allot_y.align
	self.__align.z = allot_z.align
    def merge_intersect(self, region):
	if not region.defined(): return
	if not self.__valid: return self.copy(region)
	l, u = region.bounds()
	self.merge_max(self.__lower, l)
	self.merge_min(self.__upper, u)
    def merge_union(self, region):
	if not region.defined(): return
	if not self.__valid: return self.copy(region)
	l, u = region.bounds()
	self.merge_min(self.__lower, l)
	self.merge_max(self.__upper, u)
    def subtract(self, region):
	pass #NYI
    def apply_transform(self, tr):
	# See comment in RegionImpl.cc for info about this math
	if not self.__valid: return
	o = tr.transform_vertex(self.origin())
	m = tr.store_matrix()
	lx = self.__upper.x - self.__lower.x
	ly = self.__upper.y - self.__lower.y
	lz = self.__upper.z - self.__lower.z
	center = tr.transform_vertex(self.center())
	nlx = math.fabs(lx * m[0][0]) + math.fabs(ly * m[0][1]) + math.fabs(lz * m[0][2])
	nly = math.fabs(lx * m[1][0]) + math.fabs(ly * m[1][1]) + math.fabs(lz * m[1][2])
	nlz = math.fabs(lx * m[2][0]) + math.fabs(ly * m[2][1]) + math.fabs(lz * m[2][2])
	# form the new box
	self.__lower.x = center.x - nlx * 0.5;
	self.__upper.x = center.x + nlx * 0.5;
	self.__lower.y = center.y - nly * 0.5;
	self.__upper.y = center.y + nly * 0.5;
	self.__lower.z = center.z - nlz * 0.5;
	self.__upper.z = center.z + nlz * 0.5;
	if math.fabs(nlx) > 1e-4: xalign = (o.x - self.__lower.x) / nlx
	if math.fabs(nly) > 1e-4: yalign = (o.y - self.__lower.y) / nly
	if math.fabs(nlz) > 1e-4: zalign = (o.z - self.__lower.z) / nlz
    def bounds(self): # --> (Vertex, Vertex)
	return self.__lower, self.__upper
    def center(self): # --> Vertex
	return Fresco.Vertex(
	    (self.__upper.x + self.__lower.x) * .5,
	    (self.__upper.y + self.__lower.y) * .5,
	    (self.__upper.z + self.__lower.z) * .5)
    def origin(self): # --> Vertex
	return Fresco.Vertex(
	    self.span_origin(self.__lower.x, self.__upper.x, self.__align.x),
	    self.span_origin(self.__lower.y, self.__upper.y, self.__align.y),
	    self.span_origin(self.__lower.z, self.__upper.z, self.__align.z))
    def span(self, axis): # --> Region.Allotment
	Allotment = Fresco.Region.Allotment
	if axis == Fresco.xaxis:
	    return Allotment(self.__lower.x, self.__upper.x, self.__align.x)
	if axis == Fresco.yaxis:
	    return Allotment(self.__lower.y, self.__upper.y, self.__align.y)
	if axis == Fresco.zaxis:
	    return Allotment(self.__lower.z, self.__upper.z, self.__align.z)
    def outline(self): # --> Path
	pass # NYI
    # non-i/f utility methods
    def span_origin(self, lower, upper, align):
	if math.fabs(lower - upper) < 1e-4: return 0
	return lower + align * (upper - lower)
    def span_align(self, lower, upper, origin):
	if math.fabs(lower - upper) < 1e-4: return 0
	return (origin - lower) / (upper - lower)
    def merge_min(self, v0, v):
	v0.x = min(v0.x, v.x)
	v0.y = min(v0.y, v.y)
	v0.z = min(v0.z, v.z)
    def merge_max(self, v0, v):
	v0.x = max(v0.x, v.x)
	v0.y = max(v0.y, v.y)
	v0.z = max(v0.z, v.z)

def dump_transform(transform):
    m = transform.store_matrix()
    for j in range(4):
	if j == 1: print "M =",
	else: print "   ",
	print "[ %s ]"%string.join(map(str, m[j]))

class Transform (Fresco__POA.Transform):
    """Transform impl.
            [ r00 r01 r02 tx]
        M = [ r10 r11 r12 ty]
            [ r20 r21 r22 tz]
            [ 0   0   0   1 ]
    """
    def __init__(self):
	self.__dirty = 0
	self.__identity =  1
	self.__translation = 1 
	self.__xy = 1
	self.__matrix = map(list, [[0.]*4]*4)
	self.init()
    def dump(self):
	for j in range(4):
	    if j == 1: print "M =",
	    else: print "   ",
	    print "[ %s ]"%string.join(map(str, self[j]))
    def __getitem__(self, ix): return self.__matrix[ix]
    def init(self):
	for x in range(4):
	    self[x][x] = 1.
	    for y in range(4):
		if y != x: 
		    self[x][y] = 0.
	self.__identity = 1
	self.__translation = 1
	self.__xy = 1
	self.__dirty = 0
    def recompute(self):
	self.__translation = (
	    math.fabs(self[0][0] - 1.) < 1e-4 and \
	    math.fabs(self[1][1] - 1.) < 1e-4 and \
	    math.fabs(self[2][2] - 1.) < 1e-4 and \
	    math.fabs(self[0][1]) < 1e-4 and \
	    math.fabs(self[1][0]) < 1e-4 and \
	    math.fabs(self[0][2]) < 1e-4 and \
	    math.fabs(self[2][0]) < 1e-4 and \
	    math.fabs(self[1][2]) < 1e-4 and \
	    math.fabs(self[2][1]) < 1e-4)
	self.__xy = (self.__translation or (
	    math.fabs(self[2][2] - 1.) < 1e-4 and \
	    math.fabs(self[0][2]) < 1e-4 and \
	    math.fabs(self[2][0]) < 1e-4 and \
	    math.fabs(self[1][2]) < 1e-4 and \
	    math.fabs(self[2][1]) < 1e-4)) and \
	    math.fabs(self[2][3]) < 1e-4
	self.__identity = self.__translation and \
	    math.fabs(self[0][3]) < 1e-4 and \
	    math.fabs(self[1][3]) < 1e-4 and \
	    math.fabs(self[2][3]) < 1e-4
	self.__dirty = 0
    def det(self):
	sum =       self[0][0] * self[1][1] * self[2][2]
	sum = sum + self[0][1] * self[1][2] * self[2][0]
	sum = sum + self[0][2] * self[1][0] * self[2][1]
	sum = sum - self[0][2] * self[1][1] * self[2][0]
	sum = sum - self[0][1] * self[1][0] * self[2][2]
	sum = sum - self[0][0] * self[1][2] * self[2][1]
	return sum
    def copy(self, transform):
	if not transform: self.init()
	else: self.load_matrix(transform.store_matrix())
    def load_identity(self): self.init()
    def load_matrix(self, matrix):
	for i in range(3):
	    for j in range(4):
		self[i][j] = matrix[i][j]
	self[3][0] = self[3][1] = self[3][2] = 0.
	self[3][3] = 1.
	self.__dirty = 1
    def store_matrix(self): # --> Matrix
	return map(list, self.__matrix)
    def equal(self, transform): # --> bool
	if self.__dirty: self.recompute()
	if transform is None: return false
	if self.__identity != transform.identity(): return false
	m = transform.store_matrix()
	for x in range(3):
	    for y in range(4):
		if self[x][y] != m[x][y]: return false
	return true
    def identity(self): # --> bool
	if self.__dirty: self.recompute()
	return self.__identity
    def translation(self): # --> bool
	if self.__dirty: self.recompute()
	return self.__translation
    def det_is_zero(self): # --> bool
	if self.__dirty: self.recompute()
	return math.fabs(self.det()) < 1e-4
    def scale(self, vertex):
	self[0][0] = self[0][0] * vertex.x
	self[0][1] = self[0][1] * vertex.x
	self[0][2] = self[0][2] * vertex.x
	self[1][0] = self[1][0] * vertex.y
	self[1][1] = self[1][1] * vertex.y
	self[1][2] = self[1][2] * vertex.y
	self[2][0] = self[2][0] * vertex.z
	self[2][1] = self[2][1] * vertex.z
	self[2][2] = self[2][2] * vertex.z
	self.__dirty = 1
    def rotate(self, angle, axis):
	r_angle = ange * math.pi / 180
	c = math.cos(r_angle)
	s = math.sin(r_angle)
	mi, mj = [0.]*4, [0.]*4
	i, j = 0, 1
	if axis == Fresco.xaxis: i = 2
	elif axis == Fresco.yaxis: j = 2
	for y in range(4):
	    mi[y], mj[i] = self[i][y], self[j][y]
	for y in range(4):
	    self[i][y] = c * mi[y] - s * mj[y]
	    self[j][y] = s * mi[y] + c * mj[y]
	self.__dirty = 1
    def translate(self, vertex):
	self[0][3] = self[0][3] + vertex.x
	self[1][3] = self[1][3] + vertex.y
	self[2][3] = self[2][3] + vertex.z
	self.__dirty = 1
    def premultiply(self, transform):
	if not transform or transform.identity(): return
	m = transform.store_matrix()
	if self.identity():
	    self.load_matrix(m)
	    return
	for i in range(3):
	    mi = list(self[i])
	    for j in range(4):
		self[i][j] = mi[0]*m[0][j] + mi[1]*m[1][j] + mi[2]*m[2][j] + mi[3]*m[3][j]
	self.__dirty = 1
    def postmultiply(self, transform):
	if not transform or transform.identity(): return
	m = transform.store_matrix()
	if self.identity():
	    self.load_matrix(m)
	    return
	for j in range(4):
	    mj = (self[0][j], self[1][j], self[2][j])
	    for i in range(3):
		self[i][j] = mj[0]*m[i][0] + mj[1]*m[i][1] + mj[2]*m[i][2]
	self.__dirty = 1
    def invert(self):
	if self.translation():
	    self[0][3] = -self[0][3]
	    self[1][3] = -self[1][3]
	    self[2][3] = -self[2][3]
	    self.modifed()
	    return
	d = self.det()
	if math.fabs(d) < 1e-4: return
	m = map(list, self.__matrix[0:3]) + [0., 0., 0., 1.]
	self[0][0] =  (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / d
	self[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) / d
	self[0][2] =  (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / d
	self[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) / d
	self[1][1] =  (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / d
	self[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) / d
	self[2][0] =  (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / d
	self[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) / d
	self[2][2] =  (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / d
	self[0][3] = - (m[0][3] * self[0][0] + m[1][3] * self[0][1] + m[2][3] * self[0][2])
	self[1][3] = - (m[0][3] * self[1][0] + m[1][3] * self[1][1] + m[2][3] * self[1][2])
	self[2][3] = - (m[0][3] * self[2][0] + m[1][3] * self[2][1] + m[2][3] * self[2][2])
	self.__dirty = 1
    def transform_vertex(self, vertex): # --> Vertex
	tx, ty, tz = vertex.x, vertex.y, vertex.z
	nx = self[0][0] * tx + self[0][1] * ty + self[0][2] * tz + self[0][3]
	ny = self[1][0] * tx + self[1][1] * ty + self[1][2] * tz + self[1][3]
	nz = self[2][0] * tx + self[2][1] * ty + self[2][2] * tz + self[2][3]
	return Fresco.Vertex(nx, ny, nz)
    def inverse_transform_vertex(self, vertex): # --> Vertex
	d = self.det()
	if math.fabs(d) < 1e-4: return Fresco.Vertex(0,0,0)
	tmp = Fresco.Vertex(vertex.x, vertex.y, vertex.z)
	tmp.x = tmp.x - self[0][3]
	tmp.y = tmp.y - self[1][3]
	tmp.z = tmp.z - self[2][3]
	nx = ((self[1][1] * self[2][2] - self[1][2] * self[2][1]) * tmp.x -
	     (self[0][1] * self[2][2] - self[0][2] * self[2][1]) * tmp.y +
	     (self[0][1] * self[1][2] - self[0][2] * self[1][1]) * tmp.z) / d
	ny = (-(self[1][0] * self[2][2] - self[1][2] * self[2][0]) * tmp.x +
	     (self[0][0] * self[2][2] - self[0][2] * self[2][0]) * tmp.y -
	     (self[0][0] * self[1][2] - self[0][2] * self[1][0])) / d
	nz = ((self[1][0] * self[2][1] - self[1][1] * self[2][0]) -
	     (self[0][0] * self[2][1] - self[0][1] * self[2][0]) +
	     (self[0][0] * self[1][1] - self[0][1] * self[1][0])) / d
	return Fresco.Vertex(nx, ny, nz)

class Allocation (Fresco__POA.Allocation):
    def __init__(self):
	self.__list = []
    def add(self, region, screen):
	alloc = Region()
	alloc.copy(region)
	xform = Transform()
	xform.load_identity()
	info = Fresco.Allocation.Info(alloc._this(), xform._this(), screen)
	self.__list.append(info)
    def size(self):
	return len(self.__list)
    def get(self, index):
	return self.__list[index]

class Subject (Fresco__POA.Identifiable, Fresco__POA.RefCountBase):
    def __init__(self):
	RefCountBase.__init__(self)
	self.__blocked = 0
	self.__observers = []
    def attach(self, observer):
	self.__observers.append(observer)
    def detach(self, observer):
	obs = self.__observers
	for i in range(len(obs)):
	    while len(obs) > i and observer.is_identical(obs[i]):
		del obs[i]
    def block(self, blocked):
	self.__blocked = blocked
    def notify(self, any):
	if self.__blocked: return
	for observer in list(self.__observers):
	    try:
		observer.update(any)
	    except CORBA.SystemException:
		# Remove observer
		self.__observers.remove(observer)
	    except:
		print "Subject.notify(): Error occurred during update."
		raise

class Edge:
    "An edge in the DAG"
    def __init__(self, peer=None, peerId=None, localId=None):
	self.peer = peer
	self.peerId = peerId
	self.localId = localId

class Graphic (Fresco__POA.Graphic, Identifiable, RefCountBase):
    def __init__(self):
	RefCountBase.__init__(self)
	self.__parents = []

    class Iterator (Fresco__POA.GraphicIterator, ServantBase):
	def destroy(self): self.deactivate()

    # Attribute 'body'
    def _get_body(self): return None
    def _set_body(self, body): pass
    
    # Methods
    def append_graphic(self, graphic): # --> void
	pass
    def prepend_graphic(self, graphic): # --> void
	pass
    def remove_graphic(self, tag): # --> void
	pass
    def remove_child_graphic(self, tag): # --> void
	pass

    def add_parent_graphic(self, graphic, parent_tag): # --> Tag
	mytag = len(self.__parents)
	self.__parents.append( Edge(graphic, parent_tag, mytag) )
	return mytag
    def remove_parent_graphic(self, parent_tag): # --> void
	for i in range(len(self.__parents)):
	    if self.__parents[i].peerId == parent_tag:
		del self.__parents[i]
		return

    def first_child_graphic(self): # --> Iterator
	pass
    def last_child_graphic(self): # --> Iterator
	pass

    def transformation(self): # --> Transform
	pass

    def request(self): # --> void
	x = Fresco.Graphic.Requirement(1, 200., 300., 100., 0.)
	y = Fresco.Graphic.Requirement(1, 200., 300., 100., 0.)
	z = Fresco.Graphic.Requirement(1, 0., 0., 0., 0.)
	req = Fresco.Graphic.Requisition(x, y, z, 0)
	return req
	
    def extension(self, alloc_info, region): # --> void
	if alloc_info.transformation:
	    tmp = Region()
	    tmp.copy(alloc_info.allocation)
	    if not alloc_info.transformation.identity():
		tmp.apply_transform(alloc_info.transformation)
	    else:
		print "transformation was identity"
	    region.merge_union(tmp._this())
	else:
	    region.merge_union(alloc_info.allocation)
	#pv = lambda v: '(x:%f, y:%f, z:%f)'%(v.x,v.y,v.z)
	#print "extension:",map(pv, region.bounds())
    def shape(self, Region): # --> void
	pass

    def traverse(self, traversal): # --> void
	traversal.visit(self._this())

    def draw(self, drawTraversal): # --> void
	pass

    def pick(self, pickTraversal): # --> void
	pass

    def allocate(self, tag, alloc_info): # --> void
	pass
    def allocations(self, alloc): # --> void
	begin = alloc.size()
	for edge in self.__parents:
	    edge.peer.allocations(alloc)
	    end = alloc.size()
	    for j in range(begin, end):
		info = alloc.get(j)
		edge.peer.allocate(edge.peerId, info)
	    begin = end
    def need_redraw(self): # --> void
	for edge in self.__parents:
	    edge.peer.need_redraw()
    def need_redraw_region(self, Region): # --> void
	pass
    def need_resize(self): # --> void
	pass

class MonoGraphic (Graphic):
    "Python analogue of MonoGraphic"
    def __init__(self):
	Graphic.__init__(self)
	self.__child = Edge()
    def _get_body(self):
	self.__child.peer.increment()
	return self.__child.peer
    def _set_body(self, newpeer):
	peer = self.__child.peer
	if peer:
	    peer.remove_parent_graphic(self.__child.peerId)
	    peer.decrement()
	peer = self.__child.peer = newpeer
	if peer:
	    self.__child.peerId = peer.add_parent_graphic(self._this(), 0) 
	    peer.increment()
    def append_graphic(self, graphic):
	if not self.__child.peer: return
	self.__child.peer.append_graphic(graphic)
    def prepend_graphic(self, graphic):
	if not self.__child.peer: return
	self.__child.peer.prepend_graphic(graphic)
    def remove_graphic(self, localId):
	if not self.__child.peer: return
	self.__child.peer.remove_graphic(localId)
    def remove_child_graphic(self, localId):
	if localId == 0: self.__child.peer = None
	self.need_resize()
    def first_child_graphic(self):
	return self.__child.peer and \
	    self.__child.peer.first_child_graphic()
    def last_child_graphic(self):
	return self.__child.peer and \
	    self.__child.peer.first_child_graphic()
    def transformation(self):
	return self.__child.peer and \
	    self.__child.peer.transformation()
    def request(self):
	return self.__child.peer and \
	    self.__child.peer.request()
    def extension(self, info, region):
	if not self.__child.peer: return
	my_region = Region()
	my_region.copy(info.allocation)
	my_trans = Transform()
	my_trans.copy(info.transformation)
	my_info = Fresco.Allocation.Info(my_region._this(), my_trans._this(), None)
	self.allocate(0, my_info) # Template method to modify as per the concrete Graphic type (eg layout decorators etc)
	self.__child.peer.extension(my_info, region)
	#pv = lambda v: '(x:%f, y:%f, z:%f)'%(v.x,v.y,v.z)
	#print "extension:",map(pv, region.bounds())
    def shape(self, region):
	if self.__child.peer: self.__child.peer.shape(region)
    def traverse(self, traversal):
	if not self.__child.peer: return
	traversal.traverse_child(self.__child.peer, 0, None, None)
	    


class Controller (Fresco__POA.Controller, MonoGraphic, Subject):
    "Python impl of Fresco::Controller interface"
    def __init__(self, transparent):
	MonoGraphic.__init__(self)
	Subject.__init__(self)
	self.__parent = None
	self.__telltale = 0
	self.__constraint = None
	self.__grabs = 0 # bitmap against input_devices
	self.__focus = 0 # bitmap against input_devices
	self._children = []
	self.__transparent = transparent

    class Iterator (Fresco__POA.ControllerIterator, Graphic.Iterator):
	def __init__(self, con, tag):
	    self.__parent = con
	    self.__cursor = tag
	def child(self): # --> Controller
	    if self.__cursor >= len(self.__parent._children): return None
	    if self.__cursor < 0: return None
	    return self.__parent._children[self.__cursor] # duplicate?
	def next(self): self.__cursor = self.__cursor + 1
	def prev(self): self.__cursor = self.__cursor - 1
	def insert(self, child):
	    if self.__cursor > len(self.__parent._children):
		self.__cursor = len(self.__parent._children)
	    child.increment()
	    self.__parent._children.insert(self.__cursor, child)
	    child.set_parent_controller(self.__parent._this())
	    self.__parent.need_resize()
	def replace(self, child):
	    if self.__cursor > len(self.__parent._children): return
	    old = self.__parent._children[self.__cursor]
	    if old: # try for corba errors
		old.remove_parent_controller()
	    child.increment()
	    self.__parent._children[self.__cursor] = child
	    child.set_parent_controller(self.__parent._this())
	    self.__parent.need_resize()
	def remove(self):
	    if self.__cursor > len(self.__parent._children): return
	    self.__parent._children[self.__cursor].remove_parent_controller()
	    del self.__parent._children[self.__cursor]
	    self.__parent.need_resize()

    # ---- also implement TellTale interface for Controller
    def _get_constraint(self):
	return self.__constraint
    def _set_constraint(self, constraint):
	self.__constraint = constraint
    def set(self, mask): # --> void
	if self.__constraint:
	    self.__constraint.trymodify(self._this(), mask, 1)
	self.modify(mask, 1)
    def clear(self, mask): # --> void
	if self.__constraint:
	    self.__constraint.trymodify(self._this(), mask, 0)
	self.modify(mask, 0)
    def test(self, mask): # --> bool
	return (self.__telltale & mask) == mask
    def modify(self, mask, set): # --> void
	if set: flags = self.__telltale | mask
	else: flags = self.__telltale & ~mask
	if flags == self.__telltale: return
	self.__telltale = flags
	any = CORBA.Any(CORBA.TC_ulong, flags)
	self.notify(any)

    # ---- Graphic interface
    def draw(self, drawTraversal):
	MonoGraphic.traverse(self, drawTraversal)

    def pick(self, traversal):
	if not traversal.intersects_allocation(): return
	traversal.enter_controller(self._this())
	MonoGraphic.traverse(self, traversal)
	if not self.__transparent and not traversal.picked():
	    traversal.hit()
	traversal.leave_controller()

    def traverse(self, traversal):
	traversal.visit(self._this())

    # ---- Controller interface
    def append_controller(self, controller): # --> void
	controller.increment()
	self._children.append(controller)
	controller.set_parent_controller(self._this())
    def prepend_controller(self, controller): # --> void
	controller.increment()
	self._children.insert(0, controller)
	controller.set_parent_controller(self._this())
    def remove_controller(self, controller): # --> void
	for i in range(len(self._children)):
	    if self._children[i].is_identical(controller):
		controller.remove_parent_controller()
		del self._children[i]
		return
    def set_parent_controller(self, controller): # --> void
	self.__parent = controller
    def remove_parent_controller(self): # --> void
	self.__parent = None
    def parent_controller(self): # --> Controller
	return self.__parent
    def first_child_controller(self): # --> Iterator
	iter = Controller.Iterator(self, 0)
	#self.activate(iter)
	return iter._this()
    def last_child_controller(self): # --> Iterator
	iter = Controller.Iterator(self, len(self._children) - 1)
	#self.activate(iter)
	return iter._this()
    def grabbed(self, input_device): return self.__grabs & (1 << input_device)
    def _test_focus(self, input_device):
	return self.__focus & (1 << input_device)
    def set_focus(self, input_device):
	self.__focus = self.__focus | (1 << input_device)
	self.update_state()
    def clear_focus(self, input_device):
	self.__focus = self.__focus & ~(1 << input_device)
	self.update_state()
    def request_focus(self, controller, input_device): # --> boolean
	if not self.__parent: return 0
	return self.__parent.request_focus(controller, input_device)
    def receive_focus(self, focus): # --> boolean
	self.set_focus(focus._get_device())
	if (focus._get_device() == 0): self.set(Fresco.Controller.active)
	return true
    def lose_focus(self, input_device): # --> void
	self.clear_focus(input_device)
	if (input_device == 0): self.clear(Fresco.Controller.active)
    def first_focus(self, input_device): # --> boolean
	# Ask children first
	for child in self._children:
	    if child.first_focus(input_device):
		return true
	# Request ourself
	parent = self.parent_controller()
	if not parent: return false
	return parent.request_focus(self._this(), input_device)
    def last_focus(self, input_device): # --> boolean
	# Ask children first
	children = list(self._children)
	children.reverse()
	for child in children:
	    if child.first_focus(input_device):
		return true
	# Request ourself
	parent = self.parent_controller()
	if not parent: return false
	return parent.request_focus(self._this(), input_device)
    def next_focus(self, input_device): # --> boolean
	parent = self.parent_controller()
	if not parent: return false
	# First locate the next controller in control graph
	iter = parent.first_child_controller()
	next = iter.child()
	while next and not self.is_identical(next):
	    iter.next()
	    next = iter.child()
	# If 'self' is not in the list then its an error!
	if not next: raise NameError, 'I wasnt in my parent\'s controllers!'
	iter.next()
	next = iter.child()
	iter.destroy()
	# Now try to pass focus to it
	if next: return next.first_focus(input_device)
	# Or pass up to parent if I was the last in the list
	return parent.next_focus(input_device)
    def prev_focus(self, input_device): # --> boolean
	parent = self.parent_controller()
	if not parent: return false
	# First locate the next controller in control graph
	iter = parent.first_child_controller()
	prev = iter.child()
	while prev and not self.is_identical(prev):
	    iter.next()
	    prev = iter.child()
	# If 'self' is not in the list then its an error!
	if not prev: raise NameError, 'I wasnt in my parent\'s controllers!'
	iter.prev()
	prev = iter.child()
	iter.destroy()
	# Now try to pass focus to it
	if prev: return prev.last_focus(input_device)
	# Or pass up to parent if I was the last in the list
	return parent.prev_focus(input_device)
    def handle_positional(self, pickTraversal, input_event): # --> boolean
	try:
	    position = get_position(input_event)
	except TypeError:
	    return 0 # Fatal error, but we silently ignore
	if input_event[0].attr._d == Fresco.Input.button:
	    toggle = input_event[0].attr.selection
	    if toggle.actuation == Fresco.Input.Toggle.press:
		self.press(pickTraversal, input_event)
	    elif toggle.actuation == Fresco.Input.Toggle.release:
		self.release(pickTraversal, input_event)
	elif input_event[0].attr._d == Fresco.Input.positional:
	    if self.test(Fresco.Controller.pressed):
		self.drag(pickTraversal, input_event)
	    else:
		self.move(pickTraversal, input_event)
	else:
	    self.other(input_event)
	return 1 #Handled
    def handle_non_positional(self, input_event): # --> boolean
	if input_event[0].dev != 0 or input_event[0].attr._d != Fresco.Input.key:
	    # fatal, silent ignore
	    return 0
	if input_event[0].attr.selection.actuation != Fresco.Input.Toggle.press:
	    return 0
	self.key_press(input_event)
	return 1 # Handled
    def inside(self, pickTraversal):
	# Default impl: use bounding box
	return pickTraversal.intersects_allocation()
    def move(self, pickTraversal, input_event):
	pass
    def press(self, pickTraversal, input_event):
	self.grab(pickTraversal)
	self.request_focus(self._this(), 0)
	self.set(Fresco.Controller.pressed)
    def drag(self, pickTraversal, input_event):
	pass
    def release(self, pickTraversal, input_event):
	self.clear(Fresco.Controller.pressed)
	self.ungrab(pickTraversal)
    def double_click(self, pickTraversal, input_event):
	pass
    def key_press(self, input_event):
	toggle = input_event[0].attr.selection
	if toggle.number == Babylon.KEY_CURSOR_LEFT:
	    self.prev_focus(input_event[0].dev)
	elif toggle.number == Babylon.UC_HORIZONTAL_TABULATION or \
		toggle.number == Babylon.KEY_CURSOR_RIGHT:
	    self.next_focus(input_event[0].dev)
    def key_release(self, input_event):
	pass
    def other(self, input_event):
	pass
    def grab(self, pickTraversal):
	focus = pickTraversal.get_focus()
	if not focus: return
	focus.grab()
	self.__grabs = self.__grabs | (1 << focus._get_device())
	self.update_state()
    def ungrab(self, pickTraversal):
	focus = pickTraversal.get_focus()
	if not focus: return
	focus.ungrab()
	self.__grabs = self.__grabs & ~(1 << focus._get_device())
	self.update_state()
    def update_state(self):
	pass

# Finds a position from event
def get_position(event): # --> Position
    device = event[0].dev
    for i in range(len(event)):
	if event[i].dev != device: raise TypeError, "No positionals found"
	if event[i].attr._d == Fresco.Input.positional:
	    return event[i].attr.location
    raise TypeError, "No positionals found"


# vim: ts=8:sts=4:sw=4
