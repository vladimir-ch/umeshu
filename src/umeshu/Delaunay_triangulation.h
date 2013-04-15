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

#ifndef UMESHU_DELAUNAY_TRIANGULATION_H
#define UMESHU_DELAUNAY_TRIANGULATION_H

#include "Exact_adaptive_kernel.h"
#include "Triangulation.h"

#include <boost/unordered/unordered_set.hpp>
#include <boost/pool/pool_alloc.hpp>

namespace umeshu
{

template <typename Delaunay_triangulation_items, typename Kernel_ = Exact_adaptive_kernel, typename Alloc = boost::fast_pool_allocator<int> >
class Delaunay_triangulation : public Triangulation<Delaunay_triangulation_items, Kernel_, Alloc>
{

  typedef          Triangulation<Delaunay_triangulation_items, Kernel_, Alloc> Base;

public:

  typedef          Kernel_        Kernel;

  typedef typename Base::Node     Node;
  typedef typename Base::Halfedge Halfedge;
  typedef typename Base::Edge     Edge;
  typedef typename Base::Face     Face;

  typedef typename Base::Node_iterator       Node_iterator;
  typedef typename Base::Edge_iterator       Edge_iterator;
  typedef typename Base::Face_iterator       Face_iterator;

  typedef typename Base::Node_const_iterator Node_const_iterator;
  typedef typename Base::Edge_const_iterator Edge_const_iterator;
  typedef typename Base::Face_const_iterator Face_const_iterator;

  typedef typename Base::Node_handle         Node_handle;
  typedef typename Base::Halfedge_handle     Halfedge_handle;
  typedef typename Base::Edge_handle         Edge_handle;
  typedef typename Base::Face_handle         Face_handle;

  void make_cdt()
  {
    boost::unordered_set<Edge_iterator, edge_iterator_hash> edges_to_flip;

    for ( Edge_iterator iter = this->edges_begin(); iter != this->edges_end(); ++iter )
    {
      if ( not iter->is_constrained_delaunay() )
      {
        edges_to_flip.insert( iter );
      }
    }

    while ( not edges_to_flip.empty() )
    {
      Edge_handle e = *edges_to_flip.begin();
      edges_to_flip.erase( edges_to_flip.begin() );

      if ( not e->is_diagonal_of_convex_quadrilateral() || e->is_constrained_delaunay() )
      {
        continue;
      }

      Halfedge_handle he = e->he1();
      edges_to_flip.insert( he->next()->edge() );
      edges_to_flip.insert( he->prev()->edge() );
      edges_to_flip.insert( he->pair()->next()->edge() );
      edges_to_flip.insert( he->pair()->prev()->edge() );
      e->flip();
    }
  }

private:

  struct edge_iterator_hash
  {
    std::size_t operator()( Edge_iterator e ) const
    {
      return boost::hash<Edge*>()( &( *e ) );
    }
  };

};

} // namespace umeshu

#endif // UMESHU_DELAUNAY_TRIANGULATION_H
