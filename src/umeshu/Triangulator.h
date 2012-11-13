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

#ifndef __TRIANGULATOR_H_INCLUDED__
#define __TRIANGULATOR_H_INCLUDED__ 

#include "Exceptions.h"
#include "Polygon.h"

#include <boost/foreach.hpp>

#include <list>

namespace umeshu {

template <typename Triangulation>
class Triangulator {
public:
    typedef          Triangulation               Tria;
    typedef typename Tria::Kernel                Kernel;
    typedef typename Tria::Point_2               Point_2;

    typedef typename Tria::Node_handle           Node_handle;
    typedef typename Tria::Halfedge_handle       Halfedge_handle;
    typedef typename Tria::Edge_handle           Edge_handle;
    typedef typename Tria::Face_handle           Face_handle;
    typedef typename Tria::Node_const_handle     Node_const_handle;
    typedef typename Tria::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Tria::Edge_const_handle     Edge_const_handle;
    typedef typename Tria::Face_const_handle     Face_const_handle;

    void triangulate(Polygon const& poly, Tria &tria);

    struct triangulator_error : virtual umeshu_error { };

private:
    Halfedge_handle add_polygon_to_triangulation(Polygon const& poly, Tria& tria) {
        Polygon::vertex_const_iterator iter = poly.vertices_begin();
        Node_handle first_node = tria.add_node(*iter);
        Node_handle prev_node = first_node;
        ++iter;
        for (; iter != poly.vertices_end(); ++iter) {
            Node_handle cur_node = tria.add_node(*iter);
            tria.add_edge(prev_node, cur_node);
            prev_node = cur_node;
        }
        return tria.add_edge(prev_node, first_node);
    }

    bool halfedge_origin_is_convex(Halfedge_handle he) const;
    bool halfedge_origin_is_ear(Halfedge_handle he) const;

    void classify_vertices(Halfedge_handle bhe) {
        Halfedges convex_vertices;

        Halfedge_handle he_iter = bhe;
        do {
            if (halfedge_origin_is_convex(he_iter)) {
                convex_vertices.push_back(he_iter);
            } else {
                reflex_vertices.push_back(he_iter);
            }
            he_iter = he_iter->next();
        } while (he_iter != bhe);

        BOOST_FOREACH(Halfedge_handle conv_he, convex_vertices) {
            if (this->halfedge_origin_is_ear(conv_he)) {
                ears.push_back(conv_he);
            }
        }
    }

    typedef std::list<Halfedge_handle> Halfedges;
    Halfedges reflex_vertices, ears;
};

template <typename Triangulation>
void Triangulator<Triangulation>::triangulate(Polygon const& poly, Triangulation &tria)
{
    if (poly.number_of_vertices() < 3) {
        throw triangulator_error();
    }

    this->classify_vertices(this->add_polygon_to_triangulation(poly, tria));

    while (not ears.empty()) {
        Halfedge_handle he2 = *ears.begin();
        Halfedge_handle he1 = he2->prev();
        Halfedge_handle he5 = he2->next();
        Node_handle n1 = he1->origin();
        Node_handle n3 = he5->origin();

        // since we will cut it off, remove the ear from ears. Also, we have to
        // erase he1 and he5 from all the sets, since after cutting the ear we
        // will have to update their info anyway
        ears.remove(he2);
        ears.remove(he1);
        reflex_vertices.remove(he1);
        ears.remove(he5);
        reflex_vertices.remove(he5);

        // if this is not the last ear, i.e., only one triangle left to
        // triangulate
        if (he5 != he1->prev()) {
            Halfedge_handle he3 = tria.add_edge(n3, n1);
            Halfedge_handle he4 = he3->pair();
            tria.add_face(he1, he2, he3);

            bool he4_is_convex = false, he5_is_convex = false;
            if (halfedge_origin_is_convex(he4)) {
                he4_is_convex = true;
            } else {
                reflex_vertices.push_back(he4);
            }
            if (halfedge_origin_is_convex(he5)) {
                he5_is_convex = true;
            } else {
                reflex_vertices.push_back(he5);
            }
            if (he4_is_convex && halfedge_origin_is_ear(he4)) {
                ears.push_back(he4);
            }
            if (he5_is_convex && halfedge_origin_is_ear(he5)) {
                ears.push_back(he5);
            }
        } else {
            tria.add_face(he1, he2, he5);
        }
    }
}

template <typename Triangulation>
bool Triangulator<Triangulation>::halfedge_origin_is_convex(Halfedge_handle he) const
{
    Halfedge_handle hep = he->prev();
    Point_2 p1 = hep->origin()->position();
    Point_2 p2 = he->origin()->position();
    Point_2 p3 = he->pair()->origin()->position();

    if (Kernel::oriented_side(p1, p2, p3) == Kernel::ON_POSITIVE_SIDE) {
        return true;
    } else {
        return false;
    }
}

template <typename Triangulation>
bool Triangulator<Triangulation>::halfedge_origin_is_ear(Halfedge_handle he) const
{
    BOOST_ASSERT(this->halfedge_origin_is_convex(he));

    Node_handle n1 = he->prev()->origin();
    Node_handle n2 = he->origin();
    Node_handle n3 = he->pair()->origin();
    Point_2 p1 = n1->position();
    Point_2 p2 = n2->position();
    Point_2 p3 = n3->position();

    /* to test if a vertex is an ear, we just need to iterate over reflex
     * vertices */
    BOOST_FOREACH(Halfedge_handle refl_he, reflex_vertices) {
        Node_handle refl_node = refl_he->origin();
        if (refl_node != n1 && refl_node != n3) {
            typename Kernel::Oriented_side os1, os2, os3;
            Point_2 p = refl_node->position();
            os1 = Kernel::oriented_side(p1, p2, p);
            os2 = Kernel::oriented_side(p2, p3, p);
            os3 = Kernel::oriented_side(p3, p1, p);
            if (os1 != Kernel::ON_NEGATIVE_SIDE &&
                os2 != Kernel::ON_NEGATIVE_SIDE &&
                os3 != Kernel::ON_NEGATIVE_SIDE ) {
                return false;
            }
        }
    }
    return true;
}

} // namespace umeshu

#endif /* __TRIANGULATOR_H_INCLUDED__ */
