//
//  emitter.h
//  x2d
//
//  Created by Alex Kremer on 3/4/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_EMITTER_H__
#define __X2D_EMITTER_H__

#include "kernel.h"
#include "log.h"
#include "value.h"

namespace x2d { 
    
    class object;
    
namespace config {
    
    class configuration;
    
} // namespace config
using namespace x2d::config;
    
namespace particle_system {

    struct emitter_settings
    {
        emitter_settings()
        : position_(glm::vec2(0.0f, 0.0f))
        , gravity_(glm::vec2(0.0f, 0.0f))
        , start_color_(color_info(0.0f, 0.0f, 0.0f, 0.0f))
        , finish_color_(color_info(0.0f, 0.0f, 0.0f, 0.0f))
        {
        }
        
        int                 type_;
        std::string         sprite_;
        
        value<glm::vec2>    position_;
        value<float>        speed_;
        value<float>        life_span_;
        value<float>        angle_;
        value<glm::vec2>    gravity_;
        value<float>        radial_acceleration_;
        value<float>        tangential_acceleration_;
        value<color_info>   start_color_;
        value<color_info>   finish_color_;
        int                 max_particles_;
        float               duration_;
        int                 blend_src_;
        int                 blend_dst_;
        value<float>        start_size_;
        value<float>        finish_size_;
        value<float>        max_radius_;
        value<float>        min_radius_;
        value<float>        radius_speed_;
        value<float>        rotate_per_second_;
        value<float>        rotation_start_;
        value<float>        rotation_end_; 
    };
    
    class emitter
    {
        friend class x2d::object;
        
    public:
        emitter(kernel& k, configuration& conf, const emitter_settings& es);
        
        virtual ~emitter()
        {            
        }
        
    protected:
        void update(const clock_info& clock) 
        {
        }
        
        void draw()
        {
        }

    private:
        kernel&         kernel_;
        configuration&  config_;
        
        emitter_settings es_;
    };
    
} // namespace particle_system
} // namespace x2d
using namespace x2d::particle_system;

#endif // __X2D_SPAWNER_H__
