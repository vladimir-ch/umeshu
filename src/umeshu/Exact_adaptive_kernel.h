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

#ifndef __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__
#define __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__

#include "Point2.h"

#include <boost/assert.hpp>
#include <boost/math/constants/constants.hpp>

#include <cmath>

namespace umeshu {

class Exact_adaptive_kernel {
    public:
        typedef Point2 Point_2;

        typedef enum {ON_POSITIVE_SIDE, ON_NEGATIVE_SIDE, ON_ORIENTED_BOUNDARY} Oriented_side;

        static Oriented_side oriented_side   (Point_2 const& pa, Point_2 const& pb, Point_2 const& test);
        static Oriented_side oriented_circle (Point_2 const& pa, Point_2 const& pb, Point_2 const& pc, Point_2 const& test);
        static Point_2 circumcenter (Point_2 const& p1, Point_2 const& p2, Point_2 const& p3);
        static Point_2 offcenter    (Point_2 const& p1, Point_2 const& p2, Point_2 const& p3, double offconstant);
        static double  signed_area  (Point_2 const& pa, Point_2 const& pb, Point_2 const& pc);

        static double distance_squared (Point_2 const& p1, Point_2 const& p2) {
            double x = p1.x() - p2.x();
            double y = p1.y() - p2.y();
            return x*x + y*y;
        }

        static double distance (Point_2 const& p1, Point_2 const& p2) {
            return std::sqrt(distance_squared(p1, p2));
        }

        static Point_2 midpoint (Point_2 const& p1, Point_2 const& p2) {
            return Point_2(0.5*(p1.x()+p2.x()), 0.5*(p1.y()+p2.y()));
        }

        static double circumradius (Point_2 const& p1, Point_2 const& p2, Point_2 const& p3) {
            double a = distance(p1, p2);
            double b = distance(p2, p3);
            double c = distance(p3, p1);
            double s = 0.5*(a+b+c);
            return 0.25*a*b*c/(std::sqrt(s*(s-a)*(s-b)*(s-c)));
        }

        static void triangle_angles (Point_2 const& p1, Point_2 const& p2, Point_2 const& p3, double& a1, double& a2, double& a3) {
            double a, b, c;
            a = distance(p2, p3);
            b = distance(p1, p3);
            c = distance(p1, p2);
            double aa = a*a;
            double bb = b*b;
            double cc = c*c;

            a1 = std::acos((bb+cc-aa)/(2.0*b*c));
            a2 = std::acos((aa+cc-bb)/(2.0*a*c));
            a3 = std::acos((aa+bb-cc)/(2.0*a*b));
        }

        static Point_2 barycenter (Point_2 const& p1, Point_2 const& p2, Point_2 const& p3) {
            const double one_third = boost::math::constants::third<double>();
            return Point2(one_third*(p1.x()+p2.x()+p3.x()),one_third*(p1.y()+p2.y()+p3.y()));
        }
};

} // namespace umeshu

#endif /* __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__ */
