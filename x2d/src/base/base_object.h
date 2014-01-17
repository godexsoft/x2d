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
#include <boost/tuple/tuple.hpp>

namespace x2d 
{
    /**
     * Base class for all objects in x2d
     */
    class base_object 
    : public boost::signals2::trackable // automatic management of slot connections
    {
        friend class kernel;

        typedef std::map<space, kernel::input_connections_t > input_connections_map;

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
            LOG("base_object destruction...");
            update_connection_.disconnect();
            render_connection_.disconnect();
        }
           
    protected:
        kernel& kernel_;        
        
        /**
         * Used to connect the update function to the kernel.
         * Should be invoked in the constructor of a child class.
         */
        void connect_update()
        {
            if(!update_connection_.connected())
            {
                update_connection_ = kernel_.connect_update(this);
            }
        }
        
        /**
         * Disconnect the update function from the kernel
         */
        void disconnect_update()
        {
            if(update_connection_.connected())
            {
                update_connection_.disconnect();
            }
        }

        /**
         * Used to connect the render function to the kernel.
         * Should be invoked in the constructor of a child class.
         * @param[in] z The priority. The bigger z is the faster it will get rendered
         * @param[in] camera_space True if object is in camera space; false otherwise
         */
        void connect_render(float z, bool camera_space = false)
        {
            if(!render_connection_.connected())
            {
                render_connection_ = kernel_.connect_render(this, z, camera_space);
            }
        }

        /**
         * Disconnect the render function from the kernel
         */
        void disconnect_render()
        {
            if(render_connection_.connected())
            {
                render_connection_.disconnect();
            }
        }
        
        /**
         * Used to connect the touch_input* functions to the kernel.
         * Should be invoked in the constructor of a child class.
         * Can be invoked multiple times, once for each space.
         * Safe to invoke multiple times for same space. Will be ignored.
         */
        void connect_touch_input(space s)
        {
            input_connections_map::iterator it = touch_input_connections_.find(s);
            if(it == touch_input_connections_.end())
            {
                touch_input_connections_[s] = kernel_.connect_touch_input(s, this);
            }
        }

        /**
         * Used to disconnect input for a given space.
         * Safe to invoke multiple times.
         */
        void disconnect_touch_input(space s)
        {
            input_connections_map::iterator it = touch_input_connections_.find(s);
            if(it != touch_input_connections_.end())
            {
                touch_input_connections_[s].get<0>().disconnect();
                touch_input_connections_[s].get<1>().disconnect();
                touch_input_connections_[s].get<2>().disconnect();
            }
        }

        /**
         * Used to connect the accelerometer_input function to the kernel.
         * Should be invoked in the constructor of a child class.
         */
        void connect_accelerometer_input()
        {
            kernel_.connect_accelerometer_input(this);
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
        
        /**
         * Accelerometer input receiver.
         * @see connect_accelerometer_input
         */
        virtual void accelerometer_input(const glm::vec3& acceleration) {}
        
    private:
        boost::signals2::connection update_connection_;
        boost::signals2::connection render_connection_;

        input_connections_map touch_input_connections_;
    };

} // namespace x2d
using namespace x2d;

#endif // __X2D_BASE_OBJECT_H__
