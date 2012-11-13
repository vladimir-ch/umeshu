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

#ifndef __HDS_EDGE_BASE_H_INCLUDED__
#define __HDS_EDGE_BASE_H_INCLUDED__ 

#include <boost/assert.hpp>

namespace umeshu {
namespace hds {

template <typename HDS>
class HDS_edge_base {
public:
    typedef typename HDS::Node_handle           Node_handle;
    typedef typename HDS::Node_const_handle     Node_const_handle;
    typedef typename HDS::Halfedge_handle       Halfedge_handle;
    typedef typename HDS::Halfedge_const_handle Halfedge_const_handle;
    typedef typename HDS::Edge_handle           Edge_handle;
    typedef typename HDS::Edge_const_handle     Edge_const_handle;
    typedef typename HDS::Face_handle           Face_handle;
    typedef typename HDS::Face_const_handle     Face_const_handle;

    HDS_edge_base(Halfedge_handle g, Halfedge_handle h)
        : halfedge_(g)
    {
        g->set_pair(h);
        h->set_pair(g);
    }

    Halfedge_handle       he1()       { return halfedge_; }
    Halfedge_const_handle he1() const { return halfedge_; }
    Halfedge_handle       he2()       { return halfedge_->pair(); }
    Halfedge_const_handle he2() const { return halfedge_->pair(); }

    Halfedge_handle halfedge_with_origin(Node_handle n) {
        BOOST_ASSERT(he1()->origin()==n || he2()->origin()==n);
        return (he1()->origin() == n) ? he1() : he2();
    }
    Halfedge_const_handle halfedge_with_origin(Node_handle n) const {
        BOOST_ASSERT(he1()->origin()==n || he2()->origin()==n);
        return (he1()->origin() == n) ? he1() : he2();
    }

    bool is_boundary () const { return he1()->is_boundary() || he2()->is_boundary(); }

    void nodes(Node_handle& n1, Node_handle& n2) {
        n1 = he1()->origin();
        n2 = he2()->origin();
    }

private:
    Halfedge_handle halfedge_;
};

} // namespace hds
} // namespace umeshu

#endif /* __HDS_EDGE_BASE_H_INCLUDED__ */
