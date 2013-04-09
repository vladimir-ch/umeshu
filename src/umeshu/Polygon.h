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

#ifndef UMESHU_POLYGON_H
#define UMESHU_POLYGON_H

#include "Point2.h"
#include "Exceptions.h"

#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/read.hpp>

#include <fstream>

namespace umeshu
{

// counter-clockwise orientation
typedef boost::geometry::model::polygon< Point2, false > Polygon;

inline void read_polygon( std::string const& filename, Polygon& poly )
{
  std::ifstream file( filename.c_str() );

  if ( ! file.is_open() )
  {
    BOOST_THROW_EXCEPTION( umeshu_error() );
  }

  std::string line;
  std::getline( file, line );
  boost::trim( line );

  boost::geometry::read_wkt( line, poly );
}

} // namespace umeshu

#endif /* UMESHU_POLYGON_H */
