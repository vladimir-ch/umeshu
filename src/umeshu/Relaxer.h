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

#ifndef UMESHU_RELAXER_H
#define UMESHU_RELAXER_H

#include "Point2.h"
#include "Utils.h"

#include <boost/assert.hpp>

#include <cmath>

namespace umeshu {

template <typename Triangulation>
class Relaxer {
public:
    typedef          Relaxer<Triangulation>      Self;
    typedef          Triangulation               Tria;
    typedef typename Tria::Kernel                Kernel;

    typedef typename Tria::Node_handle           Node_handle;
    typedef typename Tria::Halfedge_handle       Halfedge_handle;
    typedef typename Tria::Edge_handle           Edge_handle;
    typedef typename Tria::Face_handle           Face_handle;

    void relax(Tria &tria) {
        for (int relax = 4; relax >=2; --relax)
        {
            bool flipped = true;
            while (flipped) {
                flipped = false;
                typename Tria::Edge_iterator edge_iter = tria.edges_begin();
                for (; edge_iter != tria.edges_end(); ++edge_iter) {
                    Edge_handle edge = edge_iter;
                    if (edge->is_boundary()) {
                        continue;
                    }

                    int index = edge_relaxation_index(edge);
                    if (index > relax) {
                        if (edge->is_diagonal_of_convex_quadrilateral()) {
                            edge->flip();
                            flipped = true;
                        }
                    }
                }
            }
        }

        // gboolean swapped = TRUE;
        // while ( swapped )
        // {
        // swapped = FALSE;
        // GList * edges_iter = mesh->edges;
        // while( edges_iter != NULL )
        // {
        // Edge *edge = EDGE( edges_iter->data );
        // edges_iter = g_list_next( edges_iter );

        // if ( edge_is_at_boundary( edge ) )
        // continue;

        // Node *n1 = edge->he[0].origin;
        // if ( node_is_at_boundary( n1 ) )
        // continue;
        // Node *n2 = edge->he[1].origin;
        // if ( node_is_at_boundary( n2 ) )
        // continue;

        // if ( node_degree( n1 ) + node_degree( n2 ) == 10 )
        // {
        // mesh_collapse_edge( mesh, edge );
        // edges_iter = mesh->edges;
        // }
        // }
        // }
    }

    struct relaxer_error : virtual umeshu_error { };

private:
    int ideal_degree(Node_handle n) const {
        // ideal degree for an interior point
        int D = 6;

        Halfedge_handle he2 = n->boundary_halfedge();
        // compute ideal degree for a boundary node
        if (he2 != Halfedge_handle()) {
            Halfedge_handle he1 = he2->prev();

            Point2 const& p1 = he1->origin()->position();
            Point2 const& p2 = he2->origin()->position();
            Point2 const& p3 = he2->pair()->origin()->position();

            double v1[2], v2[2];
            v1[0] = p1.x() - p2.x();
            v1[1] = p1.y() - p2.y();
            v2[0] = p3.x() - p2.x();
            v2[1] = p3.y() - p2.y();

            double angle = ::atan2( v1[0]*v2[1] - v2[0]*v1[1], v1[0]*v2[0] + v1[1]*v2[1] );
            if (angle < 0.0) {
                angle = 2.0*M_PI + angle;
            }

            if (angle <= utils::degrees_to_radians(84.85)) {
                D = 2;
            } else if (angle >  utils::degrees_to_radians( 84.85) &&
                       angle <= utils::degrees_to_radians(146.97)) {
                D = 3;
            } else if (angle >  utils::degrees_to_radians(146.97) &&
                       angle <= utils::degrees_to_radians(207.85)) {
                D = 4;
            } else if (angle >  utils::degrees_to_radians(207.85) &&
                       angle <= utils::degrees_to_radians(268.33)) {
                D = 5;
            } else if (angle >  utils::degrees_to_radians(268.33) &&
                       angle <= utils::degrees_to_radians(328.63)) {
                D = 6;
            } else {
                BOOST_ASSERT(angle > utils::degrees_to_radians(328.63) &&
                             angle < utils::degrees_to_radians(360.0));
                D = 7;
            }
        }
        return D;
    }

    int virtual_degree(Node_handle n) const {
        return n->degree() + (6 - ideal_degree(n));
    }

    int edge_relaxation_index(Edge_handle e) const {
        Node_handle n1 = e->he1()->origin();
        Node_handle n2 = e->he2()->origin();
        Node_handle n3 = e->he1()->prev()->origin();
        Node_handle n4 = e->he2()->prev()->origin();
        return virtual_degree(n1) + virtual_degree(n2) - virtual_degree(n3) - virtual_degree(n4);
    }
};

} // namespace umeshu

#endif /* __RELAXER_H_INCLUDED__ */
