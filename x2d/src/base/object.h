//
//  object.h
//  x2d
//
//  Created by Alex Kremer on 2/5/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_OBJECT_H__
#define __X2D_OBJECT_H__

#include "log.h"
#include "base_object.h"
#include "space.h"
#include "animation.h"
#include "value_holder.h"

#include <boost/shared_ptr.hpp>

namespace x2d {
namespace config {
    
    class configuration;
    class object_cfg;
    
} // namespace config
    
    using namespace x2d::config;
    
    /**
     * @brief Object traits
     */
    struct object_traits
    {
        object_traits();
        
        // basic object properties
        value_holder<vector_2d> position;
        value_holder<float>     scale;
        value_holder<float>     rotation;
        
        // input methods requested by object
        bool        want_screen_touch_input;
        bool        want_world_touch_input;
        bool        want_accelerometer_input;
        
        // graphics
        bool        has_animation;
        std::string animation;
        
        bool        has_sprite;
        std::string sprite;
        
        // space
        space       obj_space;
        bool        has_parent;
    };
    
    /**
     * @brief Default x2d object
     */
    class object 
    : public base_object    
    {        
    public:
        object(kernel& k, config::configuration& c, const object_traits& t=object_traits());
        
        virtual ~object() 
        {
        }
        
        /**
         * Add a child object.
         * @param[in] child The child object to add
         */
        void add_child(const boost::shared_ptr<object>& child)
        {
            children_.push_back(child);

            // Now connect update and render if we did not before.
            // Need these connections to update children.
            connect_update();
            connect_render();
        }
        
        void position(const vector_2d& p)        
        {
            position_ = p;
        }

        void rotation(float a)        
        {
            rotation_ = a;
        }
        
        void scale(float s)
        {
            scale_ = s;
        }
        
        const vector_2d& camera_space_position() const
        {
            return camera_space_position_;
        }
        
        float camera_space_scale() const
        {
            return camera_space_scale_;
        }

        float camera_space_rotation() const
        {
            return camera_space_rotation_;
        }

    protected:
        
        virtual void update(const clock_info& clock);        
        virtual void render(const clock_info& clock); 
        
        config::configuration& config()
        {
            return config_;
        }
        
        space       space_;
        
        vector_2d   position_;
        vector_2d   camera_space_position_; // used if space_ == CAMERA_SPACE
        
        float       scale_;
        float       camera_space_scale_; // used if space_ == CAMERA_SPACE
        
        float       rotation_;
        float       camera_space_rotation_; // used if space_ == CAMERA_SPACE
        
        boost::shared_ptr<animation> cur_animation_;
        boost::shared_ptr<sprite>    cur_sprite_;
        
        std::vector< boost::shared_ptr<object> >     children_;
        
    private:
        config::configuration&  config_;
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_OBJECT_H__
