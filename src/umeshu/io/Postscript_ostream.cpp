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

#include "Postscript_ostream.h"

#include <cmath>

namespace umeshu {
namespace io {

/* 1cm is this amount of PostScript points */
const float cm = 28.3464567;

float const Postscript_ostream::default_fig_size = 10 * cm;
float const Postscript_ostream::default_fig_margin = 0.1 * cm;
float const Postscript_ostream::default_line_width = 0.01 * cm;

Postscript_ostream::Postscript_ostream( std::string const& filename, Bounding_box const& bbox )
  : of_( filename.c_str() )
  , bbox_( bbox )
{
  using namespace boost::geometry;

  typedef typename coordinate_type<Bounding_box>::type ct;

  ct width  = get<max_corner, 0>( bbox ) - get<min_corner, 0>( bbox );
  ct height = get<max_corner, 1>( bbox ) - get<min_corner, 1>( bbox );

  ct min_size = std::min( width, height );

  fig_width_ = width / min_size * default_fig_size;
  fig_height_ = height / min_size * default_fig_size;

  x_scale = fig_width_ / width;
  y_scale = fig_height_ / height;

  x_trans = x_scale * get<min_corner, 0>( bbox );
  y_trans = y_scale * get<min_corner, 1>( bbox );

  of_ << "%!PS-Adobe-3.0 EPSF-3.0" << std::endl;
  of_ << "%%BoundingBox: 0 0 "
      << static_cast<int>( ceil( fig_width_ + 2 * default_fig_margin ) ) << " "
      << static_cast<int>( ceil( fig_height_ + 2 * default_fig_margin ) ) << std::endl;
  of_ << "%%HiResBoundingBox: 0.0 0.0 "
      << fig_width_ + 2 * default_fig_margin << " "
      << fig_height_ + 2 * default_fig_margin << std::endl;
  of_ << "%%Creator: umeshu++" << std::endl;
  of_ << "%%Title: Unstructured mesh " << filename << std::endl;
  of_ << "%%EndComments" << std::endl;

  of_ << default_fig_margin << " " << default_fig_margin << " translate" << std::endl;
  of_ << default_line_width << " setlinewidth" << std::endl;

  of_ << "/c {\n"
      << 3 * default_line_width << " 0 360 arc closepath\n"
      << "} def\n";
  of_ << "/m {moveto} def\n";
  of_ << "/l {lineto} def\n";
  of_ << "/f {fill} def\n";
  of_ << "/s {stroke} def\n";
  of_ << "/np {newpath} def\n";
  of_ << "/cp {closepath} def\n";
  of_ << "/sg {setgray} def\n";
  of_ << "/sc {setrgbcolor} def\n";
}

} // namespace io
} // namespace umeshu

