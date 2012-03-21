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
#include "log.h"
#include "graphics_engine.h"

#include "glm.hpp"
#include "matrix_transform.hpp"

namespace x2d {
    
    class object;
    
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
        , is_applied(false)
        {            
        }
        
        /**
         * Set new position
         * @param[in] v Position given in world coordinates
         */
        void position(const glm::vec2& v)
        {
            position_ = v;
        }
        
        /**
         * Get current camera position
         * @return Camera position given in world coordinates
         */
        const glm::vec2 position() const
        {
            return position_;
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
            
            rotation_ = glm::clamp(angle, 0.0f, 360.0f);
        }
        
        /**
         * Zoom scene
         * @param[in] z Zoom level (scale)
         */
        void zoom(float z)
        {
            zoom_ = z;
        }
        
        /**
         * Remove the camera matrix for opengl
         */
        void remove()
        {
            glLoadIdentity();
            is_applied = false;
        }
        
        /**
         * Apply the camera matrix for opengl
         */
        void apply();
        
        // get world position from camera-space position
        const glm::vec3 to_world(const glm::vec3& p);
        
        // inverse of 'apply' on point in space
        const point inverted_transformation(const point& p);
        
        /**
         * Get screen position out of camera-space position
         */
        void calculate_in_screen(object* o);
        
    private:
        size        frustum_;
        
        float       rotation_;
        float       zoom_;        
        glm::vec2   position_;
        
        bool        is_applied;
    };

} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_CAMERA_H__
