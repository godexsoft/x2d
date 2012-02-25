//
//  input_manager.h
//  x2d
//
//  Created by Alex Kremer on 2/3/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_INPUT_MANAGER_H__
#define __X2D_INPUT_MANAGER_H__

#include "touch.h"
#include "glm.hpp"
#include "log.h"

#include <vector>

namespace x2d {

    // forward declare kernel to make friends
    class kernel;
    
namespace input {

    /**
     * @brief This class is used to transform touches using the given affine matrix.
     * 
     * This transformation functor can be used with stl algorithms like transform
     * to apply affine transformations to each touch's location and previous location.
     */
    struct transform_touches 
    : public std::unary_function<const touch, const touch> 
    {        
        glm::mat4& m_;
     
        /**
         * @param[in] m The affine transformation matrix to apply
         */
        transform_touches(glm::mat4& m) 
        : m_(m) 
        {
        }
        
        const touch operator()(const touch& t) 
        {
            glm::vec4 tt (t.location().x, t.location().y, 0.0f, 1.0f);
            glm::vec4 ptt(t.prev_location().x, t.prev_location().y, 0.0f, 1.0f);

            tt = m_ * tt;
            ptt = m_ * ptt;
            
            return touch( point(tt.x, tt.y), point(ptt.x, ptt.y), t.tap_count(), t.timestamp() );
        }
    };
    
    /**
     * @brief Input manager class.
     *
     * This class is used together with the kernel. The kernel of x2d is forwarding
     * screen-space touch and acceleration input from hardware to the input manager and
     * the input manager is applying all required transformations and sends the calculated
     * world-space touches back to the kernel which in turn sends them to any subscribed objects.
     */
    class input_manager
    {
        friend class kernel;
                
    public:
        /**
         * @param[in] k The kernel
         * @param[in] wt true if touch input is required; false otherwise
         * @param[in] wa true if accelerometer input is required; false otherwise         
         */
        input_manager(kernel& k, bool wt, bool wa)        
        : kernel_(k)
        , want_touch_(wt)
        , want_accel_(wa)
        {            
        }
        
        /**
         * Apply affine transformation to every touch in the given collection.
         *
         * @param[in] touches Collection of touches to transform
         * @param[in] mat The affine transformation to apply to every touch
         * @return Collection of transformed touches
         */
        static const std::vector<touch> transform(const std::vector<touch>& touches, glm::mat4& mat) 
        {            
            std::vector<touch> tr;
            tr.reserve(touches.size());
            
            std::transform(touches.begin(), touches.end(), std::back_inserter(tr), transform_touches(mat));
            
            return tr;
        }
        
    private:
        /**
         * Called by the kernel when hardware touch input began.
         * @see kernel
         */
        void on_touches_began(const std::vector<touch>& touches);    
        
        /**
         * Called by the kernel when hardware touch input moved.
         * @see kernel
         */
        void on_touches_moved(const std::vector<touch>& touches);
        
        /**
         * Called by the kernel when hardware touch input ended.
         * @see kernel
         */
        void on_touches_ended(const std::vector<touch>& touches);

        /**
         * Called by the kernel when hardware accelerometer input arrive.
         * @see kernel
         */
        void on_acceleration(float x, float y, float z);
        
        kernel&     kernel_;
        bool        want_touch_;
        bool        want_accel_;
    };
    
} // namespace input
} // namespace x2d
using namespace x2d::input;

#endif // __X2D_INPUT_MANAGER_H__
