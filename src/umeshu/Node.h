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

#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include "Identifiable.h"
#include "Point2.h"

#include <iostream>

namespace umeshu {

template <typename Mesh>
class Node : public Identifiable
{
public:
    typedef typename Mesh::Halfedge_handle       Halfedge_handle;
    typedef typename Mesh::Halfedge_const_handle Halfedge_const_handle;

             Node(double x, double y) : p_(x,y), out_he_() {}
    explicit Node(Point2 const& p)    : p_(p),   out_he_() {}

    double& x()       { return p_.x(); }
    double  x() const { return p_.x(); }
    double& y()       { return p_.y(); }
    double  y() const { return p_.y(); }

    Point2&       position()       { return p_; }
    Point2 const& position() const { return p_; }
    
    Halfedge_handle        halfedge()       { return out_he_; }
    Halfedge_const_handle  halfedge() const { return out_he_; }

    void set_halfedge(Halfedge_handle he) { out_he_ = he; }
    
    bool is_isolated() const { return out_he_ == Halfedge_handle(); }
    bool is_boundary() const;

    int degree() const;
    
    Halfedge_handle find_free_incident_halfedge ();
    Halfedge_handle find_free_incident_halfedge_in_range (Halfedge_handle he1, Halfedge_handle he2);

    friend std::ostream& operator<< (std::ostream& os, Node<Mesh> const& n) {
        os << n.position();
        return os;
    }
    
private:
    Point2 p_;
    Halfedge_handle out_he_;
};

template <typename Mesh>
inline double distance_squared(typename Mesh::Node_handle n1, typename Mesh::Node_handle n2)
{
    return distance_squared(n1->position(), n2->position());
}

template <typename Mesh>
inline double distance(typename Mesh::Node_handle n1, typename Mesh::Node_handle n2)
{
    return std::sqrt(distance_squared(n1, n2));
}

template <typename Mesh>
bool Node<Mesh>::is_boundary() const
{
    if (is_isolated()) {
        return false;
    }

    Halfedge_handle he_start = halfedge();
    Halfedge_handle he_iter  = he_start;
    do {
        if (he_iter->is_boundary()) {
            return true;
        }
        he_iter = he_iter->pair()->next();
    } while (he_iter != he_start);

    return false;
}

template <typename Mesh>
int Node<Mesh>::degree() const
{
    int d = 0;
    if (not is_isolated()) {
        Halfedge_const_handle he_start = halfedge();
        Halfedge_const_handle he_iter = he_start;
        do {
            ++d;
            he_iter = he_iter->pair()->next();
        } while (he_iter != he_start);
    }
    return d;
}

template <typename Mesh>
typename Mesh::Halfedge_handle Node<Mesh>::find_free_incident_halfedge ()
{
    return find_free_incident_halfedge_in_range(halfedge()->pair(), halfedge()->pair());
}

template <typename Mesh>
typename Mesh::Halfedge_handle Node<Mesh>::find_free_incident_halfedge_in_range (Halfedge_handle he1, Halfedge_handle he2)
{
    Halfedge_handle result = he1;
    while (not result->is_boundary()) {
        result = result->next()->pair();
        if (result == he2)
            break;
    }
    if (result->is_boundary()) {
        return result;
    } else {
        return Halfedge_handle();
    }
}

} // namespace umeshu

#endif // __NODE_H_INCLUDED__
