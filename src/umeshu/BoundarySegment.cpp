//
// Copyright (c) 2011-2012 Vladimir Chalupecky
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "BoundarySegment.h"
#include "FastKernel.h"

namespace umeshu {

Point2 LineBoundarySegment::midpoint( const Point2& p1, const Point2& p2 )
{
    return Point2(0.5*(p1.x()+p2.x()), 0.5*(p1.y()+p2.y()));
}

CircleBoundarySegment::CircleBoundarySegment(Point2 const& p1, Point2 const& p2, Point2 const& p3 )
: p1_(p1), p2_(p2), p3_(p3)
{
	center_ = FastKernel::circumcenter(p1_, p2_, p3_);
    radius_ = circumradius(p1_, p2_, p3_);
}

Point2 CircleBoundarySegment::midpoint(Point2 const& p1, Point2 const& p2)
{
    Point2 p(0.5*(p1.x()+p2.x()), 0.5*(p1.y()+p2.y()));
    double d = distance(p, center_);
    p.x() = (p.x() - center_.x())/d;
    p.y() = (p.y() - center_.y())/d;
    p.x() = center_.x()+radius_*p.x();
    p.y() = center_.y()+radius_*p.y();
    return p;
}

} // namespace umeshu
