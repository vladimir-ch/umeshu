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

#ifndef __POSTSCRIPT_STREAM_H_INCLUDED__
#define __POSTSCRIPT_STREAM_H_INCLUDED__

#include <umeshu/Bounding_box.h>

#include <fstream>
#include <string>

namespace umeshu {
namespace io {

class Postscript_ostream {    
public:
    Postscript_ostream (std::string const& filename, Bounding_box const& bb);
    
    void newpath() {
        of_ << "np\n";
    }
    void closepath() {
        of_ << "cp\n";
    }
    void moveto(double x, double y) {
        double nx, ny;
        transform(x, y, nx, ny);
        of_ << nx << " " << ny << " m\n";
    }
    void lineto(double x, double y) {
        double nx, ny;
        transform(x, y, nx, ny);
        of_ << nx << " " << ny << " l\n";
    }
    void stroke() {
        of_ << "s\n";
    }
    void fill() {
        of_ << "f\n";
    }
    void dot(double x, double y) {
        double nx, ny;
        transform(x, y, nx, ny);
        of_ << nx << " " << ny << " c\n";
    }
    void setrgbcolor(double r, double g, double b) {
        of_ << r << " " << g << " " << b << " sc\n";
    }
    void setgray(double g) {
        of_ << g << " sg\n";
    }

private:
    void transform(double x, double y, double& nx, double& ny) {
        nx = x_scale*x - x_trans;
        ny = y_scale*y - y_trans;
    }

    std::ofstream of_;
    Bounding_box bb_;
    double fig_width_, fig_height_;
    double x_scale, y_scale;
    double x_trans, y_trans;

    static float const default_fig_size;
    static float const default_fig_margin;
    static float const default_line_width;
};

} // namespace io
} // namespace umeshu

#endif // __POSTSCRIPT_STREAM_H_INCLUDED__
