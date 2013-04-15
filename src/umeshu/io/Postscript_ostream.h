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

#ifndef UMESHU_POSTSCRIPT_STREAM_H
#define UMESHU_POSTSCRIPT_STREAM_H

#include <umeshu/Bounding_box.h>

#include <fstream>
#include <string>

namespace umeshu {
namespace io {

class Postscript_ostream
{

public:

  Postscript_ostream( std::string const& filename, Bounding_box const& bb );

  Postscript_ostream& newpath()
  {
    of_ << "np\n";
    return *this;
  }

  Postscript_ostream& closepath()
  {
    of_ << "cp\n";
    return *this;
  }

  Postscript_ostream& moveto( double x, double y )
  {
    double nx, ny;
    transform( x, y, nx, ny );
    of_ << nx << " " << ny << " m\n";
    return *this;
  }

  Postscript_ostream& lineto( double x, double y )
  {
    double nx, ny;
    transform( x, y, nx, ny );
    of_ << nx << " " << ny << " l\n";
    return *this;
  }

  Postscript_ostream& stroke()
  {
    of_ << "s\n";
    return *this;
  }

  Postscript_ostream& fill()
  {
    of_ << "f\n";
    return *this;
  }

  Postscript_ostream& dot( double x, double y )
  {
    double nx, ny;
    transform( x, y, nx, ny );
    of_ << nx << " " << ny << " c\n";
    return *this;
  }

  Postscript_ostream& setrgbcolor( double r, double g, double b )
  {
    of_ << r << " " << g << " " << b << " sc\n";
    return *this;
  }

  Postscript_ostream& setgray( double g )
  {
    of_ << g << " sg\n";
    return *this;
  }

private:

  void transform( double x, double y, double& nx, double& ny )
  {
    nx = x_scale * x - x_trans;
    ny = y_scale * y - y_trans;
  }

  std::ofstream of_;
  Bounding_box bbox_;
  double fig_width_, fig_height_;
  double x_scale, y_scale;
  double x_trans, y_trans;

  static float const default_fig_size;
  static float const default_fig_margin;
  static float const default_line_width;

};

template <typename Tria>
void write_ps( io::Postscript_ostream& ps, Tria const& tria )
{
  Point2 p1, p2, p3;

  ps.setgray( 0.8 );

  for ( typename Tria::Face_const_iterator iter = tria.faces_begin(); iter != tria.faces_end(); ++iter )
  {
    iter->vertices( p1, p2, p3 );
    ps.newpath().moveto( p1.x(), p1.y() ).lineto( p2.x(), p2.y() ).lineto( p3.x(), p3.y() ).fill();
  }

  ps.setgray( 0 );
  ps.newpath();

  for ( typename Tria::Edge_const_iterator iter = tria.edges_begin(); iter != tria.edges_end(); ++iter )
  {
    iter->vertices( p1, p2 );
    ps.moveto( p1.x(), p1.y() ).lineto( p2.x(), p2.y() );
  }

  ps.stroke();

  for ( typename Tria::Node_const_iterator iter = tria.nodes_begin(); iter != tria.nodes_end(); ++iter )
  {
    if ( iter->is_boundary() )
    {
      ps.setrgbcolor( 1, 0, 0 );
    }
    else
    {
      ps.setrgbcolor( 1, 1, 0 );
    }

    ps.newpath().dot( iter->position().x(), iter->position().y() ).fill();
  }
}

} // namespace io
} // namespace umeshu

#endif // UMESHU_POSTSCRIPT_STREAM_H
