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

#include "Bounding_box.h"

#include <ostream>
#include <limits>

namespace umeshu {

Bounding_box::Bounding_box()
: ll_(std::numeric_limits<double>::max(),std::numeric_limits<double>::max()),
  ur_(-std::numeric_limits<double>::max(),-std::numeric_limits<double>::max())
{}

Bounding_box::Bounding_box(Point2 const& p1, Point2 const& p2)
: ll_(std::numeric_limits<double>::max(),std::numeric_limits<double>::max()),
  ur_(-std::numeric_limits<double>::max(),-std::numeric_limits<double>::max())
{
    include(p1);
    include(p2);
}

void Bounding_box::include(Point2 const& p)
{
    if (p.x() < ll_.x())
        ll_.x() = p.x();
    if (p.x() > ur_.x())
        ur_.x() = p.x();
    if (p.y() < ll_.y())
        ll_.y() = p.y();
    if (p.y() > ur_.y())
        ur_.y() = p.y();
}

std::ostream& operator<< (std::ostream& os, Bounding_box const& bb)
{
    os << "Bounding box: lower left:  " << bb.ll() << std::endl
       << "              upper right: " << bb.ur() << std::endl
       << "              width: " << bb.width() << std::endl
       << "              height: " << bb.height() << std::endl;
    return os;
}

} // namespace umeshu
