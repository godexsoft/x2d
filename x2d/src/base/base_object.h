//
//  base_object.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_BASE_OBJECT_H__
#define __X2D_BASE_OBJECT_H__

#include "log.h"
#include "kernel.h"
#include "space.h"
#include "touch.h"

#include <vector>

namespace x2d 
{
    /**
     * Base class for all objects in x2d
     */
    class base_object 
    {
        friend class kernel;
        
    public:
        /**
         * @param[in] k The kernel
         */
        base_object(kernel& k)
        : kernel_(k)
        {        
        }
        
        virtual ~base_object()
        {
        }
           
    protected:
        kernel& kernel_;        
        
        /**
         * Used to connect the update function to the kernel.
         * Should be invoked in the constructor of a child class.
         */
        void connect_update()
        {
            kernel_.connect_update(this);
        }

        /**
         * Used to connect the render function to the kernel.
         * Should be invoked in the constructor of a child class.
         */
        void connect_render()
        {
            kernel_.connect_render(this);
        }        

        /**
         * Used to connect the touch_input function to the kernel.
         * Should be invoked in the constructor of a child class.
         * Can be invoked multiple times, once for each space.
         */
        void connect_touch_input(space s)
        {
            kernel_.connect_touch_input(s, this);
        }       
        
        /**
         * Base update function. Override this if you need updates.
         * Don't forget to connect.
         * @see connect_update.
         */
        virtual void update(const clock_info& clock) {}

        /**
         * Base render function. Override this if you need rendering.
         * Don't forget to connect.
         * @see connect_render.
         */
        virtual void render(const clock_info& clock) {}
        
        /**
         * Touch input began receiver. One can connect multiple receivers for different spaces.
         * Don't forget to connect.
         * @see connect_touch_input
         */
        virtual void touch_input_began(space s, const std::vector<touch>& touches) {}
        
        /**
         * Touch input moved receiver. One can connect multiple receivers for different spaces.
         * Don't forget to connect.
         * @see connect_touch_input
         */
        virtual void touch_input_moved(space s, const std::vector<touch>& touches) {}
        
        /**
         * Touch input ended receiver. One can connect multiple receivers for different spaces.
         * Don't forget to connect.
         * @see connect_touch_input
         */
        virtual void touch_input_ended(space s, const std::vector<touch>& touches) {}
    };

} // namespace x2d
using namespace x2d;

#endif // __X2D_BASE_OBJECT_H__
