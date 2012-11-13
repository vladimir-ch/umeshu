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

#ifndef __SMOOTHER_H_INCLUDED__
#define __SMOOTHER_H_INCLUDED__ 

namespace umeshu {

template<typename Mesh>
class Smoother
{
public:
    typedef typename Mesh::kernel_type kernel_type;

    void smooth(Mesh &mesh, int niter);

private:
    void smooth_once(Mesh &mesh);
};

template<typename Mesh>
void Smoother<Mesh>::smooth(Mesh &mesh, int niter)
{
    for (int i = 0; i < niter; ++i) {
        this->smooth_once(mesh);
    }
}

template<typename Mesh>
void Smoother<Mesh>::smooth_once(Mesh &mesh)
{
    typename Mesh::nodes_iterator nodes_iter = mesh.nodes_begin();
    for(; nodes_iter != mesh.nodes_end(); ++nodes_iter) {
        NodeHandle node = *nodes_iter;
        if (node->is_boundary()) {
            continue;
        }
        Point2 new_pos;
        int n = node->degree();
        HalfEdgeHandle he_start = node->out_he()->pair();
        HalfEdgeHandle he_iter = he_start;
        do {
            new_pos += he_iter->origin()->position()/n;
            he_iter = he_iter->next()->pair();
        } while (he_iter != he_start);
        node->position() = new_pos;
    }
}

} // namespace umeshu

#endif /* __SMOOTHER_H_INCLUDED__ */
