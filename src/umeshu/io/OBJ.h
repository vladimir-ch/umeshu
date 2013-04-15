#ifndef UMESHU_IO_OBJ_H
#define UMESHU_IO_OBJ_H

#include "../Point2.h"

#include <boost/utility/enable_if.hpp>

#include <fstream>
#include <string>

namespace umeshu {
namespace io {

// enabled only for triangulations with ids
template< typename Tria >
void write_obj( std::string const& filename, Tria& tria,
    typename boost::enable_if< typename Tria::Items::Supports_id >::type* dummy = 0 )
{
  namespace bg = boost::geometry;

  std::ofstream out( filename.c_str() );

  if ( out.fail() )
  {
    return;
  }

  Point2 p1, p2, p3;

  out << "# " << filename << std::endl;

  tria.generate_item_ids();

  for ( typename Tria::Node_const_iterator iter = tria.nodes_begin(); iter != tria.nodes_end(); ++iter )
  {
    out << "v " << bg::get<0>( iter->position() ) << " " << bg::get<1>( iter->position() ) << " 0\n";
  }
  
  for ( typename Tria::Face_const_iterator iter = tria.faces_begin(); iter != tria.faces_end(); ++iter )
  {
    typename Tria::Node_handle n1, n2, n3;
    iter->nodes( n1, n2, n3 );
    out << "f " << n1->id() + 1 << " " << n2->id() + 1 << " " << n3->id() + 1 << std::endl;
  }
}

} // io
} // umeshu

#endif // UMESHU_IO_OBJ_H
