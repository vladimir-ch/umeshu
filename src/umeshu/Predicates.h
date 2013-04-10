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

#ifndef UMESHU_PREDICATES_H
#define UMESHU_PREDICATES_H

double orient2dfast(double const* pa, double const* pb, double const* pc);
double orient2d(double const* pa, double const* pb, double const* pc);

double orient3dfast(double const* pa, double const* pb, double const* pc, double const* pd );
double orient3d(double const* pa, double const* pb, double const* pc, double const* pd );

double incirclefast(double const* pa, double const* pb, double const* pc, double const* pd);
double incircle(double const* pa, double const* pb, double const* pc, double const* pd);

double inspherefast(double const* pa, double const* pb, double const* pc, double const* pd, double const* pe );
double insphere(double const* pa, double const* pb, double const* pc, double const* pd, double const* pe );

#endif // UMESHU_PREDICATES_H
