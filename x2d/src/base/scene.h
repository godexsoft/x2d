//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 2/24/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_SCENE_H__
#define __X2D_SCENE_H__

#include "log.h"
#include "object.h"
#include <boost/shared_ptr.hpp>

namespace x2d {
    
    class app_framework;
    
namespace config {
    
    class configuration;
    
} // namespace config
namespace base {

    /**
     * @brief A base class for a scene
     */
    class scene
    : public object
    {
        friend class app_framework;
        
    public:
        /**
         * @param[in] config The configuration
         */
        scene(config::configuration& c);
        
    protected:
        virtual void on_transition_from();
        virtual void on_transition_to();

        configuration& get_config() const
        {
            return config_;
        }
        
        app_framework& get_app() const
        {
            if(app_ == NULL)
            {
                throw sys_exception("scene::get_app() called before app_framework was assigned by the engine");
            }
            
            return *app_;
        }
        
    private:
        configuration& config_;
        app_framework* app_;
    };

} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_SCENE_H__
