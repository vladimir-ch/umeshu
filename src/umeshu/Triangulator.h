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

#ifndef __TRIANGULATOR_H_INCLUDED__
#define __TRIANGULATOR_H_INCLUDED__

#include "Exact_adaptive_kernel.h"
#include "Exceptions.h"
#include "Orientation.h"
#include "Polygon.h"

#include <boost/foreach.hpp>

#include <list>

namespace umeshu
{

namespace internal
{

template< typename Triangulation >
class Add_point_to_triangulation
{
  typedef typename Triangulation::Node_handle Node_handle;
  typedef typename Triangulation::Halfedge_handle Halfedge_handle;

public:

  Add_point_to_triangulation( Triangulation& tria, std::size_t num_points )
    : last_halfedge()
    , tria( &tria )
    , first_node()
    , prev_node()
    , num_points( num_points )
  {}

  void operator()( Point2 const& p )
  {
    if ( tria->number_of_nodes() == 0 )
    {
      // we are adding the first point
      first_node = tria->add_node( p );
      prev_node = first_node;
    }
    else if ( tria->number_of_nodes() == num_points - 1 )
    {
      // we are back at the first point, just add the edge
      last_halfedge = tria->add_edge( prev_node, first_node );
    }
    else
    {
      Node_handle cur_node = tria->add_node( p );
      tria->add_edge( prev_node, cur_node );
      prev_node = cur_node;
    }
  }

  Halfedge_handle last_halfedge;

private:

  Triangulation* tria;
  Node_handle first_node, prev_node;
  std::size_t num_points;
};

} // namespace internal

template <typename Triangulation>
class Triangulator
{
public:
  typedef          Triangulation         Tria;
  typedef typename Tria::Kernel          Kernel;

  typedef typename Tria::Node_handle     Node_handle;
  typedef typename Tria::Halfedge_handle Halfedge_handle;
  typedef typename Tria::Edge_handle     Edge_handle;
  typedef typename Tria::Face_handle     Face_handle;

  void triangulate( Polygon const& poly, Tria& tria );

  struct triangulator_error : virtual umeshu_error {};

private:

  bool halfedge_origin_is_convex( Halfedge_handle he ) const;
  bool halfedge_origin_is_ear( Halfedge_handle he ) const;

  void classify_vertices( Halfedge_handle bhe )
  {
    Halfedges convex_vertices;

    Halfedge_handle he_iter = bhe;

    do
    {
      if ( halfedge_origin_is_convex( he_iter ) )
      {
        convex_vertices.push_back( he_iter );
      }
      else
      {
        reflex_vertices.push_back( he_iter );
      }

      he_iter = he_iter->next();
    }
    while ( he_iter != bhe );

    BOOST_FOREACH( Halfedge_handle conv_he, convex_vertices )
    {
      if ( this->halfedge_origin_is_ear( conv_he ) )
      {
        ears.push_back( conv_he );
      }
    }
  }

  typedef std::list<Halfedge_handle> Halfedges;
  Halfedges reflex_vertices, ears;
};

template <typename Triangulation>
void Triangulator<Triangulation>::triangulate( Polygon const& polygon, Triangulation& tria )
{
  Polygon poly( polygon );

  std::size_t num_points = boost::geometry::num_points( poly );
  std::cerr << "Number of input polygon vertices: " << num_points << std::endl;
  if ( num_points < 4 )
  {
    // we need at least a triangle at the input
    throw triangulator_error();
  }

  // check if we need to reverse the input polygon
  double area = boost::geometry::area( poly );
  std::cerr << "Area of the input polygon: " << area << std::endl;
  if ( area < 0 )
  {
    std::cerr << "Reversing the input polygon\n";
    boost::geometry::reverse( poly );
  }

  // insert polygon into the triangulation
  internal::Add_point_to_triangulation< Triangulation > add_points_to_tria( tria, num_points );
  add_points_to_tria = boost::geometry::for_each_point( poly, add_points_to_tria );

  // classify the boundary vertices for 'earness'
  this->classify_vertices( add_points_to_tria.last_halfedge );

  while ( !ears.empty() )
  {
    Halfedge_handle he2 = *ears.begin();
    Halfedge_handle he1 = he2->prev();
    Halfedge_handle he5 = he2->next();
    Node_handle n1 = he1->origin();
    Node_handle n3 = he5->origin();

    // since we will cut it off, remove the ear from ears. Also, we have to
    // erase he1 and he5 from all the sets, since after cutting the ear we
    // will have to update their info anyway
    ears.remove( he2 );
    ears.remove( he1 );
    reflex_vertices.remove( he1 );
    ears.remove( he5 );
    reflex_vertices.remove( he5 );

    // if this is not the last ear, i.e., only one triangle left to
    // triangulate
    if ( he5 != he1->prev() )
    {
      Halfedge_handle he3 = tria.add_edge( n3, n1 );
      Halfedge_handle he4 = he3->pair();
      tria.add_face( he1, he2, he3 );

      bool he4_is_convex = false, he5_is_convex = false;

      if ( halfedge_origin_is_convex( he4 ) )
      {
        he4_is_convex = true;
      }
      else
      {
        reflex_vertices.push_back( he4 );
      }

      if ( halfedge_origin_is_convex( he5 ) )
      {
        he5_is_convex = true;
      }
      else
      {
        reflex_vertices.push_back( he5 );
      }

      if ( he4_is_convex && halfedge_origin_is_ear( he4 ) )
      {
        ears.push_back( he4 );
      }

      if ( he5_is_convex && halfedge_origin_is_ear( he5 ) )
      {
        ears.push_back( he5 );
      }
    }
    else
    {
      tria.add_face( he1, he2, he5 );
    }
  }
}

template <typename Triangulation>
bool Triangulator<Triangulation>::halfedge_origin_is_convex( Halfedge_handle he ) const
{
  Halfedge_handle hep = he->prev();
  Point2 p1 = hep->origin()->position();
  Point2 p2 = he->origin()->position();
  Point2 p3 = he->pair()->origin()->position();

  if ( Kernel::oriented_side( p1, p2, p3 ) == ON_POSITIVE_SIDE )
  {
    return true;
  }
  else
  {
    return false;
  }
}

template <typename Triangulation>
bool Triangulator<Triangulation>::halfedge_origin_is_ear( Halfedge_handle he ) const
{
  BOOST_ASSERT( this->halfedge_origin_is_convex( he ) );

  Node_handle n1 = he->prev()->origin();
  Node_handle n2 = he->origin();
  Node_handle n3 = he->pair()->origin();
  Point2 p1 = n1->position();
  Point2 p2 = n2->position();
  Point2 p3 = n3->position();

  /* to test if a vertex is an ear, we just need to iterate over reflex
   * vertices */
  BOOST_FOREACH( Halfedge_handle refl_he, reflex_vertices )
  {
    Node_handle refl_node = refl_he->origin();

    if ( refl_node != n1 && refl_node != n3 )
    {
      Point2 p = refl_node->position();
      Oriented_side os1 = Kernel::oriented_side( p1, p2, p );
      Oriented_side os2 = Kernel::oriented_side( p2, p3, p );
      Oriented_side os3 = Kernel::oriented_side( p3, p1, p );

      if ( os1 != ON_NEGATIVE_SIDE &&
           os2 != ON_NEGATIVE_SIDE &&
           os3 != ON_NEGATIVE_SIDE )
      {
        return false;
      }
    }
  }
  return true;
}

} // namespace umeshu

#endif /* __TRIANGULATOR_H_INCLUDED__ */
