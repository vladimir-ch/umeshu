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

#ifndef __HDS_ITEMS_H_INCLUDED__
#define __HDS_ITEMS_H_INCLUDED__ 

#include "HDS_node_base.h"
#include "HDS_halfedge_base.h"
#include "HDS_edge_base.h"
#include "HDS_face_base.h"

namespace umeshu {
namespace hds {

struct HDS_items {
    template <typename Kernel, typename HDS>
    struct Node_wrapper {
        typedef HDS_node_base<HDS> Node;
    };
    template <typename Kernel, typename HDS>
    struct Halfedge_wrapper {
        typedef HDS_halfedge_base<HDS> Halfedge;  
    };
    template <typename Kernel, typename HDS>
    struct Edge_wrapper {
        typedef HDS_edge_base<HDS> Edge;  
    };
    template <typename Kernel, typename HDS>
    struct Face_wrapper {
        typedef HDS_face_base<HDS> Face;  
    };
};

} // namespace hds
} // namespace umeshu

#endif /* __HDS_ITEMS_H_INCLUDED__ */
