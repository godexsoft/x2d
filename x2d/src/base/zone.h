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

#include "log.h"
#include "glm.hpp"
#include "math_util.h"
#include "base_object.h"

namespace x2d {
namespace base {

    /**
     * @brief Base class for all zone implementations.
     */
    class zone
    : public base_object
    {
        typedef boost::function<void(object&)> trigger_type;
        
    public:
        zone(kernel& k)
        : base_object(k)
        {
            connect_update(); // we want updates
        }
        
        void set_trigger(const trigger_type& trigger)
        {
            trigger_ = trigger;
        }
        
        /**
         * Check whether the given position is inside the zone.
         * This method must be implemented by all zone classes.
         */
        virtual bool match(const glm::vec3& position) = 0;
        
    protected:
        void update(const clock_info& clock)
        {
            
        }
        
        // TODO context list
        
        // trigger
        trigger_type trigger_;
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
            return rect_.contains_point( point(position.x, position.y) );
        }
        
    private:
        rect    rect_;
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_ZONE_H__
