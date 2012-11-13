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

// #include "BoundarySegment.h"
// #include "Smoother.h"

#include <umeshu/Bounding_box.h>
#include <umeshu/Delaunay_mesher.h>
#include <umeshu/Delaunay_triangulation.h>
#include <umeshu/Delaunay_triangulation_items.h>
#include <umeshu/Exceptions.h>
#include <umeshu/Polygon.h>
#include <umeshu/Relaxer.h>
#include <umeshu/Triangulator.h>
#include <umeshu/io/Postscript_ostream.h>

using namespace umeshu;

typedef Delaunay_triangulation<Delaunay_triangulation_items> Mesh;
typedef Mesh::Node_handle     Node_handle;
typedef Mesh::Halfedge_handle Halfedge_handle;
typedef Mesh::Edge_handle     Edge_handle;
typedef Mesh::Face_handle     Face_handle;
typedef Delaunay_mesher<Mesh> Mesher;
typedef Relaxer<Mesh>         Relax;
// typedef Smoother<mesh> smoother;

int main (int argc, const char * argv[])
{
    try {
        Mesh mesh;
        Triangulator<Mesh> triangulator;
        // Polygon boundary = Polygon::letter_u();
        // Polygon boundary = Polygon::crack();
        Polygon boundary = Polygon::kidney();
        // Polygon boundary = Polygon::letter_a();
        // Polygon boundary = Polygon::square(1.0);
        // Polygon boundary = Polygon::island();
        // Polygon boundary = Polygon::triangle();
        
        triangulator.triangulate(boundary, mesh);
        io::Postscript_ostream ps1("mesh_1.eps", mesh.bounding_box());
        ps1 << mesh;

        mesh.make_cdt();
        io::Postscript_ostream ps2("mesh_2.eps", mesh.bounding_box());
        ps2 << mesh;

        Mesher mesher;
        mesher.refine(mesh, 0.001, 21.0);
        io::Postscript_ostream ps3("mesh_3.eps", mesh.bounding_box());
        ps3 << mesh;

        Relax relax;
        relax.relax(mesh);
        io::Postscript_ostream ps4("mesh_4.eps", mesh.bounding_box());
        ps4 << mesh;

        // smoother smooth;
        // smooth.smooth(m, 1);
        // Postscript_stream ps5("mesh_5.eps", m.bounding_box());
        // ps5 << m;

        // // code does not pass a debug assert:
        // meshgen.refine(0.0001000, 25);
        // Postscript_stream ps6("mesh_6.eps", m.bounding_box());
        // ps6 << m;

        // // smooth.smooth(m, 5);
        // // Postscript_stream ps7("mesh_7.eps", m.bounding_box());
        // // ps7 << m;

        std::cout << "Number of nodes: " << mesh.number_of_nodes() << std::endl;
        std::cout << "Number of edges: " << mesh.number_of_edges() << std::endl;
        std::cout << "Number of faces: " << mesh.number_of_faces() << std::endl;
    }
    catch (boost::exception & e) {
        std::cerr << boost::diagnostic_information(e);
    }

    return 0;
}
