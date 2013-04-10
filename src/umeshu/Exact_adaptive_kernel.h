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

#ifndef __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__
#define __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__

#include "Point2.h"
#include "Orientation.h"

#include <boost/math/constants/constants.hpp>

#include <cmath>

namespace umeshu
{

struct Exact_adaptive_kernel
{
  static Oriented_side oriented_side( Point2 const& pa, Point2 const& pb, Point2 const& test );
  static Oriented_side oriented_circle( Point2 const& pa, Point2 const& pb, Point2 const& pc, Point2 const& test );

  static Point2 circumcenter( Point2 const& p1, Point2 const& p2, Point2 const& p3 );
  static Point2 offcenter( Point2 const& p1, Point2 const& p2, Point2 const& p3, double offconstant );
  static double signed_area( Point2 const& pa, Point2 const& pb, Point2 const& pc );

  static double distance_squared( Point2 const& p1, Point2 const& p2 )
  {
    return ( p2 - p1 ).squaredNorm();
  }

  static double distance( Point2 const& p1, Point2 const& p2 )
  {
    return ( p2 - p1 ).norm();
  }

  static Point2 midpoint( Point2 const& p1, Point2 const& p2 )
  {
    return 0.5 * ( p1 + p2 );
  }

  static double circumradius( Point2 const& p1, Point2 const& p2, Point2 const& p3 )
  {
    double a = distance( p1, p2 );
    double b = distance( p2, p3 );
    double c = distance( p3, p1 );
    double s = 0.5 * ( a + b + c );
    return 0.25 * a * b * c / ( std::sqrt( s * ( s - a ) * ( s - b ) * ( s - c ) ) );
  }

  static void triangle_angles( Point2 const& p1, Point2 const& p2, Point2 const& p3, double& a1, double& a2, double& a3 )
  {
    double a = distance( p2, p3 );
    double b = distance( p1, p3 );
    double c = distance( p1, p2 );

    double aa = a * a;
    double bb = b * b;
    double cc = c * c;

    a1 = std::acos( ( bb + cc - aa ) / ( 2.0 * b * c ) );
    a2 = std::acos( ( aa + cc - bb ) / ( 2.0 * a * c ) );
    a3 = std::acos( ( aa + bb - cc ) / ( 2.0 * a * b ) );
  }

  static Point2 barycenter( Point2 const& p1, Point2 const& p2, Point2 const& p3 )
  {
    const double one_third = boost::math::constants::third<double>();
    return one_third * ( p1 + p2 + p3 );
  }
};

} // namespace umeshu

#endif /* __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__ */
