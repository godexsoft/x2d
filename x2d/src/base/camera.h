//
//  camera.h
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CAMERA_H__
#define __X2D_CAMERA_H__

#include "math_util.h"
#include "graphics_engine.h"

namespace x2d {
namespace base {

    /**
     * @brief A basic 2d camera
     */
    class camera
    {
        friend class viewport;
        
    public:
        /**
         * @param[in] frustum The camera's frustum
         */
        camera(const size& frustum)
        : frustum_(frustum)
        , rotation_(0.0f)
        , zoom_(1.0f)
        , position_(0.0f, 0.0f)
        {            
        }
        
        /**
         * Set new position
         * @param[in] v Position given in world coordinates
         */
        void position(const vector_2d& v)
        {
            position_ = v;
        }
        
        /**
         * Turn camera by angle
         * @param[in] angle The angle
         */
        void rotation(float angle)
        {
            if(angle < 0.0f) 
            {
                angle = 360.0f + angle;    
            }
            
            rotation_ = clamp(angle, 0.0f, 360.0f);
        }
        
        /**
         * Zoom scene
         * @param[in] z Zoom level (scale)
         */
        void zoom(float z)
        {
            zoom_ = z;
        }
        
        void apply()
        {
            glLoadIdentity();

            // move to center of screen
            glTranslatef(frustum_.width / 2.0f, 
                         frustum_.height / 2.0f, 0.0);
            
            // rotate about center of screen
            if(rotation_ != 0.0f)
            {
                glRotatef(rotation_, 0, 0, 1);
            }

            // zoom scene
            if(zoom_ != 1.0f)
            {
                glScalef(zoom_, zoom_, 1.0);
            }

            // move to camera position
            glTranslatef(position_.X(), position_.Y(), 0.0);
        }
        
        // inverse of 'apply' on point in space
        const point inverted_transformation(const point& p)
        {
            affine_matrix m = affine_matrix::translation(-frustum_.width/2.0f, -frustum_.height/2.0f);
            m              *= affine_matrix::rotate( math::to_rad(360.0f-rotation_) );
            m              *= affine_matrix::scale(1.0f/zoom_, 1.0f/zoom_);
            m              *= affine_matrix::translation(-position_.X(), -position_.Y());
            return m.apply(p);
        }

    private:
        size        frustum_;
        
        float       rotation_;
        float       zoom_;        
        vector_2d   position_;
    };

} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_CAMERA_H__
