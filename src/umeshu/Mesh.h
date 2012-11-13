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

#ifndef __MESH_H_INCLUDED__
#define __MESH_H_INCLUDED__

#include "BoundarySegment.h"
#include "BoundingBox.h"
#include "Edge.h"
#include "Face.h"
#include "Mesh_storage.h"
#include "Node.h"
#include "Postscript_ostream.h"

#include <boost/assert.hpp>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>

namespace umeshu {

template <typename Kernel>
class Mesh : public boost::noncopyable {
public:
    typedef enum {IN_FACE, ON_EDGE, ON_NODE, OUTSIDE_MESH} Point2_location;

    typedef Kernel                                  Kernel_type;
    typedef Mesh<Kernel_type>                       Self;
    typedef Mesh_storage_list<Self>                 Storage;

    typedef typename Storage::Node_handle           Node_handle;
    typedef typename Storage::Edge_handle           Edge_handle;
    typedef Halfedge<Self>*                         Halfedge_handle;
    typedef typename Storage::Face_handle           Face_handle;

    typedef typename Storage::Node_const_handle     Node_const_handle;
    typedef typename Storage::Edge_const_handle     Edge_const_handle;
    typedef Halfedge<Self> const*                   Halfedge_const_handle;
    typedef typename Storage::Face_const_handle     Face_const_handle;

    typedef typename Storage::Node_iterator         Node_iterator;
    typedef typename Storage::Edge_iterator         Edge_iterator;
    typedef typename Storage::Face_iterator         Face_iterator;
    typedef typename Storage::Node_const_iterator   Node_const_iterator;
    typedef typename Storage::Edge_const_iterator   Edge_const_iterator;
    typedef typename Storage::Face_const_iterator   Face_const_iterator;
    
    Node_handle add_node (Point2 const& p) { return storage_.insert(Node<Self>(p)); }
    void        remove_node (Node_handle n);

    Edge_handle add_edge (Node_handle n1, Node_handle n2, BoundarySegment *bseginfo) {
        BOOST_ASSERT(n1 != n2);
        Edge_handle e = storage_.insert(Edge<Self>(n1, n2, bseginfo));
        e->set_edge_handle(e);
        this->attach_edge(e, n1);
        this->attach_edge(e, n2);
        return e;
    }

    void remove_edge (Edge_handle e);

    Node_handle split_edge (Edge_handle e) {
        return this->split_edge(e, e->midpoint());
    }

    Node_handle split_edge (Edge_handle e, Point2 const& p) {
        Halfedge_handle he1 = e->he1();
        Halfedge_handle he2 = e->he2();

        Halfedge_handle he1n, he1p;
        if (not he1->is_boundary()) {
            he1n = he1->next();
            he1p = he1->prev();
        }

        Halfedge_handle he2n, he2p;
        if (not he2->is_boundary()) {
            he2n = he2->next();
            he2p = he2->prev();
        }

        Node_handle n1 = he1->origin();
        Node_handle n2 = he2->origin();
        BoundarySegment *bseginfo = e->boundary_info();
        this->remove_edge(e);
        Node_handle n  = this->add_node(p);
        Edge_handle e1 = this->add_edge(n1, n, bseginfo);
        Edge_handle e2 = this->add_edge(n2, n, bseginfo);
        if (not he1->is_boundary()) {
            Edge_handle e3 = this->add_edge(he1p->origin(), n, NULL);
            this->add_face(he1p, e1->halfedge_with_origin(n1), e3->halfedge_with_origin(n));
            this->add_face(he1n, e3->halfedge_with_origin(n)->pair(), e2->halfedge_with_origin(n));
        }
        if (not he2->is_boundary()) {
            Edge_handle e4 = this->add_edge(he2p->origin(), n, NULL);
            this->add_face(he2p, e2->halfedge_with_origin(n2), e4->halfedge_with_origin(n));
            this->add_face(he2n, e4->halfedge_with_origin(n)->pair(), e1->halfedge_with_origin(n));
        }

        return n;
    }

    Edge_handle swap_edge (Edge_handle e) {   
        BOOST_ASSERT(not e->is_boundary());

        Halfedge_handle he1 = e->he1()->next();
        Halfedge_handle he2 = e->he1()->prev();
        Halfedge_handle he3 = e->he2()->next();
        Halfedge_handle he4 = e->he2()->prev();
        Node_handle n1 = he2->origin();
        Node_handle n2 = he4->origin();

        this->remove_edge(e);
        Edge_handle new_edge = this->add_edge(n1, n2, NULL);
        this->add_face(he2, he3, new_edge->halfedge_with_origin(n2));
        this->add_face(he4, he1, new_edge->halfedge_with_origin(n1));

        return new_edge;
    }

    Face_handle add_face (Halfedge_handle he1, Halfedge_handle he2, Halfedge_handle he3) {
        if (not(he1->is_boundary() &&
                he2->is_boundary() &&
                he3->is_boundary())) {
            std::clog << "Mesh<Kernel>::add_face: half-edges are not free, cannot add face\n";
            return Face_handle();
        }

        if (not(he1->pair()->origin() == he2->origin() &&
                he2->pair()->origin() == he3->origin() &&
                he3->pair()->origin() == he1->origin()))
        {
            std::clog << "Mesh<Kernel>::add_face: half-edges do not form a chain, cannot add face\n";
            return Face_handle();
        }

        if (not(make_adjacent(he1, he2) &&
                make_adjacent(he2, he3) &&
                make_adjacent(he3, he1)))
        {
            std::clog << "Mesh<Kernel>::add_face: attempting to create non-manifold mesh, cannot add face\n";
            return Face_handle();
        }

        Face_handle f = storage_.insert(Face<Self>(he1));
        he1->face() = he2->face() = he3->face() = f;
        return f;
    }

    void remove_face (Face_handle f);

    Node_handle split_face  (Face_handle f, Point2 const& p) {
        Halfedge_handle he1 = f->halfedge();
        Halfedge_handle he2 = he1->next();
        Halfedge_handle he3 = he2->next();

        this->remove_face(f);

        Node_handle n  = this->add_node(p);
        Edge_handle e1 = this->add_edge(n, he1->origin(), NULL);
        Edge_handle e2 = this->add_edge(n, he2->origin(), NULL);
        Edge_handle e3 = this->add_edge(n, he3->origin(), NULL);
        this->add_face(he1, e2->halfedge_with_origin(n)->pair(), e1->halfedge_with_origin(n));
        this->add_face(he2, e3->halfedge_with_origin(n)->pair(), e2->halfedge_with_origin(n));
        this->add_face(he3, e1->halfedge_with_origin(n)->pair(), e3->halfedge_with_origin(n));

        return n;
    }

    BoundingBox bounding_box () const;

    Halfedge_handle boundary_halfedge() const {
        for (Edge_iterator iter = edges_begin(); iter != edges_end(); ++iter) {
            if (iter->he1()->is_boundary()) return iter->he1();
            if (iter->he2()->is_boundary()) return iter->he2();
        }
        return Halfedge_handle();
    }

    double smallest_angle() const;

    Node_const_iterator nodes_begin() const { return storage_.nodes_begin(); }
    Node_const_iterator nodes_end()   const { return storage_.nodes_end(); }
    Edge_const_iterator edges_begin() const { return storage_.edges_begin(); }
    Edge_const_iterator edges_end()   const { return storage_.edges_end(); }
    Face_const_iterator faces_begin() const { return storage_.faces_begin(); }
    Face_const_iterator faces_end()   const { return storage_.faces_end(); }
    Node_iterator       nodes_begin()       { return storage_.nodes_begin(); }
    Node_iterator       nodes_end()         { return storage_.nodes_end(); }
    Edge_iterator       edges_begin()       { return storage_.edges_begin(); }
    Edge_iterator       edges_end()         { return storage_.edges_end(); }
    Face_iterator       faces_begin()       { return storage_.faces_begin(); }
    Face_iterator       faces_end()         { return storage_.faces_end(); }

    size_t number_of_nodes () const { return storage_.nodes_size(); }
    size_t number_of_edges () const { return storage_.edges_size(); }
    size_t number_of_faces () const { return storage_.faces_size(); }

    void debug_print(std::ostream& os);
private:
    void attach_edge   (Edge_handle e, Node_handle n);
    void detach_edge   (Edge_handle e, Node_handle n);
    bool make_adjacent (Halfedge_handle in, Halfedge_handle out);

    template <typename> friend class Mesher;
    template <typename> friend class Triangulator;

    Mesh_storage_list<Self> storage_;
};

template <typename Kernel>
void Mesh<Kernel>::debug_print(std::ostream& os)
{
    os << "Nodes:\n";
    Node_const_iterator niter = nodes_begin();
    for(; niter != nodes_end(); ++niter) {
        os << &(*niter) << ": " << niter->position() << std::endl;
    }
    os << "Edges:\n";
    Edge_const_iterator eiter = edges_begin();
    for(; eiter != edges_end(); ++eiter) {
        os << &(*eiter) << ": " << eiter->he1()->origin()->position() << "--"
                                << eiter->he2()->origin()->position() << std::endl;
    }
    os << "Faces:\n";
    Face_const_iterator fiter = faces_begin();
    for(; fiter != faces_end(); ++fiter) {
        Point2 p1, p2, p3;
        fiter->vertices(p1, p2, p3);
        os << &(*fiter) << ": " << p1 << "--" << p2 << "--" << p3 << std::endl;
    }
}

template <typename Kernel>
void Mesh<Kernel>::remove_node (Node_handle n)
{
    Halfedge_handle next = n->halfedge();
    while (not n->is_isolated()) {
        Halfedge_handle cur = next;
        next = cur->pair()->next();
        this->remove_edge(cur->edge());
    }
    storage_.erase(n);
}

template <typename Kernel>
void Mesh<Kernel>::attach_edge (Edge_handle e, Node_handle n)
{
    Halfedge_handle out_he = e->halfedge_with_origin(n);
    
    if (n->is_isolated()) {
        n->set_halfedge(out_he);
    } else {
        Halfedge_handle free_in_he = n->find_free_incident_halfedge();
        // TODO: better handling of this situation (which should not normally happen)
        BOOST_ASSERT_MSG(free_in_he != Halfedge_handle(), "Did not find free incident half-edge");
        
        Halfedge_handle free_out_he = free_in_he->next();        
        BOOST_ASSERT(free_out_he->is_boundary());
        
        free_in_he->set_next(out_he);
        out_he->set_prev(free_in_he);
        out_he->pair()->set_next(free_out_he); 
        free_out_he->set_prev(out_he->pair());
    }
}

template <typename Kernel>
void Mesh<Kernel>::remove_edge (Edge_handle e)
{
    // remove elements on both sides of the edge
    if (not e->he1()->is_boundary()) {
        this->remove_face(e->he1()->face());
    }
    if (not e->he2()->is_boundary()) {
        this->remove_face(e->he2()->face());
    }

    this->detach_edge(e, e->he1()->origin());
    this->detach_edge(e, e->he2()->origin());
    
    storage_.erase(e);
}

template <typename Kernel>
void Mesh<Kernel>::detach_edge (Edge_handle e, Node_handle n)
{
    Halfedge_handle he = e->halfedge_with_origin(n);
    
    if (n->halfedge() == he) {
        if (he->pair()->next() != he) {
            n->halfedge() = he->pair()->next();
        } else {
            n->halfedge() = Halfedge_handle();
        }
    }
    
    he->prev()->next() = he->pair()->next();
    he->pair()->next()->prev() = he->prev();
}

template <typename Kernel>
bool Mesh<Kernel>::make_adjacent (Halfedge_handle in, Halfedge_handle out)
{
    // half-edges are already adjacent
    if (in->next() == out)
        return true;
    
    Halfedge_handle b = in->next();
    Halfedge_handle d = out->prev();
    
    Node_handle n = out->origin();
    Halfedge_handle g = n->find_free_incident_halfedge_in_range(out->pair(), in);
    // half-edges cannot be made adjacent
    if (g == Halfedge_handle())
        return false;
    
    Halfedge_handle h = g->next();
    in->set_next(out);
    out->set_prev(in);
    g->set_next(b);
    b->set_prev(g);
    d->set_next(h);
    h->set_prev(d);
    
    return true;
}

template <typename Kernel>
void Mesh<Kernel>::remove_face (Face_handle f)
{    
    Halfedge_handle he_start = f->halfedge();
    Halfedge_handle he_iter = he_start;
    do {
        he_iter->face() = Face_handle();
        he_iter = he_iter->next();
    } while (he_iter != he_start);
    
    storage_.erase(f);
}


template <typename Kernel>
BoundingBox Mesh<Kernel>::bounding_box () const
{
    BoundingBox bb;
    for (Node_const_iterator iter = nodes_begin(); iter != nodes_end(); ++iter) {
        bb.include(iter->position());
    }
    return bb;
}

template <typename Kernel>
double Mesh<Kernel>::smallest_angle() const
{
    double sa = std::numeric_limits<double>::max();
    for (Face_iterator iter = faces_begin(); iter != faces_end(); ++iter) {
        double a = iter->minimum_angle();
        if (a < sa) {
            sa = a;
        }
    }
    return sa;
}

template<typename Kernel>
Postscript_ostream& operator<< (Postscript_ostream& ps, Mesh<Kernel> const& m)
{
    typedef typename Mesh<Kernel>::Node_const_iterator Node_iterator;
    typedef typename Mesh<Kernel>::Edge_const_iterator Edge_iterator;
    typedef typename Mesh<Kernel>::Face_const_iterator Face_iterator;

    ps.setgray(0.8);
    for (Face_iterator iter = m.faces_begin(); iter != m.faces_end(); ++iter) {
        ps << *iter;
        ps.fill();
    }
    
    ps.setgray(0);
    ps.newpath();
    for (Edge_iterator iter = m.edges_begin(); iter != m.edges_end(); ++iter) {
        ps << *iter;
    }
    ps.stroke();

    ps.setrgbcolor(1,0,0);
    for (Node_iterator iter = m.nodes_begin(); iter != m.nodes_end(); ++iter) {
        ps << iter->position();
        ps.fill();
    }

    return ps;
}

template <typename Mesh>
void locate_point(Point2 const& p, typename Mesh::Face_handle start_face, typename Mesh::Point2_location& loc, void*& entity)
{
    typedef typename Mesh::Kernel_type     Kernel_type;
    typedef typename Mesh::Halfedge_handle Halfedge_handle;

    Halfedge_handle he_iter = start_face->halfedge();
    Halfedge_handle he_start = he_iter;
    Point2 q = p;
    
    while (true) {
        Oriented_side os = Kernel_type::oriented_side(he_iter, q);
        switch (os) {
            case ON_POSITIVE_SIDE:
                he_iter = he_iter->next();
                if (he_iter == he_start) {
                    entity = he_iter->face();
                    loc = Mesh::IN_FACE;
                    return;
                }
                break;
            case ON_ORIENTED_BOUNDARY:
            {
                Point2 p1 = he_iter->origin()->position();
                Point2 p2 = he_iter->pair()->origin()->position();
                if ((std::min(p1.x(),p2.x()) < q.x() && q.x() < std::max(p1.x(),p2.x())) ||
                    (std::min(p1.y(),p2.y()) < q.y() && q.y() < std::max(p1.y(),p2.y())) )
                {
                    entity = he_iter->edge();
                    loc = Mesh::ON_EDGE;
                    return;
                } else if (q == p1) {
                    entity = he_iter->origin();
                    loc = Mesh::ON_NODE;
                    return;
                } else if (q == p2) {
                    entity = he_iter->pair()->origin();
                    loc = Mesh::ON_NODE;
                    return;
                }
            }
            case ON_NEGATIVE_SIDE:
                he_iter = he_iter->pair();
                if ( he_iter->is_boundary() )
                {
                    loc = Mesh::OUTSIDE_MESH;
                    entity = he_iter->pair();
                    return;
                }
                he_start = he_iter;
                he_iter = he_iter->next();
        }
    }
}

} // namespace umeshu

#endif // __MESH_H_INCLUDED__
