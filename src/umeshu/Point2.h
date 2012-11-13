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

#ifndef __POINT_H_INCLUDED__
#define __POINT_H_INCLUDED__

#include <boost/operators.hpp>

#include <ostream>
#include <limits>

namespace umeshu {

class Point2
    : boost::additive< Point2
    , boost::multiplicative< Point2, double
    , boost::equality_comparable< Point2
      > > >
{
public:
    Point2() { coord_[0] = 0.0; coord_[1] = 0.0; }
    Point2(double x, double y) { coord_[0] = x; coord_[1] = y; }
    
    double& x()       { return coord_[0]; }
    double  x() const { return coord_[0]; }
    double& y()       { return coord_[1]; }
    double  y() const { return coord_[1]; }
    
    double const* coord() const { return &coord_[0]; }

    Point2 operator+=(Point2 const& p) { x() += p.x(); y() += p.y(); return *this; }
    Point2 operator-=(Point2 const& p) { x() -= p.x(); y() -= p.y(); return *this; }
    Point2 operator*=(double a) { x() *= a; y() *= a; return *this; }
    Point2 operator/=(double a) { x() /= a; y() /= a; return *this; }

    friend bool          operator== (Point2 const& p1, Point2 const& p2);
    friend std::ostream& operator<< (std::ostream& os, Point2 const& p);

private:
    double coord_[2];
};

inline bool operator== (Point2 const& p1, Point2 const& p2)
{
    return p1.x() == p2.x() && p1.y() == p2.y();
}

inline std::ostream& operator<<(std::ostream& os, Point2 const& p)
{
    os.precision(std::numeric_limits<double>::digits10);
    os << "[" << std::fixed << p.x() << "," << p.y() << "]";
    return os;
}

} // namespace umeshu

#endif // __POINT_H_INCLUDED__
