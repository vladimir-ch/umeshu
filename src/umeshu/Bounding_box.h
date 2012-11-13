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

#ifndef __BOUNDING_BOX_H_INCLUDED__
#define __BOUNDING_BOX_H_INCLUDED__

#include "Point2.h"

#include <iosfwd>

namespace umeshu {

class Bounding_box {
public:
    Bounding_box();
    Bounding_box(Point2 const& ll, Point2 const& ur);
    
    Point2 const& ll() const { return ll_; }
    Point2 const& ur() const { return ur_; }
    
    double width() const { return ur_.x() - ll_.x(); }
    double height() const { return ur_.y() - ll_.y(); }
    
    void include(Point2 const& p);
    
private:
    Point2 ll_, ur_;
};

std::ostream& operator<< (std::ostream& os, Bounding_box const& bb);

} // namespace umeshu

#endif // __BOUNDING_BOX_H_INCLUDED__
