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

#ifndef __BOUNDARY_SEGMENT_H_INCLUDED__
#define __BOUNDARY_SEGMENT_H_INCLUDED__

#include "Point2.h"

namespace umeshu {

class BoundarySegment
{
public:
    virtual ~BoundarySegment() {}
    virtual Point2 midpoint(Point2 const& p1, Point2 const& p2 ) = 0;
};


class LineBoundarySegment : public BoundarySegment
{
public:
    LineBoundarySegment(Point2 const& p1, Point2 const& p2 ) : p1_(p1), p2_(p2) {}
    Point2 midpoint(Point2 const& p1, Point2 const& p2);
    
private:
    Point2 p1_, p2_;
};


class CircleBoundarySegment : public BoundarySegment
{
public:
    CircleBoundarySegment(Point2 const& p1, Point2 const& p2, Point2 const& p3);
    Point2 midpoint(Point2 const& p1, Point2 const& p2);
    
private:
    Point2 p1_, p2_, p3_;
    Point2 center_;
    double radius_;
};

} // namespace umeshu

#endif // __BOUNDARY_SEGMENT_H_INCLUDED__
