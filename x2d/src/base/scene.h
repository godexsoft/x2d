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
        friend class x2d::config::configuration;
        
    public:
        /**
         * @param[in] config The configuration
         * @param[in] t Object traits
         */
        scene(config::configuration& c, const object_traits& t=object_traits());
        
    protected:
        virtual void on_transition_from();
        virtual void on_transition_to();

        void switch_to(const boost::shared_ptr<scene>& sc) const;
        
        configuration& get_config() const;        
        kernel& get_kernel() const;
        
    private:
        configuration& config_;
        
        // callables
        callable<scene*> on_transition_from_;
        callable<scene*> on_transition_to_;
    };

} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_SCENE_H__
