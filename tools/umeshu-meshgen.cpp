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

#include <umeshu/Delaunay_mesher.h>
#include <umeshu/Delaunay_triangulation.h>
#include <umeshu/Delaunay_triangulation_items.h>
#include <umeshu/Exceptions.h>
#include <umeshu/Polygon.h>
#include <umeshu/Relaxer.h>
#include <umeshu/Triangulator.h>
#include <umeshu/io/OBJ.h>
#include <umeshu/io/OFF.h>
#include <umeshu/io/EPS.h>
#include <umeshu/io/STL.h>

#include <boost/program_options.hpp>

using namespace umeshu;
namespace po = boost::program_options;

typedef Delaunay_triangulation< Delaunay_triangulation_items_with_id > Mesh;
typedef Mesh::Node_handle     Node_handle;
typedef Mesh::Halfedge_handle Halfedge_handle;
typedef Mesh::Edge_handle     Edge_handle;
typedef Mesh::Face_handle     Face_handle;
typedef Delaunay_mesher< Mesh > Mesher;
typedef Relaxer< Mesh >       Relax;

int main( int argc, const char* argv[] )
{
  double max_area;
  double min_angle;

  po::options_description po_desc( "Allowed options" );
  po_desc.add_options()
    ( "help", "produce help message" )
    ( "max-size,s", po::value<double>( &max_area )->default_value( 0.01 ), "set the maximum triangle area for the refinement algorithm" )
    ( "min-angle,a", po::value<double>( &min_angle )->default_value( 21 ), "set the minimum angle for the refinement algorithm" )
    ( "input-file", po::value<std::string>(), "input file describing the polygonal boundary (in Well-Known Text format)");

  po::positional_options_description po_pdesc;
  po_pdesc.add("input-file", -1);

  po::variables_map po_vm;
  po::store( po::command_line_parser( argc, argv ).options( po_desc ).positional( po_pdesc ).run(), po_vm );
  po::notify( po_vm );

  if ( po_vm.count( "help" ) )
  {
    std::cout << po_desc << std::endl;
    return EXIT_SUCCESS;
  }

  if ( ! po_vm.count( "input-file" ) )
  {
    std::cout << "Name of the input file not specified\n";
    std::cout << po_desc << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Parameters used:" << std::endl
    << "  maximum triangle area = " << max_area << std::endl
    << "  minimum angle = " << min_angle << std::endl;

  try
  {
    Polygon boundary;
    read_polygon( po_vm["input-file"].as< std::string >(), boundary );

    Mesh mesh;

    Triangulator<Mesh> triangulator;
    triangulator.triangulate( boundary, mesh );
    io::write_eps( "mesh_1.eps", mesh );

    mesh.make_cdt();
    io::write_eps( "mesh_2.eps", mesh );

    Mesher mesher;
    mesher.refine( mesh, max_area, min_angle );
    io::write_eps( "mesh_3.eps", mesh );
    io::write_stl( "mesh_3.stl", mesh );
    io::write_off( "mesh_3.off", mesh );
    io::write_obj( "mesh_3.obj", mesh );

    Relax relax;
    relax.relax( mesh );
    io::write_eps( "mesh_4.eps", mesh );

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

    std::cout << "Final mesh:" << std::endl
      << "  # nodes: " << mesh.number_of_nodes() << std::endl
      << "  # edges: " << mesh.number_of_edges() << std::endl
      << "  # faces: " << mesh.number_of_faces() << std::endl;
  }
  catch ( boost::exception& e )
  {
    std::cerr << boost::diagnostic_information( e );
  }

  return EXIT_SUCCESS;
}
