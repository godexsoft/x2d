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
#include "emitter.h"
#include "font.h"
#include "value.h"
#include "context.h"
#include "body_part.h"
#include "body.h"
#include "object_traits.h"
#include "callable.h"

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
    , public boost::enable_shared_from_this<object>
    {
        friend class x2d::config::configuration;
        
    public:
        object(config::configuration& c, const object_traits& t=object_traits(), spawner* spwn = NULL);
        
        virtual ~object(); 
        
        /**
         * Add a child object.
         * @param[in] child The child object to add
         */
        void add_child(const boost::shared_ptr<object>& child);
        void release(object& obj);
        boost::shared_ptr<object> child_by_name(const std::string& n);
        const std::string name() const;
        
        const glm::vec3 position() const;
        void position(const glm::vec3& p);
        void position(const glm::vec2& p);
        const glm::vec3 world_position() const;
        const bbox screen_bbox() const;
        
        const float rotation() const;
        void rotation(float a);
        
        void scale(float s);
        
        void pivot(const glm::vec2& p);
        const glm::vec2 pivot() const;
        
        void box(const size& s);
        const size box() const;
        
        const std::string text() const;
        void text(const std::string& t);
        
        void set_sprite(const boost::shared_ptr<sprite>& spr);
        
        void set_bgcolor(const color_info& bgcolor);
        
        void visible(bool v);
        
        const glm::vec3& camera_space_position() const;
        void camera_space_position(const glm::vec2& p);
        void camera_space_position(const glm::vec3& p);
        
        const size camera_space_box() const;
        
        const glm::vec2 camera_space_pivot() const;
                
        void reposition_in_parent_space(const size& b);
        
        boost::shared_ptr<spawner> get_spawner();
        
        boost::shared_ptr<emitter> get_emitter();
        
        // collision. can be reimplemented by clients
        virtual void on_collision_begin(object* with);
        virtual void on_collision_end(object* with);
        
        // can be overriden by clients
        virtual bool on_input_began(const glm::vec2& v)
        {
            if(on_input_began_)
            {
                // FIXME: pass location info
                on_input_began_(this);
                return true;
            }

            return false;
        }
        
        virtual bool on_input_moved(const glm::vec2& v)
        {
            if(on_input_moved_)
            {
                // FIXME: pass location info
                on_input_moved_(this);
                return true;
            }

            return false;
        }
        
        virtual bool on_input_ended(const glm::vec2& v)
        {
            if(on_input_ended_)
            {
                // FIXME: pass location info                
                on_input_ended_(this);
                return true;
            }
            
            return false;
        }
        
        void set_lifetime(const float& ttl);
        
        void set_on_input_began(const boost::function<void(object*)>& fn);
        void set_on_input_moved(const boost::function<void(object*)>& fn);
        void set_on_input_ended(const boost::function<void(object*)>& fn);
        
    protected:
        
        virtual void update(const clock_info& clock);        
        virtual void render(const clock_info& clock); 
        
        config::configuration& config()
        {
            return config_;
        }
        
        void set_parent(object* o);
        
        const glm::mat4 final_transform() const;
        
        void destroy_self();

    private:
        
        bool is_camera_space() const;
        
        void set_space(const space& s)
        {
            space_ = s;
        }
        
        void on_lifetime_timer(const clock_info& clock);

        void on_destroy()
        {
            on_destroy_(this);
        }
        
        void on_create()
        {
            on_create_(this);
        }    
        
        config::configuration&  config_;
        timer                   lifetime_timer_;
        
    protected:        
        const std::string           name_;
        const basic_path<char, '.'> path_;
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
        boost::shared_ptr<emitter>   emitter_;
        boost::shared_ptr<zone>      zone_;
        
        bool                         is_visible_;
        
        std::vector< boost::shared_ptr<context> >    contexts_;
        std::vector< boost::shared_ptr<object> >     children_;
        object                                      *parent_;
        bool                                         has_parent_;
        
        // physics
        boost::shared_ptr<body>    body_;
        
        // callables
        callable<object*> on_create_;
        callable<object*> on_destroy_;
        callable<object*> on_input_began_;
        callable<object*> on_input_moved_;
        callable<object*> on_input_ended_;
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_OBJECT_H__
