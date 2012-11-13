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

#ifndef __HDS_FACE_BASE_H_INCLUDED__
#define __HDS_FACE_BASE_H_INCLUDED__ 

#include <boost/assert.hpp>

namespace umeshu {
namespace hds {

template <typename HDS>
class HDS_face_base {
public:
    typedef typename HDS::Node_handle           Node_handle;
    typedef typename HDS::Node_const_handle     Node_const_handle;
    typedef typename HDS::Halfedge_handle       Halfedge_handle;
    typedef typename HDS::Halfedge_const_handle Halfedge_const_handle;
    typedef typename HDS::Edge_handle           Edge_handle;
    typedef typename HDS::Edge_const_handle     Edge_const_handle;
    typedef typename HDS::Face_handle           Face_handle;
    typedef typename HDS::Face_const_handle     Face_const_handle;

    Halfedge_handle       halfedge ()       { return adj_he_; }
    Halfedge_const_handle halfedge () const { return adj_he_; }
    void                  set_halfedge(Halfedge_handle he) { adj_he_ = he; }    
    
private:
    Halfedge_handle adj_he_;
};

} // namespace hds
} // namespace umeshu

#endif /* __HDS_FACE_BASE_H_INCLUDED__ */
