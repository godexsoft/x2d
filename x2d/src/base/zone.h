//
//  zone.h
//  x2d
//
//  Created by Alex Kremer on 2/26/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_ZONE_H__
#define __X2D_ZONE_H__

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

#include "log.h"
#include "glm.hpp"
#include "math_util.h"
#include "base_object.h"
#include "context.h"

namespace x2d {
        
    class object;
        
namespace config {
            
    class configuration;
            
} // namespace config
using namespace x2d::config;

namespace base {

    /**
     * @brief Base class for all zone implementations.
     */
    class zone
    : public base_object
    {
        friend class object;
        typedef boost::function<void(object&)> trigger_type;
        
    public:
        zone(kernel& k)
        : base_object(k)
        , parent_(NULL)
        {
            enable();
        }
        
        void set_trigger(const trigger_type& trigger)
        {
            trigger_ = trigger;
        }
        
        void disable()
        {
            disconnect_update();
        }

        void enable()
        {
            connect_update();
        }
        
        void add_context(const boost::shared_ptr<context>& ctx)
        {
            ctx_list_.push_back(ctx);
        }
                
        /**
         * Check whether the given position is inside the zone.
         * This method must be implemented by all zone classes.
         */
        virtual bool match(const glm::vec3& position) = 0;
        
    protected:
        typedef std::vector< boost::shared_ptr<context> > ctx_vec;
        
        void set_parent(object* o)
        {
            parent_ = o;
        }
        
        void update(const clock_info& clock)
        {
            // check all objects in all contexts
            for(ctx_vec::iterator it = ctx_list_.begin(); it != ctx_list_.end(); ++it)
            {
                bool found = true;
                
                while(found)
                {
                    found = false;
                    
                    for(std::vector<object*>::iterator oit = (*it)->objects_.begin(); 
                            oit != (*it)->objects_.end();)
                    {
                        object* o = *oit;
                        ++oit;
                        
                        if(match( o->position() ) )
                        {
                            if(trigger_) 
                            {
                                trigger_(*o);
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        // trigger
        trigger_type trigger_;
        
        // contexts
        ctx_vec   ctx_list_;
        
        object*   parent_;
    };
    
    /**
     * @brief The rectangular zone implementation
     */
    class rectangular_zone
    : public zone
    {
    public:
        rectangular_zone(kernel& k, const rect& r)
        : zone(k)
        , rect_(r)
        {            
        }
        
        bool match(const glm::vec3& position)
        {
            if(parent_ != NULL)
            {                
                glm::vec3 p = parent_->world_position();
                return rect(p.x + rect_.origin.x, p.y + rect_.origin.y, 
                    rect_.size.width, rect_.size.height).contains_point( point(position.x, position.y) );
            }
            else
            {            
                return rect_.contains_point( point(position.x, position.y) );
            }
        }
        
    private:
        rect    rect_;
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_ZONE_H__
