//
//  base_object.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __X2D_BASE_OBJECT_H__
#define __X2D_BASE_OBJECT_H__

#include "log.h"
#include "kernel.h"

namespace x2d 
{
    class base_object 
    {
        friend class kernel;
        
    public:
        base_object(kernel& k)
        : kernel_(k)
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
    };

} // namespace x2d
using namespace x2d;

#endif // __X2D_BASE_OBJECT_H__
