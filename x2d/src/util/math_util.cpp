//
//  math_util.cpp
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "math_util.h"
#include <cstring>

namespace x2d {
namespace math {

    float vector_2d::dist(const vector_2d& l, const vector_2d& r) 
    {
        return sqrt( vector_2d::dist_squared(l, r) );
    }

    float vector_2d::dist_squared(const vector_2d& l, const vector_2d& r) 
    {
        return (l.x_-r.x_)*(l.x_-r.x_) + (l.y_-r.y_)*(l.y_-r.y_);
    }

    void point_transform(float out[4], const float m[16], const float in[4]) 
    {
    #define M(row,col)  m[col*4+row]
        out[0] = M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
        out[1] = M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
        out[2] = M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
        out[3] = M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
    #undef M	
    }

    void matrix_multiply(float * product, const float * a, const float * b) 
    {
        float temp[16];
        int i;
        
    #define A(row,col)  a[(col<<2)+row]
    #define B(row,col)  b[(col<<2)+row]
    #define T(row,col)  temp[(col<<2)+row]
        
        for (i = 0; i < 4; i++) {
            T(i, 0) = A(i, 0) * B(0, 0) + A(i, 1) * B(1, 0) + A(i, 2) * B(2, 0) + A(i,3) * B(3, 0);
            T(i, 1) = A(i, 0) * B(0, 1) + A(i, 1) * B(1, 1) + A(i, 2) * B(2, 1) + A(i,3) * B(3, 1);
            T(i, 2) = A(i, 0) * B(0, 2) + A(i, 1) * B(1, 2) + A(i, 2) * B(2, 2) + A(i,3) * B(3, 2);
            T(i, 3) = A(i, 0) * B(0, 3) + A(i, 1) * B(1, 3) + A(i, 2) * B(2, 3) + A(i,3) * B(3, 3);
        }
        
    #undef A
    #undef B
    #undef T
        memcpy(product, temp, 16 * sizeof(float));
    }

    bool matrix_invert(const float * m, float * out) 
    {
    #define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
    #define M(m,r,c) (m)[(c)*4+(r)]
        
        float wtmp[4][8];
        float m0, m1, m2, m3, s;
        float *r0, *r1, *r2, *r3;
        
        r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
        
        r0[0] = M(m, 0, 0), r0[1] = M(m, 0, 1),
        r0[2] = M(m, 0, 2), r0[3] = M(m, 0, 3),
        r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
        r1[0] = M(m, 1, 0), r1[1] = M(m, 1, 1),
        r1[2] = M(m, 1, 2), r1[3] = M(m, 1, 3),
        r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
        r2[0] = M(m, 2, 0), r2[1] = M(m, 2, 1),
        r2[2] = M(m, 2, 2), r2[3] = M(m, 2, 3),
        r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
        r3[0] = M(m, 3, 0), r3[1] = M(m, 3, 1),
        r3[2] = M(m, 3, 2), r3[3] = M(m, 3, 3),
        r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
        
        if (fabs(r3[0]) > fabs(r2[0]))
            SWAP_ROWS(r3, r2);
        if (fabs(r2[0]) > fabs(r1[0]))
            SWAP_ROWS(r2, r1);
        if (fabs(r1[0]) > fabs(r0[0]))
            SWAP_ROWS(r1, r0);
        if (0.0 == r0[0])
            return false;
        
        m1 = r1[0] / r0[0];
        m2 = r2[0] / r0[0];
        m3 = r3[0] / r0[0];
        s = r0[1];
        r1[1] -= m1 * s;
        r2[1] -= m2 * s;
        r3[1] -= m3 * s;
        s = r0[2];
        r1[2] -= m1 * s;
        r2[2] -= m2 * s;
        r3[2] -= m3 * s;
        s = r0[3];
        r1[3] -= m1 * s;
        r2[3] -= m2 * s;
        r3[3] -= m3 * s;
        s = r0[4];
        if (s != 0.0) {
            r1[4] -= m1 * s;
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r0[5];
        if (s != 0.0) {
            r1[5] -= m1 * s;
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r0[6];
        if (s != 0.0) {
            r1[6] -= m1 * s;
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r0[7];
        if (s != 0.0) {
            r1[7] -= m1 * s;
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }
        
        if (fabs(r3[1]) > fabs(r2[1]))
            SWAP_ROWS(r3, r2);
        if (fabs(r2[1]) > fabs(r1[1]))
            SWAP_ROWS(r2, r1);
        if (0.0 == r1[1])
            return false;
        
        m2 = r2[1] / r1[1];
        m3 = r3[1] / r1[1];
        r2[2] -= m2 * r1[2];
        r3[2] -= m3 * r1[2];
        r2[3] -= m2 * r1[3];
        r3[3] -= m3 * r1[3];
        s = r1[4];
        if (0.0 != s) {
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r1[5];
        if (0.0 != s) {
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r1[6];
        if (0.0 != s) {
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r1[7];
        if (0.0 != s) {
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }
        
        if (fabs(r3[2]) > fabs(r2[2]))
            SWAP_ROWS(r3, r2);
        if (0.0 == r2[2])
            return false;
        
        m3 = r3[2] / r2[2];
        r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
        r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
        
        if (0.0 == r3[3])
            return false;
        
        s = 1.0 / r3[3];
        r3[4] *= s;
        r3[5] *= s;
        r3[6] *= s;
        r3[7] *= s;
        
        m2 = r2[3];	
        s = 1.0 / r2[2];
        r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
        r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
        m1 = r1[3];
        r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
        r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
        m0 = r0[3];
        r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
        r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
        
        m1 = r1[2];	
        s = 1.0 / r1[1];
        r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
        r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
        m0 = r0[2];
        r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
        r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
        
        m0 = r0[1];	
        s = 1.0 / r0[0];
        r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
        r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
        
        M(out, 0, 0) = r0[4];
        M(out, 0, 1) = r0[5], M(out, 0, 2) = r0[6];
        M(out, 0, 3) = r0[7], M(out, 1, 0) = r1[4];
        M(out, 1, 1) = r1[5], M(out, 1, 2) = r1[6];
        M(out, 1, 3) = r1[7], M(out, 2, 0) = r2[4];
        M(out, 2, 1) = r2[5], M(out, 2, 2) = r2[6];
        M(out, 2, 3) = r2[7], M(out, 3, 0) = r3[4];
        M(out, 3, 1) = r3[5], M(out, 3, 2) = r3[6];
        M(out, 3, 3) = r3[7];
        
        return true;
        
    #undef M
    #undef SWAP_ROWS
    }

    float to_rad( float deg ) 
    {
        return deg * (M_PI/180);
    }

    float to_deg( float rad ) 
    {
        return rad * (180/M_PI);
    }
    
    // affine matrix
    affine_matrix::affine_matrix(float m[]) 
    {
        memcpy(data_, m, 9 * sizeof(float));
    }
    
    affine_matrix& affine_matrix::operator *=(const affine_matrix& m) 
    {
        float temp[9];
        int i;
        
#define A(row,col)  data_[(col*3)+row]
#define B(row,col)  m.data_[(col*3)+row]
#define T(row,col)  temp[(col*3)+row]
        
        for (i = 0; i < 3; i++) 
        {
            T(i, 0) = A(i, 0) * B(0, 0) + A(i, 1) * B(1, 0) + A(i, 2) * B(2, 0);
            T(i, 1) = A(i, 0) * B(0, 1) + A(i, 1) * B(1, 1) + A(i, 2) * B(2, 1);
            T(i, 2) = A(i, 0) * B(0, 2) + A(i, 1) * B(1, 2) + A(i, 2) * B(2, 2);
        }
        
#undef A
#undef B
#undef T
        memcpy(data_, temp, 9 * sizeof(float));
        return *this;
    }
    
    const affine_matrix affine_matrix::translation(float x, float y) 
    {
        return affine_matrix((float[]){
            1,0,x,
            0,1,y,
            0,0,1
        });
    }
    
    const affine_matrix affine_matrix::scale(float x, float y) 
    {
        return affine_matrix((float[]){
            x,0,0,
            0,y,0,
            0,0,1
        });
    }

    vector_2d affine_matrix::apply(const vector_2d& v) 
    {
        vector_2d o;
        
        o.X( v.X() * d0 + v.Y() * d1 + d2 );
        o.Y( v.X() * d3 + v.Y() * d4 + d5 );
        
        return o;
    }

    point affine_matrix::apply(const point& p) 
    {
        point o(0,0);
        
        o.x = p.x * d0 + p.y * d1 + d2;
        o.y = p.x * d3 + p.y * d4 + d5;
        
        return o;
    }
    
} // namespace math
} // namespace x2d

