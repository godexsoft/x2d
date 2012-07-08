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
#include "spawner.h"
#include "font.h"
#include "value_holder.h"
#include "context.h"
#include "body_part.h"
#include "body.h"
#include "object_traits.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace x2d {
namespace config {
    
    class configuration;
    class object_cfg;
    
} // namespace config
using namespace x2d::config;
        
    /**
     * @brief Default x2d object
     */
    class object 
    : public base_object    
    {        
    public:
        object(kernel& k, config::configuration& c, const object_traits& t=object_traits());
        
        virtual ~object(); 
        
        /**
         * Add a child object.
         * @param[in] child The child object to add
         */
        void add_child(const boost::shared_ptr<object>& child);
        
        void release(object& obj);

        const glm::vec3 position() const;
        
        void position(const glm::vec3& p);
        
        void position(const glm::vec2& p);
        
        const glm::vec3 world_position() const;
        
        const float rotation() const;
        
        const std::string text() const;
        
        void rotation(float a);
        
        void scale(float s);
        
        void pivot(const glm::vec2& p);
        
        void box(const size& s);
        
        void text(const std::string& t);
        
        void set_sprite(const boost::shared_ptr<sprite>& spr);
        
        void visible(bool v);
        
        const glm::vec3& camera_space_position() const;

        const size box() const;
        
        const size camera_space_box() const;
        
        const glm::vec2 pivot() const;
        
        const glm::vec2 camera_space_pivot() const;
                
        boost::shared_ptr<object> child_by_name(const std::string& n);
        
        void reposition_in_parent_space(const size& b);
        
    protected:
        
        virtual void update(const clock_info& clock);        
        virtual void render(const clock_info& clock); 
        
        config::configuration& config()
        {
            return config_;
        }
        
        void set_parent(object* o)
        {
            parent_ = o;
        }
        
    private:
        config::configuration&  config_;

    protected:        
        const std::string           name_;
        boost::shared_ptr<camera>   camera_;
        space                       space_;
        parent_space                parent_space_;
        
        glm::vec3   position_;
        glm::vec3   camera_space_position_; // used if space_ == CAMERA_SPACE
        
        float       scale_;
        float       rotation_;
        
        size        box_;
        size        camera_space_box_;
        
        glm::vec2   pivot_;
        glm::vec2   camera_space_pivot_;
        
        color_info  bgcolor_;
        
        boost::shared_ptr<animation>   cur_animation_;
        boost::shared_ptr<sprite>      cur_sprite_;
        boost::shared_ptr<font>        cur_font_;
        boost::shared_ptr<std::string> text_;
        alignment                      align_;
        
        boost::shared_ptr<spawner>   spawner_;
        boost::shared_ptr<zone>      zone_;
        
        bool                         is_visible_;
        
        std::vector< boost::shared_ptr<context> >    contexts_;
        std::vector< boost::shared_ptr<object> >     children_;
        object                                      *parent_;
        bool                                         has_parent_;
        
        // physics
        boost::shared_ptr<body>    body_;
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_OBJECT_H__
