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
#include "texture.h"

namespace x2d { 
    
    class object;
    
namespace config {
    
    class configuration;
    
} // namespace config
using namespace x2d::config;
    
namespace particle_system {

    typedef struct
    {
        float x;
        float y;
        float s;
        float t;
        color_info color;
    } point_sprite;
    
    typedef struct
    {
        glm::vec2 vertex;
        glm::vec2 texture;
        color_info color;
    } textured_colored_vertex;
    
    typedef struct
    {
        textured_colored_vertex bl;
        textured_colored_vertex br;
        textured_colored_vertex tl;
        textured_colored_vertex tr;
    } particle_quad;
    
    struct emitter_settings
    {
        emitter_settings()
        : position_(glm::vec2(0.0f, 0.0f))
        , gravity_(glm::vec2(0.0f, 0.0f))
        {
        }
        
        int                 type_;
        std::string         texture_;
        
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
    
    struct particle
    {
        particle()
        : color_(0.0f, 0.0f, 0.0f, 0.0f)
        , delta_color_(0.0f, 0.0f, 0.0f, 0.0f)
        {            
        }
        
        void init(const emitter_settings& es);
        
        glm::vec2 position_;
        glm::vec2 direction_;
        glm::vec2 start_pos_;
        color_info color_;
        color_info delta_color_;
        float rotation_;
        float rotation_delta_;
        float radial_acceleration_;
        float tangential_acceleration_;
        float radius_;
        float radius_delta_;
        float angle_;
        float degrees_per_second_;
        float size_;
        float size_delta_;
        float time_to_live_;
    };
    
    class emitter
    {
        friend class x2d::object;
        
    public:
        emitter(kernel& k, configuration& conf, const emitter_settings& es);
        
        virtual ~emitter()
        {
            glDeleteBuffers(1, &vertices_id_);
        }
        
    protected:
        void update(const clock_info& clock);
        void draw();
        
    private:
        void reset();
        
        kernel&         kernel_;
        configuration&  config_;
        
        emitter_settings es_;
        
        boost::shared_ptr<texture> texture_;
        std::vector<particle> particles_;
        
        float rate_;
        float elapsed_time_;
        float emit_counter_;
        int particle_count_;
        
        int particle_index_;
        int vertex_index_;
        
        GLuint vertices_id_;
        GLuint vertex_object_id_;

        std::vector<GLushort> indices_;
        std::vector<particle_quad> quads_;
    };
    
} // namespace particle_system
} // namespace x2d
using namespace x2d::particle_system;

#endif // __X2D_SPAWNER_H__
