//
//  math_util.cpp
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "math_util.h"
#include "log.h"
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

    const affine_matrix affine_matrix::rotate(float angle)
    {
        return affine_matrix((float[]){
            cosf(angle),-sinf(angle),0,
            sinf(angle),cosf(angle),0,
            0,0,1
        });
    }     

    const affine_matrix affine_matrix::inverse()
    {
#define B(row,col)  m.data_[(col*3)+row]
        affine_matrix m;
        
        double det = determinant();
        
        if ( fabs(det) < 1e-2 )
        {
            LOG("Failed matrix inverse");
            memset( m.data_, 0, 9 * sizeof(float) );
            return m;
        }
        
        double one_over_det = 1.0 / det;
        
        for(int y = 0; y < 3; ++y )
        {
            for( int x = 0; x < 3; ++x )
            {
                /* Rule is inverse = 1/det * minor of the TRANSPOSE matrix.  *
                 * Note (y,x) becomes (x,y) INTENTIONALLY here!              */
                B(y,x) = determinant_of_minor(x,y) * one_over_det;
                
                /* (y0,x1)  (y1,x0)  (y1,x2)  and (y2,x1)  all need to be negated. */
                if(1 == ((x + y) % 2) )
                {
                    B(y,x) = -B(y,x);
                }
            }
        }
#undef B
        return m;    
    }
    
    double affine_matrix::determinant_of_minor(int y, int x)
    {
        int x1 = x == 0 ? 1 : 0;  /* always either 0 or 1 */
        int x2 = x == 2 ? 1 : 2;  /* always either 1 or 2 */
        int y1 = y == 0 ? 1 : 0;  /* always either 0 or 1 */
        int y2 = y == 2 ? 1 : 2;  /* always either 1 or 2 */
        
#define A(row,col)  data_[(col*3)+row]
        return ( A(y1,x1)  *  A(y2,x2) )
            -  ( A(y1,x2)  *  A(y2,x1) );
#undef A
    }
    
    double affine_matrix::determinant()
    {
#define A(row,col)  data_[(col*3)+row]
        return ( A(0,0)  *  determinant_of_minor(0,0) )
            -  ( A(0,1)  *  determinant_of_minor(0,1) )
            +  ( A(0,2)  *  determinant_of_minor(0,2) );
#undef A
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

