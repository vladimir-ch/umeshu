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

#ifndef UMESHU_HDS_HDS_HALFEDGE_BASE_H
#define UMESHU_HDS_HDS_HALFEDGE_BASE_H

#include "Identifiable.h"

#include <boost/intrusive/list_hook.hpp>
#include <boost/type_traits/integral_constant.hpp>

namespace umeshu {
namespace hds {

template <typename HDS>
class HDS_halfedge_base
{

public:

  typedef boost::false_type Supports_intrusive_list;
  typedef boost::false_type Supports_id;

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

private:

  Halfedge_handle pair_;
  Halfedge_handle next_;
  Halfedge_handle prev_;
  Node_handle     origin_;
  Edge_handle     edge_;
  Face_handle     face_;

};


template <typename HDS>
class HDS_halfedge_base_with_id : public HDS_halfedge_base<HDS>
                                , public Identifiable
{
  
  typedef HDS_halfedge_base<HDS> Base;

public:

  typedef boost::false_type Supports_intrusive_list;
  typedef boost::true_type  Supports_id;

  typedef typename Base::Node_handle     Node_handle;
  typedef typename Base::Halfedge_handle Halfedge_handle;
  typedef typename Base::Edge_handle     Edge_handle;
  typedef typename Base::Face_handle     Face_handle;

};


template <typename HDS>
class HDS_halfedge_base_intrusive_list : public HDS_halfedge_base<HDS>
                                       , public boost::intrusive::list_base_hook<>
{
  
  typedef HDS_halfedge_base<HDS> Base;

public:

  typedef boost::true_type  Supports_intrusive_list;
  typedef boost::false_type Supports_id;

  typedef typename Base::Node_handle     Node_handle;
  typedef typename Base::Halfedge_handle Halfedge_handle;
  typedef typename Base::Edge_handle     Edge_handle;
  typedef typename Base::Face_handle     Face_handle;

};


template <typename HDS>
class HDS_halfedge_base_intrusive_list_with_id : public HDS_halfedge_base<HDS>
                                               , public boost::intrusive::list_base_hook<>
                                               , public Identifiable
{
  
  typedef HDS_halfedge_base<HDS> Base;

public:

  typedef boost::true_type Supports_intrusive_list;
  typedef boost::true_type Supports_id;

  typedef typename Base::Node_handle     Node_handle;
  typedef typename Base::Halfedge_handle Halfedge_handle;
  typedef typename Base::Edge_handle     Edge_handle;
  typedef typename Base::Face_handle     Face_handle;

};

} // namespace hds
} // namespace umeshu

#endif // UMESHU_HDS_HDS_HALFEDGE_BASE_H
