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

#ifndef __DELAUNAY_TRIANGULATION_ITEMS_H_INCLUDED__
#define __DELAUNAY_TRIANGULATION_ITEMS_H_INCLUDED__ 

#include "Exact_adaptive_kernel.h"
#include "Triangulation_items.h"

namespace umeshu {

template <typename Kernel, typename HDS>
class Delaunay_triangulation_node_base : public Triangulation_node_base<Kernel, HDS> {
public:
    typedef          Triangulation_node_base<Kernel,HDS> Base;
    typedef typename Kernel::Point_2             Point_2;

    typedef typename Base::Node_handle           Node_handle;
    typedef typename Base::Node_const_handle     Node_const_handle;
    typedef typename Base::Halfedge_handle       Halfedge_handle;
    typedef typename Base::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Base::Edge_handle           Edge_handle;
    typedef typename Base::Edge_const_handle     Edge_const_handle;
    typedef typename Base::Face_handle           Face_handle;
    typedef typename Base::Face_const_handle     Face_const_handle;

    Delaunay_triangulation_node_base() : Base() {}
    explicit Delaunay_triangulation_node_base(Point_2 const& p) : Base(p) {}
};

template <typename Kernel, typename HDS>
class Delaunay_triangulation_halfedge_base : public Triangulation_halfedge_base<Kernel, HDS> {
public:
    typedef          Triangulation_halfedge_base<Kernel, HDS> Base;
    typedef typename Kernel::Point_2             Point_2;

    typedef typename Base::Node_handle           Node_handle;
    typedef typename Base::Node_const_handle     Node_const_handle;
    typedef typename Base::Halfedge_handle       Halfedge_handle;
    typedef typename Base::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Base::Edge_handle           Edge_handle;
    typedef typename Base::Edge_const_handle     Edge_const_handle;
    typedef typename Base::Face_handle           Face_handle;
    typedef typename Base::Face_const_handle     Face_const_handle;
};

template <typename Kernel, typename HDS>
class Delaunay_triangulation_edge_base : public Triangulation_edge_base<Kernel, HDS> {
public:
    typedef          Triangulation_edge_base<Kernel, HDS> Base;
    typedef typename Kernel::Point_2             Point_2;

    typedef typename Base::Node_handle           Node_handle;
    typedef typename Base::Node_const_handle     Node_const_handle;
    typedef typename Base::Halfedge_handle       Halfedge_handle;
    typedef typename Base::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Base::Edge_handle           Edge_handle;
    typedef typename Base::Edge_const_handle     Edge_const_handle;
    typedef typename Base::Face_handle           Face_handle;
    typedef typename Base::Face_const_handle     Face_const_handle;

    Delaunay_triangulation_edge_base(Halfedge_handle g, Halfedge_handle h)
        : Base(g, h)
    {}

    Point_2 midpoint() const {
        Point_2 p1, p2;
        this->vertices(p1, p2);
        return 0.5*(p1+p2);
    }

    bool is_encroached_upon(Point_2 const& p) const {
        Point_2 p1, p2;
        this->vertices(p1, p2);
        double dot_p = (p1.x() - p.x())*(p2.x() - p.x()) +
            (p1.y() - p.y())*(p2.y() - p.y());
        return dot_p < 0.0;
    }

    bool is_constrained() const {
        return this->is_boundary();
    }

    bool is_delaunay() const {
        if (this->is_constrained()) {
            return true;
        }

        Point_2 p1 = this->he1()->origin()->position();
        Point_2 p2 = this->he2()->prev()->origin()->position();
        Point_2 p3 = this->he2()->origin()->position();
        Point_2 p4 = this->he1()->prev()->origin()->position();

        if (Kernel::oriented_circle(p1, p2, p3, p4) == Kernel::ON_POSITIVE_SIDE) {
            return false;
        }
        return true;
    }
};

template <typename Kernel, typename HDS>
class Delaunay_triangulation_face_base : public Triangulation_face_base<Kernel, HDS> {
public:
    typedef          Triangulation_face_base<Kernel,HDS> Base;
    typedef typename Kernel::Point_2             Point_2;

    typedef typename Base::Node_handle           Node_handle;
    typedef typename Base::Node_const_handle     Node_const_handle;
    typedef typename Base::Halfedge_handle       Halfedge_handle;
    typedef typename Base::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Base::Edge_handle           Edge_handle;
    typedef typename Base::Edge_const_handle     Edge_const_handle;
    typedef typename Base::Face_handle           Face_handle;
    typedef typename Base::Face_const_handle     Face_const_handle;

};

struct Delaunay_triangulation_items {
    template <typename Kernel, typename HDS>
    struct Node_wrapper {
        typedef Delaunay_triangulation_node_base<Kernel, HDS> Node;
    };
    template <typename Kernel, typename HDS>
    struct Halfedge_wrapper {
        typedef Delaunay_triangulation_halfedge_base<Kernel, HDS> Halfedge;  
    };
    template <typename Kernel, typename HDS>
    struct Edge_wrapper {
        typedef Delaunay_triangulation_edge_base<Kernel, HDS> Edge;
    };
    template <typename Kernel, typename HDS>
    struct Face_wrapper {
        typedef Delaunay_triangulation_face_base<Kernel, HDS> Face;  
    };
};

} // namespace umeshu

#endif /* __DELAUNAY_TRIANGULATION_ITEMS_H_INCLUDED__ */
