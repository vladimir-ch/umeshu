//
//  Copyright (c) 2011-2013 Vladimir Chalupecky
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

#ifndef UMESHU_DELAUNAY_TRIANGULATION_ITEMS_H
#define UMESHU_DELAUNAY_TRIANGULATION_ITEMS_H

#include "Orientation.h"
#include "Triangulation_items.h"

namespace umeshu {

template <typename Kernel, typename HDS>
class Delaunay_triangulation_edge_base : public Triangulation_edge_base<Kernel, HDS>
{

  typedef Triangulation_edge_base<Kernel, HDS> Base;

public:

  typedef typename Base::Node_handle     Node_handle;
  typedef typename Base::Halfedge_handle Halfedge_handle;
  typedef typename Base::Edge_handle     Edge_handle;
  typedef typename Base::Face_handle     Face_handle;

  Delaunay_triangulation_edge_base( Halfedge_handle g, Halfedge_handle h )
    : Base( g, h )
    , constrained_( false )
  {}

  bool is_constrained() const
  {
    return constrained_;
  }

  void set_constrained( bool constrained )
  {
    constrained_ = constrained;
  }

  bool is_constrained_delaunay() const
  {
    if ( this->is_constrained() || this->is_boundary() )
    {
      return true;
    }

    Point2 const& p1 = this->he1()->origin()->position();
    Point2 const& p2 = this->he2()->prev()->origin()->position();
    Point2 const& p3 = this->he2()->origin()->position();
    Point2 const& p4 = this->he1()->prev()->origin()->position();

    if ( Kernel::oriented_circle( p1, p2, p3, p4 ) == ON_POSITIVE_SIDE )
    {
      return false;
    }

    return true;
  }

private:

  bool constrained_;

};


template <typename Kernel, typename HDS>
class Delaunay_triangulation_edge_base_with_id : public Delaunay_triangulation_edge_base<Kernel, HDS>
                                               , public Identifiable
{

  typedef Delaunay_triangulation_edge_base<Kernel, HDS> Base;

public:

  typedef typename Base::Node_handle     Node_handle;
  typedef typename Base::Halfedge_handle Halfedge_handle;
  typedef typename Base::Edge_handle     Edge_handle;
  typedef typename Base::Face_handle     Face_handle;

  Delaunay_triangulation_edge_base_with_id( Halfedge_handle g, Halfedge_handle h )
    : Base( g, h )
  {}

};


struct Delaunay_triangulation_items
{

  typedef boost::false_type Supports_intrusive_list;
  typedef boost::false_type Supports_id;

  template <typename Kernel, typename HDS>
  struct Node_wrapper
  {
    typedef Triangulation_node_base<Kernel, HDS> Node;
  };

  template <typename Kernel, typename HDS>
  struct Halfedge_wrapper
  {
    typedef Triangulation_halfedge_base<Kernel, HDS> Halfedge;
  };

  template <typename Kernel, typename HDS>
  struct Edge_wrapper
  {
    typedef Delaunay_triangulation_edge_base<Kernel, HDS> Edge;
  };

  template <typename Kernel, typename HDS>
  struct Face_wrapper
  {
    typedef Triangulation_face_base<Kernel, HDS> Face;
  };

};


struct Delaunay_triangulation_items_with_id
{

  typedef boost::false_type Supports_intrusive_list;
  typedef boost::true_type  Supports_id;

  template <typename Kernel, typename HDS>
  struct Node_wrapper
  {
    typedef Triangulation_node_base_with_id<Kernel, HDS> Node;
  };

  template <typename Kernel, typename HDS>
  struct Halfedge_wrapper
  {
    typedef Triangulation_halfedge_base_with_id<Kernel, HDS> Halfedge;
  };

  template <typename Kernel, typename HDS>
  struct Edge_wrapper
  {
    typedef Delaunay_triangulation_edge_base_with_id<Kernel, HDS> Edge;
  };

  template <typename Kernel, typename HDS>
  struct Face_wrapper
  {
    typedef Triangulation_face_base_with_id<Kernel, HDS> Face;
  };

};

} // namespace umeshu

#endif // UMESHU_DELAUNAY_TRIANGULATION_ITEMS
