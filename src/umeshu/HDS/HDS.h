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

#ifndef __HDS_H_INCLUDED__
#define __HDS_H_INCLUDED__ 

#include <boost/noncopyable.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <list>

namespace umeshu {
namespace hds {

template <typename Items, typename Kernel, typename Alloc = boost::fast_pool_allocator<int> >
class HDS : public boost::noncopyable {
public:
    typedef HDS<Items, Kernel, Alloc> Self;

    typedef typename Items::template Node_wrapper<Kernel, Self>     Node_wrapper;
    typedef typename Items::template Halfedge_wrapper<Kernel, Self> Halfedge_wrapper;
    typedef typename Items::template Edge_wrapper<Kernel, Self>     Edge_wrapper;
    typedef typename Items::template Face_wrapper<Kernel, Self>     Face_wrapper;

    typedef typename Node_wrapper::Node         Node;
    typedef typename Halfedge_wrapper::Halfedge Halfedge;
    typedef typename Edge_wrapper::Edge         Edge;
    typedef typename Face_wrapper::Face         Face;

    typedef typename Alloc::template rebind<Node>::other     Node_allocator;
    typedef typename Alloc::template rebind<Halfedge>::other Halfedge_allocator;
    typedef typename Alloc::template rebind<Edge>::other     Edge_allocator;
    typedef typename Alloc::template rebind<Face>::other     Face_allocator;

    typedef std::list<Node, Node_allocator>         Node_list;
    typedef std::list<Edge, Edge_allocator>         Edge_list;
    typedef std::list<Halfedge, Halfedge_allocator> Halfedge_list;
    typedef std::list<Face, Face_allocator>         Face_list;

    typedef typename Node_list::iterator           Node_iterator;
    typedef typename Halfedge_list::iterator       Halfedge_iterator;
    typedef typename Edge_list::iterator           Edge_iterator;
    typedef typename Face_list::iterator           Face_iterator;
    typedef typename Node_list::const_iterator     Node_const_iterator;
    typedef typename Halfedge_list::const_iterator Halfedge_const_iterator;
    typedef typename Edge_list::const_iterator     Edge_const_iterator;
    typedef typename Face_list::const_iterator     Face_const_iterator;

    typedef Node_iterator           Node_handle;
    typedef Halfedge_iterator       Halfedge_handle;
    typedef Edge_iterator           Edge_handle;
    typedef Face_iterator           Face_handle;

    Node_iterator       nodes_begin()       { return nodes_.begin(); }
    Node_iterator       nodes_end()         { return nodes_.end(); }
    Edge_iterator       edges_begin()       { return edges_.begin(); }
    Edge_iterator       edges_end()         { return edges_.end(); }
    Face_iterator       faces_begin()       { return faces_.begin(); }
    Face_iterator       faces_end()         { return faces_.end(); }

    Node_const_iterator nodes_begin() const { return nodes_.begin(); }
    Node_const_iterator nodes_end()   const { return nodes_.end(); }
    Edge_const_iterator edges_begin() const { return edges_.begin(); }
    Edge_const_iterator edges_end()   const { return edges_.end(); }
    Face_const_iterator faces_begin() const { return faces_.begin(); }
    Face_const_iterator faces_end()   const { return faces_.end(); }

    size_t number_of_nodes () const { return nodes_.size(); }
    size_t number_of_halfedges () const { return halfedges_.size(); }
    size_t number_of_edges () const { return edges_.size(); }
    size_t number_of_faces () const { return faces_.size(); }

protected:
    Node_handle get_new_node () {
        return nodes_.insert(nodes_.end(), Node());
    }
    Edge_handle get_new_edge () {
        Halfedge_handle he1 = halfedges_.insert(halfedges_.end(), Halfedge());
        Halfedge_handle he2 = halfedges_.insert(halfedges_.end(), Halfedge());
        Edge_handle e = edges_.insert(edges_.end(), Edge(he1, he2));
        he1->set_edge(e);
        he2->set_edge(e);
        return e;
    }
    Face_handle get_new_face () {
        return faces_.insert(faces_.end(), Face());
    }

    void delete_node (Node_handle n) {
        nodes_.erase(n);
    }
    void delete_edge (Edge_handle e) {
        halfedges_.erase(e->he1());
        halfedges_.erase(e->he2());
        edges_.erase(e);
    }
    void delete_face (Face_handle f) {
        faces_.erase(f);
    }

private:
    Node_list     nodes_;
    Halfedge_list halfedges_;
    Edge_list     edges_;
    Face_list     faces_;
};

} // namespace hds
} // namespace umeshu

#endif /* __HDS_H_INCLUDED__ */
