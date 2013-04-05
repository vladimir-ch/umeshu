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

#ifndef __HDS_HALFEDGE_BASE_H_INCLUDED__
#define __HDS_HALFEDGE_BASE_H_INCLUDED__

namespace umeshu {
namespace hds {

template <typename HDS>
class HDS_halfedge_base
{
public:
  typedef typename HDS::Node_handle     Node_handle;
  typedef typename HDS::Halfedge_handle Halfedge_handle;
  typedef typename HDS::Edge_handle     Edge_handle;
  typedef typename HDS::Face_handle     Face_handle;

  HDS_halfedge_base()
    : pair_()
    , next_()
    , prev_()
    , origin_()
    , edge_()
    , face_()
  {}

  Node_handle     origin() const { return origin_; }
  Halfedge_handle next()   const { return next_; }
  Halfedge_handle prev()   const { return prev_; }
  Face_handle     face()   const { return face_; }
  Halfedge_handle pair()   const { return pair_; }
  Edge_handle     edge()   const { return edge_; }

  void set_origin( Node_handle n )    { origin_ = n; }
  void set_next( Halfedge_handle he ) { next_ = he; }
  void set_prev( Halfedge_handle he ) { prev_ = he; }
  void set_face( Face_handle f )      { face_ = f; }
  void set_pair( Halfedge_handle he ) { pair_ = he; }
  void set_edge( Edge_handle e )      { edge_ = e; }

  bool is_boundary() const { return face_ == Face_handle(); }

private:
  Halfedge_handle pair_;
  Halfedge_handle next_;
  Halfedge_handle prev_;
  Node_handle     origin_;
  Edge_handle     edge_;
  Face_handle     face_;
};

} // namespace hds
} // namespace umeshu

#endif /* __HDS_HALFEDGE_BASE_H_INCLUDED__ */
