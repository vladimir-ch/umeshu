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

#ifndef __DELAUNAY_MESHER_H_INCLUDED__
#define __DELAUNAY_MESHER_H_INCLUDED__ 

#include "Triangulation.h"
#include "Utils.h"

#include <boost/unordered/unordered_set.hpp>

#include <set>
#include <stack>

namespace umeshu {

template <typename Delaunay_triangulation>
class Delaunay_mesh_area_quality {
public:
    typedef          Delaunay_mesh_area_quality<Delaunay_triangulation> Self;
    typedef          Delaunay_triangulation      Tria;
    typedef typename Tria::Kernel                Kernel;

    typedef typename Tria::Node_handle           Node_handle;
    typedef typename Tria::Halfedge_handle       Halfedge_handle;
    typedef typename Tria::Edge_handle           Edge_handle;
    typedef typename Tria::Face_handle           Face_handle;

    Delaunay_mesh_area_quality(Face_handle f) : face_(f) {
        Point2 p1, p2, p3;
        face_->vertices(p1, p2, p3);
        area_ = Kernel::signed_area(p1, p2, p3);
        BOOST_ASSERT(area_ > 0.0);
        double a1, a2, a3;
        Kernel::triangle_angles(p1, p2, p3, a1, a2, a3);
        min_angle_ = std::min(a1, std::min(a2, a3));
    }

    Face_handle face() const { return face_; }
    double area() const { return area_; }
    double min_angle() const { return min_angle_; }

    bool operator< (Self const& q) const {
        if (q.face() != face()) {
            if (area() > q.area()) {
                return true;
            } else if (area() < q.area()) {
                return false;
            } else { // areas are equal
                if (min_angle() < q.min_angle()) {
                    return true;
                } else if (min_angle() > q.min_angle()) {
                    return false;
                } else { // areas and min angles are equal
                    return &(*(face())) < &(*(q.face()));
                }
            }
        }
        return false;
    }

private:
    Face_handle face_;
    double area_;
    double min_angle_;
};

template <typename Delaunay_triangulation, typename Quality = Delaunay_mesh_area_quality<Delaunay_triangulation> >
class Delaunay_mesher {
public:
    typedef          Delaunay_triangulation      Tria;
    typedef typename Tria::Kernel                Kernel;

    typedef typename Tria::Node                  Node;
    typedef typename Tria::Halfedge              Halfedge;
    typedef typename Tria::Edge                  Edge;
    typedef typename Tria::Face                  Face;

    typedef typename Tria::Node_iterator         Node_iterator;
    typedef typename Tria::Edge_iterator         Edge_iterator;
    typedef typename Tria::Face_iterator         Face_iterator;

    typedef typename Tria::Node_const_iterator   Node_const_iterator;
    typedef typename Tria::Edge_const_iterator   Edge_const_iterator;
    typedef typename Tria::Face_const_iterator   Face_const_iterator;

    typedef typename Tria::Node_handle           Node_handle;
    typedef typename Tria::Halfedge_handle       Halfedge_handle;
    typedef typename Tria::Edge_handle           Edge_handle;
    typedef typename Tria::Face_handle           Face_handle;

    struct Halfedge_handle_hash {
        size_t operator()(Halfedge_handle const& e) const 
        { 
            return boost::hash<Halfedge*>()(&(*e)); 
        }
    };

    typedef boost::unordered_set<Halfedge_handle, Halfedge_handle_hash> Encroached_halfedges;
    typedef std::set<Quality> Bad_faces;
    typedef std::stack<Edge_handle> Undo_stack;

    explicit Delaunay_mesher ()
        : mesh_(NULL)
        , max_area_(1.0)
        , min_angle_(utils::degrees_to_radians(20.0))
    {}

    void refine (Delaunay_triangulation& mesh, double max_area, double min_angle) {
        mesh_ = &mesh;
        max_area_ = max_area;
        min_angle_ = utils::degrees_to_radians(min_angle);

        collect_encroached_boundary_edges();
        split_encroached_boundary_edges(false);
        BOOST_ASSERT(bad_faces_.empty());

        for (Face_iterator iter = mesh_->faces_begin(); iter != mesh_->faces_end(); ++iter) {
            enqueue_bad_face(iter);
        }

        while (not bad_faces_.empty()) {
            Face_handle bad_face = bad_faces_.begin()->face();

            Node_handle n1, n2, n3;
            bad_face->nodes(n1, n2, n3);

            Point2 center = Kernel::circumcenter( n1->position(), n2->position(), n3->position() );

            Point_location loc;
            Face_handle face_to_kill;
            Edge_handle edge_to_kill;
            Node_handle node_to_kill;
            face_to_kill = mesh_->locate(center, loc, node_to_kill, edge_to_kill, bad_face);
            BOOST_ASSERT(loc != ON_NODE);

            std::stack<Halfedge_handle> E;
            if (loc == IN_FACE) {
                Node_handle new_node = try_kill_face(face_to_kill, center, E);
                if (E.empty()) {
                    clear_undo_stack();
                    treat_new_node(new_node, true);                
                } else {
                    undo_kill_face(new_node);
                    bad_face = get_original_bad_face(n1, n2, n3);
                    BOOST_ASSERT(bad_face != Face_handle());
                    finish_dealing_with_bad_face(bad_face, E);
                }
            } else if (loc == ON_EDGE) {
                bool build_123 = not edge_to_kill->he1()->is_boundary();
                bool build_142 = not edge_to_kill->he2()->is_boundary();
                Node_handle n1_ = edge_to_kill->he1()->origin();
                Node_handle n2_ = edge_to_kill->he2()->origin();
                Node_handle new_node = try_kill_edge(edge_to_kill, center, E);
                if (E.empty()) {
                    clear_undo_stack();
                    treat_new_node(new_node, true);   
                } else {
                    undo_kill_edge(new_node, n1_, n2_, build_123, build_142);
                    bad_face = get_original_bad_face(n1, n2, n3);
                    BOOST_ASSERT(bad_face != Face_handle());
                    finish_dealing_with_bad_face(bad_face, E);
                }
            } else { // loc == OUTSIDE_MESH:
                Edge_handle e = edge_to_kill;
                BOOST_ASSERT(e->is_boundary());
                if (e->he1()->is_boundary()) {
                    enc_hedges_.insert(e->he1());
                } else {
                    enc_hedges_.insert(e->he2());
                }
                split_encroached_boundary_edges(true);
            }
        }
    }

private:
    void collect_encroached_boundary_edges () {
        Halfedge_handle bhe_start = mesh_->boundary_halfedge();
        BOOST_ASSERT(bhe_start != Halfedge_handle());
        Halfedge_handle bhe_iter = bhe_start;
        do {
            Halfedge_handle he = bhe_iter->pair();
            BOOST_ASSERT(he->face() != Face_handle());
            if ( edge_is_encroached_upon_by_point( he->edge(), he->prev()->origin()->position() ) ) {
                enc_hedges_.insert(he);
            }
            bhe_iter = bhe_iter->next();
        } while (bhe_iter != bhe_start);
    }

    void split_encroached_boundary_edges (bool check_quality) {
        while (not enc_hedges_.empty()) {        
            Halfedge_handle he = *enc_hedges_.begin();
            enc_hedges_.erase(enc_hedges_.begin());

            Halfedge_handle hen = he->next();
            Halfedge_handle hep = he->prev();

            Point2 porig = he->origin()->position();
            Point2 pdest = he->pair()->origin()->position();

            double split;
            bool acutedest = hen->pair()->is_boundary();
            bool acuteorig = hep->pair()->is_boundary();
            if (acutedest != acuteorig) {
                double l = Kernel::distance(porig, pdest);
                double nearestpoweroftwo = 1.0;
                while (l > 3.0*nearestpoweroftwo) {
                    nearestpoweroftwo *= 2.0;
                }
                while (l < 1.5 * nearestpoweroftwo) {
                    nearestpoweroftwo *= 0.5;
                }
                split = nearestpoweroftwo / l;
                if (acutedest) {
                    split = 1.0 - split;
                }
            } else {
                split = 0.5;
            }

            Point2 split_point(porig.x()+split*(pdest.x()-porig.x()), porig.y()+split*(pdest.y()-porig.y()));
            Node_handle new_node;
            if (check_quality) {
                new_node = split_edge(he->edge(), split_point);
            } else {
                new_node = mesh_->split_edge(he->edge(), split_point);
            }
            Halfedge_handle he1 = hen->prev();
            Halfedge_handle he2 = hep->next();

            recursive_flip_delaunay(hen, check_quality, false);
            recursive_flip_delaunay(hep, check_quality, false);

            treat_new_node(new_node, check_quality);

            if ( edge_is_encroached_upon_by_point( he1->edge(), he1->prev()->origin()->position() ) ) {
                enc_hedges_.insert(he1);
            }
            if ( edge_is_encroached_upon_by_point( he2->edge(), he2->prev()->origin()->position() ) ) {
                enc_hedges_.insert(he2);
            }
        }
    }

    Node_handle split_edge(Edge_handle e, Point2 const& p) {
        dequeue_bad_face(e->he1()->face());
        dequeue_bad_face(e->he2()->face());
        Node_handle new_node = mesh_->split_edge(e, p);
        Halfedge_handle he_start = new_node->halfedge();
        Halfedge_handle he_iter = he_start;
        do {
            enqueue_bad_face(he_iter->face());
            he_iter = he_iter->pair()->next();
        } while (he_iter != he_start);
        return new_node;
    }

    void recursive_flip_delaunay (Halfedge_handle he, bool check_quality, bool save_to_undo_stack) {
        if (not he->edge()->is_diagonal_of_convex_quadrilateral() || he->edge()->is_constrained_delaunay()) {
            return;
        }

        Halfedge_handle he1 = he->pair()->next();
        Halfedge_handle he2 = he->pair()->prev();

        if (check_quality) {
            flip_edge(he->edge());
        } else {
            he->edge()->flip();
        }

        if (save_to_undo_stack) {
            undo_stack_.push(he->edge());
        }

        this->recursive_flip_delaunay(he1, check_quality, save_to_undo_stack);
        this->recursive_flip_delaunay(he2, check_quality, save_to_undo_stack);
    }

    void flip_edge (Edge_handle e) {
        this->dequeue_bad_face(e->he1()->face());
        this->dequeue_bad_face(e->he2()->face());
        e->flip();
        this->enqueue_bad_face(e->he1()->face());
        this->enqueue_bad_face(e->he2()->face());  
    }

    bool edge_is_encroached_upon_by_point( Edge_handle e, Point2 const& p ) const
    {
      Point2 p1, p2;
      e->vertices( p1, p2 );
      return ( p1 - p ).dot( p2 - p ) < 0.0;
    }

    void treat_new_node (Node_handle n, bool check_quality) {
        Halfedge_handle he_start = n->halfedge();
        Halfedge_handle he_iter = he_start;
        do {
            Face_handle f = he_iter->face();
            if (f != Face_handle()) {
                Edge_handle e = he_iter->next()->edge();
                if (e->is_boundary() && edge_is_encroached_upon_by_point( e, n->position() )) {
                    enc_hedges_.insert(he_iter->next());
                } else if (check_quality) {
                    enqueue_bad_face(f);
                }
            }
            he_iter = he_iter->pair()->next();
        } while (he_iter != he_start);    
    }

    Node_handle try_kill_edge (Edge_handle edge_to_kill, Point2 const& center, std::stack<Halfedge_handle>& E) {
        Node_handle new_node = split_edge(edge_to_kill, center);
        Halfedge_handle he_start = new_node->halfedge();
        Halfedge_handle he_iter  = he_start;
        do {
            Halfedge_handle he_next = he_iter->pair()->next();
            if (not he_iter->is_boundary()) {
                recursive_flip_delaunay(he_iter->next(), true, true);
            }
            he_iter = he_next;
        } while (he_iter != he_start);
        collect_encroached_boundary_edges(new_node, E);
        return new_node;
    }

    Node_handle try_kill_face(Face_handle face_to_kill, Point2 const& center, std::stack<Halfedge_handle>& E) {
        Node_handle new_node = split_face(face_to_kill, center);
        Halfedge_handle  he1 = new_node->halfedge()->next();
        Halfedge_handle  he2 = he1->next()->pair()->next();
        Halfedge_handle  he3 = he2->next()->pair()->next();
        recursive_flip_delaunay(he1, true, true);
        recursive_flip_delaunay(he2, true, true);
        recursive_flip_delaunay(he3, true, true);
        collect_encroached_boundary_edges(new_node, E);
        return new_node;
    }

    void undo_kill_face (Node_handle new_node) {
        undo_swapping();
        Halfedge_handle he1 = new_node->halfedge()->next();
        Halfedge_handle he2 = he1->next()->pair()->next();
        Halfedge_handle he3 = he2->next()->pair()->next();
        dequeue_bad_face(he1->face());
        dequeue_bad_face(he2->face());
        dequeue_bad_face(he3->face());
        mesh_->remove_node(new_node);
        Face_handle new_face = mesh_->add_face(he1, he2, he3);
        enqueue_bad_face(new_face);
    }

    void undo_kill_edge (Node_handle new_node, Node_handle n1, Node_handle n2, bool build_123, bool build_142) {
        undo_swapping();

        Halfedge_handle he1, he2;
        Halfedge_handle he_start = new_node->halfedge();
        Halfedge_handle he_iter = he_start;
        do {
            if (he_iter->pair()->origin() == n1) {
                he1 = he_iter;
            }
            if (he_iter->pair()->origin() == n2) {
                he2 = he_iter;
            }
            he_iter = he_iter->pair()->next();
        } while (he_iter != he_start);

        Halfedge_handle he23, he31, he14, he42;
        if (build_123) {
            he23 = he2->next();
            he31 = he1->pair()->prev();
            dequeue_bad_face(he23->face());
            dequeue_bad_face(he31->face());
        }
        if (build_142) {
            he14 = he1->next();
            he42 = he2->pair()->prev();
            dequeue_bad_face(he14->face());
            dequeue_bad_face(he42->face());
        }
        mesh_->remove_node(new_node);
        Halfedge_handle new_he = mesh_->add_edge(n1, n2);
        if (build_123) {
            enqueue_bad_face(mesh_->add_face(new_he, he23, he31));
        }
        if (build_142) {
            enqueue_bad_face(mesh_->add_face(new_he->pair(), he14, he42));
        }
    }

    void undo_swapping() {
        while (not undo_stack_.empty()) {
            flip_edge(undo_stack_.top());
            undo_stack_.pop();
        }
    }

    Node_handle split_face(Face_handle f, Point2 const& p) {
        dequeue_bad_face(f);
        Node_handle new_node = mesh_->split_face(f, p);
        enqueue_bad_face(new_node->halfedge()->face());
        enqueue_bad_face(new_node->halfedge()->pair()->face());
        enqueue_bad_face(new_node->halfedge()->prev()->pair()->face());
        return new_node;
    }

    void finish_dealing_with_bad_face(Face_handle bad_face, std::stack<Halfedge_handle> &E) {
        while (not E.empty()) {
            Halfedge_handle he = E.top();
            E.pop();
            Quality q(bad_face);
            Point2 p1, p2, p3;
            bad_face->vertices(p1, p2, p3);
            double l1, l2, l3;
            l1 = Kernel::distance(p1, p2);
            l2 = Kernel::distance(p2, p3);
            l3 = Kernel::distance(p3, p1);
            double d = std::min(l1, std::min(l2, l3));
            if (q.area() > max_area_ || split_permitted(he, d)) {
                enc_hedges_.insert(he);
            }
        }
        if (not enc_hedges_.empty()) {
            split_encroached_boundary_edges(true);
        } else {
            dequeue_bad_face(bad_face);
        }
    }

    bool split_permitted (Halfedge_handle he, double d)
    {
        bool prev_b = he->prev()->edge()->is_boundary();
        bool next_b = he->next()->edge()->is_boundary();
        if (prev_b == next_b) {
            return true;
        }

        Node_handle n;
        Halfedge_handle other_he;
        Point2 p1, p2, p3;
        if (prev_b) {
            other_he = he->prev();
            n = he->origin();
            p1 = other_he->origin()->position();
            p2 = he->origin()->position();
            p3 = he->pair()->origin()->position();
        } else {
            other_he = he->next();
            n = he->pair()->origin();
            p1 = he->origin()->position();
            p2 = other_he->origin()->position();
            p3 = other_he->pair()->origin()->position();
        }

        double l = he->edge()->length();
        double other_l = other_he->edge()->length();
        if (std::abs(l-other_l) > 0.00001) {
            return true;
        }

        double ll = Kernel::distance(p1, p3);
        double phi_min = std::acos((l*l + other_l*other_l - ll*ll)/(2.0*l*other_l));
        double r_min = l*std::sin(phi_min/2.0);
        if (r_min > d) {
            return true;
        }
        return false;
    }

    void collect_encroached_boundary_edges(Node_handle n, std::stack<Halfedge_handle>& E) {
        Halfedge_handle he_start = n->halfedge();
        Halfedge_handle he_iter = he_start;
        do {
            if (he_iter->face() != Face_handle()) {
                Edge_handle e = he_iter->next()->edge();
                if (e->is_boundary() && edge_is_encroached_upon_by_point( e, n->position() )) {
                    E.push(he_iter->next());
                }
            }
            he_iter = he_iter->pair()->next();
        } while (he_iter != he_start);
    }

    void enqueue_bad_face (Face_handle f) {
        if (f != Face_handle()) {
            Quality q(f);
            unsigned bhe = 0;
            if (f->halfedge()->pair()->is_boundary()) ++bhe;
            if (f->halfedge()->next()->pair()->is_boundary()) ++bhe;
            if (f->halfedge()->prev()->pair()->is_boundary()) ++bhe;
            bool restricted = bhe > 1;
            if (q.area() > max_area_ || (q.min_angle() < min_angle_ && not restricted)) {
                bad_faces_.insert(q);
            }
        }
    }

    void dequeue_bad_face (Face_handle f)
    {
        if (f != Face_handle()) {
            Quality q(f);
            unsigned bhe = 0;
            if (f->halfedge()->pair()->is_boundary()) ++bhe;
            if (f->halfedge()->next()->pair()->is_boundary()) ++bhe;
            if (f->halfedge()->prev()->pair()->is_boundary()) ++bhe;
            bool restricted = bhe > 1;
            if (q.area() > max_area_ || (q.min_angle() < min_angle_ && not restricted)) {
                bad_faces_.erase(q);
            }
        }
    }

    void clear_undo_stack() {
       while (not undo_stack_.empty()) {
            undo_stack_.pop();
        }
    }

    Face_handle get_bad_face () {
        BOOST_ASSERT(not bad_faces_.empty());
    }

    Face_handle get_original_bad_face(Node_handle n1, Node_handle n2, Node_handle n3) const {
        typename Bad_faces::iterator iter = bad_faces_.begin();
        Node_handle fn1, fn2, fn3;
        for (; iter != bad_faces_.end(); ++iter) {
            iter->face()->nodes(fn1, fn2, fn3);
            if (fn1 == n1 && fn2 == n2 && fn3 == n3 ) return iter->face();
            if (fn1 == n2 && fn2 == n3 && fn3 == n1 ) return iter->face();
            if (fn1 == n3 && fn2 == n1 && fn3 == n2 ) return iter->face();
        }
        return Face_handle();
    }

    Delaunay_triangulation* mesh_;
    double                  max_area_, min_angle_;
    Encroached_halfedges    enc_hedges_;
    Bad_faces               bad_faces_;
    Undo_stack              undo_stack_;
};

} // namespace umeshu

#endif /* __DELAUNAY_MESHER_H_INCLUDED__ */
