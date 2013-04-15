#ifndef UMESHU_IO_OFF_H
#define UMESHU_IO_OFF_H

#include "../Point2.h"

#include <boost/utility/enable_if.hpp>

#include <fstream>
#include <string>

namespace umeshu {
namespace io {

// enabled only for triangulations with ids
template< typename Tria >
void write_off( std::string const& filename, Tria& tria,
    typename boost::enable_if< typename Tria::Items::Supports_id >::type* dummy = 0 )
{
  namespace bg = boost::geometry;

  std::ofstream out( filename.c_str() );

  if ( out.fail() )
  {
    return;
  }

  Point2 p1, p2, p3;

  out << "OFF\n";
  out << "# " << filename << std::endl;
  out << tria.number_of_nodes() << " " << tria.number_of_faces() << " " << tria.number_of_edges() << std::endl;

  tria.generate_item_ids();

  for ( typename Tria::Node_const_iterator iter = tria.nodes_begin(); iter != tria.nodes_end(); ++iter )
  {
    out << bg::get<0>( iter->position() ) << " " << bg::get<1>( iter->position() ) << " 0\n";
  }
  
  for ( typename Tria::Face_const_iterator iter = tria.faces_begin(); iter != tria.faces_end(); ++iter )
  {
    typename Tria::Node_handle n1, n2, n3;
    iter->nodes( n1, n2, n3 );
    out << "3 " << n1->id() << " " << n2->id() << " " << n3->id() << std::endl;
  }
}

} // io
} // umeshu

#endif // UMESHU_IO_OFF_H
