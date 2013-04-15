#ifndef UMESHU_IO_STL_H
#define UMESHU_IO_STL_H

#include "../Point2.h"

#include <fstream>
#include <string>

namespace umeshu {
namespace io {

template< typename Tria >
void write_stl( std::string const& filename, Tria const& tria )
{
  namespace bg = boost::geometry;

  std::ofstream out( filename.c_str() );

  if ( out.fail() )
  {
    return;
  }

  out << "solid " << filename << std::endl;
  
  Point2 p1, p2, p3;

  for ( typename Tria::Face_const_iterator iter = tria.faces_begin(); iter != tria.faces_end(); ++iter )
  {
    iter->vertices( p1, p2, p3 );
    out << "facet normal 0 0 1\n";
    out << " outer loop\n";
    out << "  vertex " << bg::get<0>( p1 ) << " " << bg::get<1>( p1 ) << " 0\n";
    out << "  vertex " << bg::get<0>( p2 ) << " " << bg::get<1>( p2 ) << " 0\n";
    out << "  vertex " << bg::get<0>( p3 ) << " " << bg::get<1>( p3 ) << " 0\n";
    out << " endloop";
    out << "endfacet\n";
  }

  out << "endsolid " << filename;
}

} // io
} // umeshu

#endif // UMESHU_IO_STL_H
