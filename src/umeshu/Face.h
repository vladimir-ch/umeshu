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

#ifndef __FACE_H_INCLUDED__
#define __FACE_H_INCLUDED__

#include "Identifiable.h"
#include "Point2.h"

#include <boost/assert.hpp>

namespace umeshu {

template <typename Mesh>
class Face : public Identifiable {
public:
    typedef typename Mesh::Kernel_type           Kernel_type;
    typedef typename Mesh::Node_handle           Node_handle;
    typedef typename Mesh::Halfedge_handle       Halfedge_handle;
    typedef typename Mesh::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Mesh::Edge_handle           Edge_handle;
    typedef typename Mesh::Face_handle           Face_handle;

    explicit Face(Halfedge_handle adj_edge)
        : adj_edge_(adj_edge)
        , area_(0.0)
    { this->compute_area(); }

    Halfedge_handle       halfedge ()       { return adj_edge_; }
    Halfedge_const_handle halfedge () const { return adj_edge_; }
    
    void edge_lengths         (double& el1, double& el2, double& el3) const;
    void edge_lengths_squared (double& el1, double& el2, double& el3) const;

    Halfedge_handle longest_edge  (double *l = NULL) const;
    Halfedge_handle shortest_edge (double *l = NULL) const;
    double longest_edge_length  () const;
    double shortest_edge_length () const;
    double longest_edge_length_squared  () const;
    double shortest_edge_length_squared () const;
    
    void angles (double& a1, double& a2, double& a3) const;
    double minimum_angle () const;
    double maximum_angle () const;

    double circumradius () const;
    Point2 barycenter   () const;
    Point2 circumcenter () const;
    Point2 offcenter    (double offconstant) const;
    double area         () const { return area_; }
    double compute_area ();
    double quality      () const;
    
    bool is_restricted() const;

    void nodes(Node_handle& n1, Node_handle& n2, Node_handle& n3) const;
    void vertices(Point2& v1, Point2& v2, Point2& v3) const;

    // friend bool operator== (Face const& f1, Face const& f2);
    // friend bool operator!= (Face const& f1, Face const& f2);

private:
    Halfedge_handle adj_edge_;
    double area_;
};

// inline bool operator== (Face const& f1, Face const& f2)
// {
    // Node_handle f1n1, f1n2, f1n3;
    // Node_handle f2n1, f2n2, f2n3;
    // f1.nodes(f1n1, f1n2, f1n3);
    // f2.nodes(f2n1, f2n2, f2n3);
    // if (f1n1 == f2n1 && f1n2 == f2n2 && f1n3 == f2n3 ) return true;
    // if (f1n1 == f2n2 && f1n2 == f2n3 && f1n3 == f2n1 ) return true;
    // if (f1n1 == f2n3 && f1n2 == f2n1 && f1n3 == f2n2 ) return true;
    // return false;
// }

// inline bool operator!= (Face const& f1, Face const& f2)
// {
    // return !(f1 == f2);
// }

template <typename Mesh>
struct compare_faces {
    bool operator() (typename Mesh::Face_handle f1, typename Mesh::Face_handle f2) const {
        double s1 = f1->shortest_edge_length();
        s1 *= s1;
        double s2 = f2->shortest_edge_length();
        s2 *= s2;

        if (s1 == s2)
            return f1 < f2;
        else
            return s1 < s2;
    }
};

template <typename Mesh>
double Face<Mesh>::compute_area()
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    area_ = Kernel_type::signed_area(p1, p2, p3);
    BOOST_ASSERT(area_ > 0.0);
    return area_;
}

template <typename Mesh>
double Face<Mesh>::quality() const
{
    // TODO
    return 1.0;
}

template <typename Mesh>
Point2 Face<Mesh>::circumcenter() const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel_type::circumcenter(p1, p2, p3);    
}

template <typename Mesh>
Point2 Face<Mesh>::offcenter(double offconstant) const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel_type::offcenter(p1, p2, p3, offconstant);
}

template <typename Mesh>
double Face<Mesh>::circumradius() const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel_type::circumradius(p1, p2, p3);
}

template <typename Mesh>
void Face<Mesh>::edge_lengths(double& el1, double& el2, double& el3) const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    el1 = umeshu::distance(p1, p2);
    el2 = umeshu::distance(p2, p3);
    el3 = umeshu::distance(p3, p1);
}

template <typename Mesh>
void Face<Mesh>::edge_lengths_squared(double& el1, double& el2, double& el3) const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    el1 = umeshu::distance_squared(p1, p2);
    el2 = umeshu::distance_squared(p2, p3);
    el3 = umeshu::distance_squared(p3, p1);
}

template <typename Mesh>
typename Mesh::Halfedge_handle Face<Mesh>::longest_edge(double *l) const
{
    Halfedge_handle he1 = adj_edge_;
    Halfedge_handle he2 = he1->next();
    Halfedge_handle he3 = he2->next();
    BOOST_ASSERT(he3->next() == he1);
    
    double l1 = he1->edge()->length();
    double l2 = he2->edge()->length();
    double l3 = he3->edge()->length();
    if (l1 > l2 && l1 > l3) {
        *l = l1;
        return he1;
    } else if (l2 > l3) {
        *l = l2;
        return he2;
    } else {
        *l = l3;
        return he3;
    }
}

template <typename Mesh>
double Face<Mesh>::longest_edge_length() const
{
    double l1, l2, l3;
    this->edge_lengths(l1, l2, l3);
    return std::max(l1,std::max(l2,l3));
}

template <typename Mesh>
double Face<Mesh>::longest_edge_length_squared() const
{
    double l1, l2, l3;
    this->edge_lengths_squared(l1, l2, l3);
    return std::max(l1,std::max(l2,l3));
}

template <typename Mesh>
typename Mesh::Halfedge_handle Face<Mesh>::shortest_edge(double *l) const
{
    Halfedge_handle he1 = adj_edge_;
    Halfedge_handle he2 = he1->next();
    Halfedge_handle he3 = he2->next();
    BOOST_ASSERT(he3->next() == he1);

    double l1 = he1->edge()->length();
    double l2 = he2->edge()->length();
    double l3 = he3->edge()->length();
    if (l1 < l2 && l1 < l3) {
        *l = l1;
        return he1;
    } else if (l2 < l3) {
        *l = l2;
        return he2;
    } else {
        *l = l3;
        return he3;
    }
}

template <typename Mesh>
double Face<Mesh>::shortest_edge_length() const
{
    double l1, l2, l3;
    this->edge_lengths(l1, l2, l3);
    return std::min(l1, std::min(l2, l3));
}

template <typename Mesh>
double Face<Mesh>::shortest_edge_length_squared() const
{
    double l1, l2, l3;
    this->edge_lengths_squared(l1, l2, l3);
    return std::min(l1, std::min(l2, l3));
}

template <typename Mesh>
void Face<Mesh>::angles(double& a1, double& a2, double& a3) const
{    
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    umeshu::triangle_angles(p1, p2, p3, a1, a2, a3);
}

template <typename Mesh>
double Face<Mesh>::minimum_angle() const
{
    double a1, a2, a3;
    this->angles(a1, a2, a3);
    return std::min(a1,std::min(a2,a3));
}

template <typename Mesh>
double Face<Mesh>::maximum_angle() const
{
    double a1, a2, a3;
    this->angles(a1, a2, a3);
    return std::max(a1,std::max(a2,a3));
}

template <typename Mesh>
Point2 Face<Mesh>::barycenter() const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return umeshu::barycenter(p1, p2, p3);
}

template <typename Mesh>
bool Face<Mesh>::is_restricted() const
{
    int bedges = 0;
    Halfedge_handle he = this->adjacent_he();
    if (he->pair()->is_boundary()) {
        ++bedges;
    }
    he = he->next();
    if (he->pair()->is_boundary()) {
        ++bedges;
    }
    he = he->next();
    if (he->pair()->is_boundary()) {
        ++bedges;
    }
    return bedges >= 2;
}

template <typename Mesh>
void Face<Mesh>::nodes(Node_handle& n1, Node_handle& n2, Node_handle& n3) const
{
    n1 = adj_edge_->origin();
    n2 = adj_edge_->next()->origin();
    n3 = adj_edge_->prev()->origin();
}

template <typename Mesh>
void Face<Mesh>::vertices(Point2& v1, Point2& v2, Point2& v3) const
{
    Node_handle n1, n2, n3;
    this->nodes(n1, n2, n3);
    v1 = n1->position();
    v2 = n2->position();
    v3 = n3->position();
}

} // namespace umeshu

#endif // __FACE_H_INCLUDED__
