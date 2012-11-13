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

#include "Exact_adaptive_kernel.h"

double orient2d(double const* pa, double const* pb, double const* pc);
double incircle(double const* pa, double const* pb, double const* pc, double const* pd);

namespace umeshu {

Exact_adaptive_kernel::Oriented_side Exact_adaptive_kernel::oriented_side (Point2 const& pa, Point2 const& pb, Point2 const& test)
{
    double r = orient2d(pa.coord(), pb.coord(), test.coord());
    if (r > 0.0)
        return ON_POSITIVE_SIDE;
    else if (r < 0.0)
        return ON_NEGATIVE_SIDE;
    else
        return ON_ORIENTED_BOUNDARY;
}

Exact_adaptive_kernel::Oriented_side Exact_adaptive_kernel::oriented_circle (Point2 const& pa, Point2 const& pb, Point2 const& pc, Point2 const& test)
{
    double r = incircle(pa.coord(), pb.coord(), pc.coord(), test.coord());
    if (r > 0.0)
        return ON_POSITIVE_SIDE;
    else if (r < 0.0)
        return ON_NEGATIVE_SIDE;
    else
        return ON_ORIENTED_BOUNDARY;    
}

Point2 Exact_adaptive_kernel::circumcenter(Point2 const& p1, Point2 const& p2, Point2 const& p3)
{
    Point2 p2p1(p2.x()-p1.x(), p2.y()-p1.y());
    Point2 p3p1(p3.x()-p1.x(), p3.y()-p1.y());
    Point2 p2p3(p2.x()-p3.x(), p2.y()-p3.y());
    double p2p1dist = p2p1.x()*p2p1.x() + p2p1.y()*p2p1.y();
    double p3p1dist = p3p1.x()*p3p1.x() + p3p1.y()*p3p1.y();
    double denominator = 0.5/(2.0*signed_area(p1, p2, p3));
    BOOST_ASSERT(denominator > 0.0);
    double dx = (p3p1.y() * p2p1dist - p2p1.y() * p3p1dist) * denominator;
    double dy = (p2p1.x() * p3p1dist - p3p1.x() * p2p1dist) * denominator;
    return Point2(p1.x()+dx, p1.y()+dy);
}

Point2 Exact_adaptive_kernel::offcenter(Point2 const& p1, Point2 const& p2, Point2 const& p3, double offconstant)
{
    Point2 p2p1(p2.x()-p1.x(), p2.y()-p1.y());
    Point2 p3p1(p3.x()-p1.x(), p3.y()-p1.y());
    Point2 p2p3(p2.x()-p3.x(), p2.y()-p3.y());
    double p2p1dist = p2p1.x()*p2p1.x() + p2p1.y()*p2p1.y();
    double p3p1dist = p3p1.x()*p3p1.x() + p3p1.y()*p3p1.y();
    double p2p3dist = p2p3.x()*p2p3.x() + p2p3.y()*p2p3.y();
    double denominator = 0.5/(2.0*Exact_adaptive_kernel::signed_area(p1, p2, p3));
    BOOST_ASSERT(denominator > 0.0);
    double dx = (p3p1.y() * p2p1dist - p2p1.y() * p3p1dist) * denominator;
    double dy = (p2p1.x() * p3p1dist - p3p1.x() * p2p1dist) * denominator;
    double dxoff, dyoff;
    
    if ((p2p1dist < p3p1dist) && (p2p1dist < p2p3dist)) {
        dxoff = 0.5 * p2p1.x() - offconstant * p2p1.y();
        dyoff = 0.5 * p2p1.y() + offconstant * p2p1.x();
        if (dxoff * dxoff + dyoff * dyoff < dx * dx + dy * dy) {
            dx = dxoff;
            dy = dyoff;
        }
    } else if (p3p1dist < p2p3dist) {
        dxoff = 0.5 * p3p1.x() + offconstant * p3p1.y();
        dyoff = 0.5 * p3p1.y() - offconstant * p3p1.x();
        if (dxoff * dxoff + dyoff * dyoff < dx * dx + dy * dy) {
            dx = dxoff;
            dy = dyoff;
        }
    } else {
        dxoff = 0.5 * p2p3.x() - offconstant * p2p3.y();
        dyoff = 0.5 * p2p3.y() + offconstant * p2p3.x();
        if (dxoff * dxoff + dyoff * dyoff < (dx - p2p1.x()) * (dx - p2p1.x()) + (dy - p2p1.y()) * (dy - p2p1.y())) {
            dx = p2p1.x() + dxoff;
            dy = p2p1.y() + dyoff;
        }
    }
    
    return Point2(p1.x()+dx, p1.y()+dy);
}

double Exact_adaptive_kernel::signed_area (Point_2 const& pa, Point_2 const& pb, Point_2 const& pc)
{
    return 0.5*orient2d(pa.coord(), pb.coord(), pc.coord());
}

} // namespace umeshu
