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

#include <boost/shared_ptr.hpp>

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
        camera(const size& frustum, const boost::shared_ptr<camera>& parent=boost::shared_ptr<camera>())
        : parent_(parent)
        , frustum_(frustum)
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
        void position(const glm::vec2& v);
        
        /**
         * Get current camera position
         * @return Camera position given in world coordinates
         */
        const glm::vec2 position() const;
        
        /**
         * Turn camera by angle
         * @param[in] angle The angle
         */
        void rotation(float angle);
        
        /**
         * Get current zoom
         * @return Current zoom level
         */
        const float zoom() const;
        
        /**
         * Zoom scene
         * @param[in] z Zoom level (scale)
         */
        void zoom(float z);
        
        /**
         * Get camera's frustum
         * @return The frustum size
         */
        const size frustum() const;
        
        /**
         * Remove the camera matrix for opengl
         */
        void remove()
        {
            glLoadIdentity();
            is_applied = false;
        }
        
        const glm::mat4 transform() const;
        
        /**
         * Apply the camera matrix for opengl
         */
        void apply();
        
        // get world position from camera-space position
        const glm::vec3 to_world(const glm::vec3& p);
        
        // inverse of 'apply' on point in space
        const point inverted_transformation(const point& p);
                
    private:
        boost::shared_ptr<camera> parent_;
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
