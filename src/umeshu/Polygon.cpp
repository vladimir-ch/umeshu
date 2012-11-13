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

#include "Polygon.h"

namespace umeshu {

Bounding_box Polygon::bounding_box() const
{
    Bounding_box bb;
    for (vertex_const_iterator iter = vertices_begin(); iter != vertices_end(); ++iter) {
        bb.include(*iter);
    }
    return bb;
}

Polygon Polygon::triangle()
{
    Polygon poly;
    poly.append_vertex(Point2(-5.0,0.0));
    poly.append_vertex(Point2(5.0,0.0));
    poly.append_vertex(Point2(-0.5,2.0));
    return poly;
}

Polygon Polygon::island()
{
    Polygon poly;

    poly.append_vertex(Point2(0.35790337, 0.38864577));
    poly.append_vertex(Point2(0.31071450, 0.40218492));
    poly.append_vertex(Point2(0.26291196, 0.39019645));
    poly.append_vertex(Point2(0.21429804, 0.38283820));
    poly.append_vertex(Point2(0.16613300, 0.39383360));
    poly.append_vertex(Point2(0.11818326, 0.40414870));
    poly.append_vertex(Point2(0.08260139, 0.37538306));
    poly.append_vertex(Point2(0.05361811, 0.34207291));
    poly.append_vertex(Point2(0.02510484, 0.30843907));
    poly.append_vertex(Point2(0.00000000, 0.27304858));
    poly.append_vertex(Point2(0.01116075, 0.23446584));
    poly.append_vertex(Point2(0.05253742, 0.21147779));
    poly.append_vertex(Point2(0.08739790, 0.18215957));
    poly.append_vertex(Point2(0.13184782, 0.16365548));
    poly.append_vertex(Point2(0.17678370, 0.14569794));
    poly.append_vertex(Point2(0.20997123, 0.11555512));
    poly.append_vertex(Point2(0.23487661, 0.08013357));
    poly.append_vertex(Point2(0.27428417, 0.05577003));
    poly.append_vertex(Point2(0.30609623, 0.02448313));
    poly.append_vertex(Point2(0.34462248, 0.00000000));
    poly.append_vertex(Point2(0.38499725, 0.00703740));
    poly.append_vertex(Point2(0.41201013, 0.04110566));
    poly.append_vertex(Point2(0.44407198, 0.07251598));
    poly.append_vertex(Point2(0.47554188, 0.10433305));
    poly.append_vertex(Point2(0.48615282, 0.14127188));
    poly.append_vertex(Point2(0.48526458, 0.18209454));
    poly.append_vertex(Point2(0.48934181, 0.22286040));
    poly.append_vertex(Point2(0.51277842, 0.25712118));
    poly.append_vertex(Point2(0.55446391, 0.27665125));
    poly.append_vertex(Point2(0.59998065, 0.26611232));
    poly.append_vertex(Point2(0.63374109, 0.23635467));
    poly.append_vertex(Point2(0.68219858, 0.22657993));
    poly.append_vertex(Point2(0.72344682, 0.20595516));
    poly.append_vertex(Point2(0.76098068, 0.18314759));
    poly.append_vertex(Point2(0.81015186, 0.18044212));
    poly.append_vertex(Point2(0.85789280, 0.17704054));
    poly.append_vertex(Point2(0.90231042, 0.15823834));
    poly.append_vertex(Point2(0.94725198, 0.14091650));
    poly.append_vertex(Point2(0.99263450, 0.14300645));
    poly.append_vertex(Point2(1.00000000, 0.18113114));
    poly.append_vertex(Point2(0.97482369, 0.21584709));
    poly.append_vertex(Point2(0.93601160, 0.24166059));
    poly.append_vertex(Point2(0.89769796, 0.26773059));
    poly.append_vertex(Point2(0.88612883, 0.30756282));
    poly.append_vertex(Point2(0.88036141, 0.34822807));
    poly.append_vertex(Point2(0.86637401, 0.38744400));
    poly.append_vertex(Point2(0.87126547, 0.42698375));
    poly.append_vertex(Point2(0.87836588, 0.46738117));
    poly.append_vertex(Point2(0.86679577, 0.50495740));
    poly.append_vertex(Point2(0.83885184, 0.53889814));
    poly.append_vertex(Point2(0.80885902, 0.57161229));
    poly.append_vertex(Point2(0.77548249, 0.60209880));
    poly.append_vertex(Point2(0.73774132, 0.62777801));
    poly.append_vertex(Point2(0.69229886, 0.64485535));
    poly.append_vertex(Point2(0.64664787, 0.66155981));
    poly.append_vertex(Point2(0.59924870, 0.67318904));
    poly.append_vertex(Point2(0.54986089, 0.67381738));
    poly.append_vertex(Point2(0.50097893, 0.66757989));
    poly.append_vertex(Point2(0.46691922, 0.69313520));
    poly.append_vertex(Point2(0.44449627, 0.72973059));
    poly.append_vertex(Point2(0.40854079, 0.75668093));
    poly.append_vertex(Point2(0.36290331, 0.77039793));
    poly.append_vertex(Point2(0.36624793, 0.79505758));
    poly.append_vertex(Point2(0.37098973, 0.82539964));
    poly.append_vertex(Point2(0.32954240, 0.84365456));
    poly.append_vertex(Point2(0.32368068, 0.87731391));
    poly.append_vertex(Point2(0.36206162, 0.90145428));
    poly.append_vertex(Point2(0.41147458, 0.90716326));
    poly.append_vertex(Point2(0.45332716, 0.92956622));
    poly.append_vertex(Point2(0.50176592, 0.93780257));
    poly.append_vertex(Point2(0.54970945, 0.94883030));
    poly.append_vertex(Point2(0.57634292, 0.97479673));
    poly.append_vertex(Point2(0.53570147, 0.98649919));
    poly.append_vertex(Point2(0.48652917, 0.99385804));
    poly.append_vertex(Point2(0.43997944, 0.98533700));
    poly.append_vertex(Point2(0.40368105, 1.00000000));
    poly.append_vertex(Point2(0.36167108, 0.98189826));
    poly.append_vertex(Point2(0.32369722, 0.95572597));
    poly.append_vertex(Point2(0.28265677, 0.93271435));
    poly.append_vertex(Point2(0.24867592, 0.90286457));
    poly.append_vertex(Point2(0.22284050, 0.86781008));
    poly.append_vertex(Point2(0.22698494, 0.82708497));
    poly.append_vertex(Point2(0.23260900, 0.78640307));
    poly.append_vertex(Point2(0.25654498, 0.75174722));
    poly.append_vertex(Point2(0.29129654, 0.72243251));
    poly.append_vertex(Point2(0.31907943, 0.68878353));
    poly.append_vertex(Point2(0.34391654, 0.65324854));
    poly.append_vertex(Point2(0.37314261, 0.62077328));
    poly.append_vertex(Point2(0.41247022, 0.59552654));
    poly.append_vertex(Point2(0.45405330, 0.57278959));
    poly.append_vertex(Point2(0.46246974, 0.53265774));
    poly.append_vertex(Point2(0.47030304, 0.49222005));
    poly.append_vertex(Point2(0.47073181, 0.45153024));
    poly.append_vertex(Point2(0.44700288, 0.41863218));
    poly.append_vertex(Point2(0.40728793, 0.39400451));

    return poly;
}

Polygon Polygon::letter_a()
{
    Polygon poly;

    poly.append_vertex(Point2(0.00000000, 0.00000000));
    poly.append_vertex(Point2(0.28533333, 0.00000000));
    poly.append_vertex(Point2(0.28533333, 0.02735978));
    poly.append_vertex(Point2(0.22400000, 0.03556772));
    poly.append_vertex(Point2(0.19733333, 0.05335157));
    poly.append_vertex(Point2(0.18666667, 0.09028728));
    poly.append_vertex(Point2(0.19866667, 0.14637483));
    poly.append_vertex(Point2(0.26800000, 0.32147743));
    poly.append_vertex(Point2(0.64533333, 0.32147743));
    poly.append_vertex(Point2(0.70133333, 0.18467852));
    poly.append_vertex(Point2(0.73200000, 0.08207934));
    poly.append_vertex(Point2(0.72266667, 0.05061560));
    poly.append_vertex(Point2(0.70266667, 0.03556772));
    poly.append_vertex(Point2(0.63200000, 0.02735978));
    poly.append_vertex(Point2(0.63200000, 0.00000000));
    poly.append_vertex(Point2(1.00000000, 0.00000000));
    poly.append_vertex(Point2(1.00000000, 0.02735978));
    poly.append_vertex(Point2(0.95733333, 0.03419973));
    poly.append_vertex(Point2(0.92666667, 0.05335157));
    poly.append_vertex(Point2(0.90133333, 0.09028728));
    poly.append_vertex(Point2(0.50800000, 1.00000000));
    poly.append_vertex(Point2(0.48133333, 1.00000000));
    poly.append_vertex(Point2(0.12933333, 0.15458276));
    poly.append_vertex(Point2(0.07600000, 0.06155951));
    poly.append_vertex(Point2(0.03333333, 0.03283174));
    poly.append_vertex(Point2(0.00000000, 0.02735978));

    return poly;
}

Polygon Polygon::square(double size)
{
    Polygon poly;

    poly.append_vertex(Point2(0.0,0.0));
    poly.append_vertex(Point2(size,0.0));
    poly.append_vertex(Point2(size,size));
    poly.append_vertex(Point2(0.0,size));

    return poly;
}

Polygon Polygon::letter_u()
{
    Polygon poly;

    poly.append_vertex(Point2(0,0)); 
    poly.append_vertex(Point2(4,0));
    poly.append_vertex(Point2(4,1));
    poly.append_vertex(Point2(2,1));
    poly.append_vertex(Point2(2,2));
    poly.append_vertex(Point2(4,2));
    poly.append_vertex(Point2(4,3));
    poly.append_vertex(Point2(0,3));
    
    return poly;
}

Polygon Polygon::kidney()
{
    Polygon poly;

    poly.append_vertex(Point2(0.6814,1.2932));
    poly.append_vertex(Point2(0.7148,1.2004));
    poly.append_vertex(Point2(0.7574,1.1074));
    poly.append_vertex(Point2(0.8143,1.0178));
    poly.append_vertex(Point2(0.8963,0.9360));
    poly.append_vertex(Point2(0.9825,0.8672));
    poly.append_vertex(Point2(1.0713,0.8072));
    poly.append_vertex(Point2(1.1610,0.7660));
    poly.append_vertex(Point2(1.2482,0.7350));
    poly.append_vertex(Point2(1.3416,0.7122));
    poly.append_vertex(Point2(1.4344,0.6954));
    poly.append_vertex(Point2(1.5202,0.6852));
    poly.append_vertex(Point2(1.6132,0.6802));
    poly.append_vertex(Point2(1.7064,0.6802));
    poly.append_vertex(Point2(1.7994,0.6946));
    poly.append_vertex(Point2(1.8926,0.7160));
    poly.append_vertex(Point2(1.9848,0.7412));
    poly.append_vertex(Point2(2.0780,0.7758));
    poly.append_vertex(Point2(2.1706,0.8178));
    poly.append_vertex(Point2(2.2616,0.8718));
    poly.append_vertex(Point2(2.3498,0.9444));
    poly.append_vertex(Point2(2.4318,1.0256));
    poly.append_vertex(Point2(2.5044,1.1130));
    poly.append_vertex(Point2(2.5600,1.1984));
    poly.append_vertex(Point2(2.5896,1.2914));
    poly.append_vertex(Point2(2.6132,1.3848));
    poly.append_vertex(Point2(2.6294,1.4774));
    poly.append_vertex(Point2(2.5998,1.5688));
    poly.append_vertex(Point2(2.5333,1.6370));
    poly.append_vertex(Point2(2.4400,1.6230));
    poly.append_vertex(Point2(2.3494,1.5844));
    poly.append_vertex(Point2(2.2576,1.5308));
    poly.append_vertex(Point2(2.1664,1.4872));
    poly.append_vertex(Point2(2.0820,1.4402));
    poly.append_vertex(Point2(1.9928,1.4052));
    poly.append_vertex(Point2(1.8996,1.3756));
    poly.append_vertex(Point2(1.8068,1.3492));
    poly.append_vertex(Point2(1.7136,1.3342));
    poly.append_vertex(Point2(1.6208,1.3278));
    poly.append_vertex(Point2(1.5274,1.3264));
    poly.append_vertex(Point2(1.4346,1.3384));
    poly.append_vertex(Point2(1.3414,1.3596));
    poly.append_vertex(Point2(1.2484,1.3860));
    poly.append_vertex(Point2(1.1556,1.4258));
    poly.append_vertex(Point2(1.0634,1.4642));
    poly.append_vertex(Point2(0.9744,1.5058));
    poly.append_vertex(Point2(0.8816,1.5464));
    poly.append_vertex(Point2(0.7882,1.5496));
    poly.append_vertex(Point2(0.7046,1.4790));
    poly.append_vertex(Point2(0.6890,1.3860));

    return poly;
}

Polygon Polygon::crack()
{
    Polygon poly;

    poly.append_vertex(Point2(-1, -1));
    poly.append_vertex(Point2(1, -1));
    poly.append_vertex(Point2(1, 1));
    poly.append_vertex(Point2(1e-2, 1));
    poly.append_vertex(Point2(0, 0));
    poly.append_vertex(Point2(-1e-2, 1));
    poly.append_vertex(Point2(-1, 1));

    return poly;
}

} // namespace umeshu
