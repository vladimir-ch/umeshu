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

#define BOOST_TEST_MODULE Triangulation
#include <boost/test/unit_test.hpp>
#include <cmath>

#include "io/Postscript_ostream.h"
#include "Triangulation_items.h"
#include "Triangulation.h"

using namespace umeshu;

typedef Triangulation<Triangulation_items> Tria;
typedef Tria::Node_handle             Node_handle;
typedef Tria::Halfedge_handle         Halfedge_handle;
typedef Tria::Edge_handle             Edge_handle;
typedef Tria::Face_handle             Face_handle;

BOOST_AUTO_TEST_CASE(construction_and_access)
{
    Tria tria;
    Node_handle n1 = tria.add_node(Point2(0.0, 0.0));
    BOOST_CHECK(n1->position().x() == 0.0);
    BOOST_CHECK(n1->position().y() == 0.0);
    BOOST_CHECK(n1->is_isolated());
    BOOST_CHECK(tria.number_of_nodes() == 1);
    BOOST_CHECK(tria.number_of_edges() == 0);
    BOOST_CHECK(tria.number_of_faces() == 0);
    
    Node_handle n2 = tria.add_node(Point2(1.0, 0.0));
    BOOST_CHECK(n2->position().x() == 1.0);
    BOOST_CHECK(n2->position().y() == 0.0);
    BOOST_CHECK(n2->is_isolated());
    BOOST_CHECK(tria.number_of_nodes() == 2);
    BOOST_CHECK(tria.number_of_edges() == 0);
    BOOST_CHECK(tria.number_of_faces() == 0);

    Node_handle n3 = tria.add_node(Point2(0.0, 1.0));
    BOOST_CHECK(n3->position().x() == 0.0);
    BOOST_CHECK(n3->position().y() == 1.0);
    BOOST_CHECK(n3->is_isolated());
    BOOST_CHECK(tria.number_of_nodes() == 3);
    BOOST_CHECK(tria.number_of_edges() == 0);
    BOOST_CHECK(tria.number_of_faces() == 0);

    Halfedge_handle he1 = tria.add_edge(n1, n2);
    Halfedge_handle he1p = he1->pair();
    BOOST_CHECK(he1->origin() == n1);
    BOOST_CHECK(he1p->origin() == n2);
    BOOST_CHECK(he1->next() == he1p);
    BOOST_CHECK(he1->prev() == he1p);
    BOOST_CHECK(he1p->next() == he1);
    BOOST_CHECK(he1p->prev() == he1);
    BOOST_CHECK(tria.number_of_nodes() == 3);
    BOOST_CHECK(tria.number_of_halfedges() == 2);
    BOOST_CHECK(tria.number_of_edges() == 1);
    BOOST_CHECK(tria.number_of_faces() == 0);
    
    Halfedge_handle he2 = tria.add_edge(n2, n3);
    Halfedge_handle he3 = tria.add_edge(n3, n1);
    BOOST_CHECK(tria.number_of_nodes() == 3);
    BOOST_CHECK(tria.number_of_halfedges() == 6);
    BOOST_CHECK(tria.number_of_edges() == 3);
    BOOST_CHECK(tria.number_of_faces() == 0);

    Face_handle f = tria.add_face(he1, he2, he3);
    BOOST_CHECK(tria.number_of_nodes() == 3);
    BOOST_CHECK(tria.number_of_halfedges() == 6);
    BOOST_CHECK(tria.number_of_edges() == 3);
    BOOST_CHECK(tria.number_of_faces() == 1);

    tria.remove_face(f);
    BOOST_CHECK(tria.number_of_nodes() == 3);
    BOOST_CHECK(tria.number_of_halfedges() == 6);
    BOOST_CHECK(tria.number_of_edges() == 3);
    BOOST_CHECK(tria.number_of_faces() == 0);

    f = tria.add_face(he1, he2, he3);
    tria.remove_edge(he1->edge());
    BOOST_CHECK(tria.number_of_nodes() == 3);
    BOOST_CHECK(tria.number_of_halfedges() == 4);
    BOOST_CHECK(tria.number_of_edges() == 2);
    BOOST_CHECK(tria.number_of_faces() == 0);

    he1 = tria.add_edge(n1, n2);
    f = tria.add_face(he1, he2, he3);
    tria.remove_node(n1);
    BOOST_CHECK(tria.number_of_nodes() == 2);
    BOOST_CHECK(tria.number_of_halfedges() == 2);
    BOOST_CHECK(tria.number_of_edges() == 1);
    BOOST_CHECK(tria.number_of_faces() == 0);
}

BOOST_AUTO_TEST_CASE(insertion_in_edge)
{
    Tria tria;
    Node_handle n1 = tria.add_node(Point2(0.0, 0.0));
    Node_handle n2 = tria.add_node(Point2(1.0, 0.0));
    Node_handle n3 = tria.add_node(Point2(1.0, 1.0));
    Node_handle n4 = tria.add_node(Point2(0.0, 1.0));
    Halfedge_handle h1 = tria.add_edge(n1, n2);
    Halfedge_handle h2 = tria.add_edge(n2, n3);
    Halfedge_handle h3 = tria.add_edge(n3, n4);
    Halfedge_handle h4 = tria.add_edge(n4, n1);
    Halfedge_handle h5 = tria.add_edge(n3, n1);
    tria.add_face(h1, h2, h5);
    tria.add_face(h3, h4, h5->pair());
    BOOST_CHECK(tria.number_of_nodes() == 4);
    BOOST_CHECK(tria.number_of_halfedges() == 10);
    BOOST_CHECK(tria.number_of_edges() == 5);
    BOOST_CHECK(tria.number_of_faces() == 2);

    {
        io::Postscript_ostream ps("insert_in_edge_1.eps", tria.bounding_box());
        ps << tria;
    }
    tria.insert_in_edge(h5->edge(), Point2(0.5,0.5));
    BOOST_CHECK(tria.number_of_nodes() == 5);
    BOOST_CHECK(tria.number_of_halfedges() == 16);
    BOOST_CHECK(tria.number_of_edges() == 8);
    BOOST_CHECK(tria.number_of_faces() == 4);
    {
        io::Postscript_ostream ps("insert_in_edge_2.eps", tria.bounding_box());
        ps << tria;
    }
}
