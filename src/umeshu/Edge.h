//
//  Copyright (c) 2011-2012 Vladimir Chalupecky
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.

#ifndef __EDGE_H_INCLUDED__
#define __EDGE_H_INCLUDED__

#include "Identifiable.h"
#include "BoundarySegment.h"
#include "Point2.h"

#include "iostream"

namespace umeshu {

template <typename Mesh>
class Halfedge {
    typedef typename Mesh::Node_handle           Node_handle;
    typedef typename Mesh::Node_const_handle     Node_const_handle;
    typedef typename Mesh::Halfedge_handle       Halfedge_handle;
    typedef typename Mesh::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Mesh::Edge_handle           Edge_handle;
    typedef typename Mesh::Edge_const_handle     Edge_const_handle;
    typedef typename Mesh::Face_handle           Face_handle;
    typedef typename Mesh::Face_const_handle     Face_const_handle;

public:
    Node_handle           origin()                     { return origin_; }
    Node_const_handle     origin() const               { return origin_; }
    void                  set_origin(Node_handle n)    { origin_ = n; }

    Halfedge_handle       next()                       { return next_; }
    Halfedge_const_handle next()   const               { return next_; }
    void                  set_next(Halfedge_handle he) { next_ = he; }

    Halfedge_handle       prev()                       { return prev_; }
    Halfedge_const_handle prev()   const               { return prev_; }
    void                  set_prev(Halfedge_handle he) { prev_ = he; }

    Face_handle           face()                       { return face_; }
    Face_const_handle     face()   const               { return face_; }
    void                  set_face(Face_handle f)      { face_ = f; }

    Halfedge_handle       pair()                       { return pair_; }
    Halfedge_const_handle pair()   const               { return pair_; }

    Edge_handle           edge()                       { return edge_; }
    Edge_const_handle     edge()   const               { return edge_; }

    bool is_boundary() const { return face_ == Face_handle(); }

    void vertices(Point2& porig, Point2& pdest) const {
        porig = origin()->position();
        pdest = pair()->origin()->position();
    }

    Halfedge()
      : pair_(), next_(), prev_(), edge_(), origin_(), face_()
    {}


    Halfedge(Halfedge<Mesh> const& he)
      : origin_(he.origin_), face_(he.face_)
    {}

    Halfedge<Mesh>& operator= (Halfedge<Mesh> const& he)
    {
        if (this != &he) {
            origin_ = he.origin_;
            face_ = he.face_;
        }
        return *this;
    }

private:

    void set_pair(Halfedge_handle he) { pair_ = he; }
    void set_edge(Edge_handle e)      { edge_ = e; }

    Halfedge_handle pair_;
    Halfedge_handle next_;
    Halfedge_handle prev_;
    Edge_handle     edge_;
    Node_handle     origin_;
    Face_handle     face_;

    template<class> friend class Edge;
};

template <typename Mesh>
inline std::ostream& operator<< (std::ostream& os, Halfedge<Mesh> const& he)
{
    os << "Halfedge " << &he
       << ": pair = " << he.pair()
       << " next = " << he.next()
       << " prev = " << he.prev()
       << " edge = " << &(*he.edge())
       << " orig = " << &(*he.origin());
    return os;
}

template <typename Mesh>
class Edge : public Identifiable {
public:
    typedef typename Mesh::Kernel_type           Kernel_type;
    typedef typename Mesh::Node_handle           Node_handle;
    typedef typename Mesh::Edge_handle           Edge_handle;
    typedef typename Mesh::Halfedge_handle       Halfedge_handle;
    typedef typename Mesh::Halfedge_const_handle Halfedge_const_handle;

    Edge(Node_handle n1, Node_handle n2, BoundarySegment *bseginfo)
        : bseginfo_(bseginfo)
    {
        he1()->set_origin(n1);
        he2()->set_origin(n2);
        setup_halfedges();
    }

    Edge(Edge<Mesh> const& e)
        : halfedge_pair(e.halfedge_pair), bseginfo_(e.boundary_info())
    {
        setup_halfedges();
    }

    Edge<Mesh>& operator= (Edge<Mesh> const& e)
    {
        if (this != &e) {
            halfedge_pair = e.halfedge_pair;
            setup_halfedges();
        }
        return *this;
    }

    void setup_halfedges()
    {
        he1()->set_next(he2());
        he1()->set_prev(he2());
        he2()->set_next(he1());
        he2()->set_prev(he1());
        he1()->set_pair(he2());
        he2()->set_pair(he1());
    }

    Halfedge_handle       he1()       { return &halfedge_pair.first; }
    Halfedge_const_handle he1() const { return &halfedge_pair.first; }
    Halfedge_handle       he2()       { return &halfedge_pair.second; }
    Halfedge_const_handle he2() const { return &halfedge_pair.second; }

    void set_edge_handle(Edge_handle e) {
        halfedge_pair.first.set_edge(e);
        halfedge_pair.second.set_edge(e);
    }

    Halfedge_handle halfedge_with_origin(Node_handle n) {
        BOOST_ASSERT(he1()->origin()==n || he2()->origin()==n);
        return (he1()->origin() == n) ? he1() : he2();
    }
    Halfedge_const_handle halfedge_with_origin(Node_handle n) const {
        BOOST_ASSERT(he1()->origin()==n || he2()->origin()==n);
        return (he1()->origin() == n) ? he1() : he2();
    }

    double length () const {
        return distance(he1()->origin(), he2()->origin());
    }

	Point2 midpoint () const;

    bool is_boundary        () const { return he1()->is_boundary() || he2()->is_boundary(); }
    bool is_encroached_upon (Point2 const& p) const;
    bool is_swappable       () const;
    bool is_delaunay        () const;

    BoundarySegment* boundary_info() const { return bseginfo_; }

    void nodes(Node_handle& n1, Node_handle& n2) {
        n1 = he1()->origin();
        n2 = he2()->origin();
    }

    void vertices(Point2& p1, Point2& p2) const {
        he1()->vertices(p1, p2);
    }

private:
    std::pair<Halfedge<Mesh>,Halfedge<Mesh> > halfedge_pair;
	BoundarySegment *bseginfo_;
};

template <typename Mesh>
inline std::ostream& operator<< (std::ostream& os, Edge<Mesh> const& e)
{
    Point2 p1, p2;
    e.vertices(p1, p2);
    os << "Edge " << &e << " vertices: " << p1 << p2 << std::endl;
    os << *(e.he1()) << std::endl;
    os << *(e.he2()) << std::endl;
    return os;
}

template <typename Mesh>
bool Edge<Mesh>::is_encroached_upon(Point2 const& p) const
{
    Point2 p1, p2;
    this->vertices(p1, p2);
    double dot_p = (p1.x() - p.x())*(p2.x() - p.x()) +
                   (p1.y() - p.y())*(p2.y() - p.y());
    return dot_p < 0.0;
}

template <typename Mesh>
Point2 Edge<Mesh>::midpoint() const
{
	Point2 p1, p2;
	this->vertices(p1, p2);
	if (bseginfo_ == NULL) {
		return umeshu::midpoint(p1, p2);
	} else {
		return bseginfo_->midpoint(p1, p2);
	}
}

template <typename Mesh>
bool Edge<Mesh>::is_swappable() const
{
    if (this->is_boundary()) {
        return false;
    }
    Point2 p1 = this->he1()->origin()->position();
    Point2 p2 = this->he2()->prev()->origin()->position();
    Point2 p3 = this->he2()->origin()->position();
    Point2 p4 = this->he1()->prev()->origin()->position();
    
    if (Kernel_type::oriented_side(p1, p2, p3) != ON_POSITIVE_SIDE ||
        Kernel_type::oriented_side(p2, p3, p4) != ON_POSITIVE_SIDE ||
        Kernel_type::oriented_side(p3, p4, p1) != ON_POSITIVE_SIDE ||
        Kernel_type::oriented_side(p4, p1, p2) != ON_POSITIVE_SIDE)
    {
        return false;
    }
    return true;    
}

template <typename Mesh>
bool Edge<Mesh>::is_delaunay() const
{
    if (this->is_boundary()) {
        return true;
    }
    
    Point2 p1 = this->he1()->origin()->position();
    Point2 p2 = this->he2()->prev()->origin()->position();
    Point2 p3 = this->he2()->origin()->position();
    Point2 p4 = this->he1()->prev()->origin()->position();
    
    if (Kernel_type::oriented_circle(p1, p2, p3, p4) == ON_POSITIVE_SIDE) {
        return false;
    }
    return true;
}

} // namespace umeshu

#endif // __EDGE_H_INCLUDED__
