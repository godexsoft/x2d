//
//  emitter.cpp
//  x2d
//
//  Created by Alex Kremer on 04/03/2013.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#include "emitter.h"
#include "configuration.h"

namespace x2d {
namespace particle_system {
    
    void particle::init(const float maximum_update_rate, const emitter_settings& es)
    {
        float update_rate_sec = 1.0f / maximum_update_rate;
        position_ = *es.position_;
        start_pos_ = position_;
        
        float a = *es.angle_;
        direction_ = glm::vec2(cosf(a), sinf(a)) * *es.speed_;
        
        float life_span = *es.life_span_;
        
        radius_ = *es.max_radius_;
        radius_delta_ = (radius_ / life_span) * update_rate_sec;
        angle_ = glm::radians(*es.angle_);
        degrees_per_second_ = glm::radians(*es.rotate_per_second_);

        radial_acceleration_ = *es.radial_acceleration_;
        tangential_acceleration_ = *es.tangential_acceleration_;
        
        time_to_live_ = fmaxf(0.0f, life_span);
        
        float start_size = *es.start_size_;
        float finish_size = *es.finish_size_;
        
        size_delta_ = ((finish_size - start_size) / time_to_live_) * update_rate_sec;
        size_ = fmaxf(0.0f, start_size);

        color_info start_color = *es.start_color_;
        color_info finish_color = *es.finish_color_;

        color_ = start_color;
        delta_color_.r = ((finish_color.r - start_color.r) / time_to_live_)  * update_rate_sec;
        delta_color_.g  = ((finish_color.g - start_color.g) / time_to_live_) * update_rate_sec;
        delta_color_.b = ((finish_color.b - start_color.b) / time_to_live_)  * update_rate_sec;
        delta_color_.a = ((finish_color.a - start_color.a) / time_to_live_)  * update_rate_sec;
        
        float start_angle = *es.rotation_start_;
        float end_angle = *es.rotation_end_;
        rotation_ = start_angle;
        rotation_delta_ = (end_angle - start_angle) / time_to_live_;
    }
    
    emitter::emitter(kernel& k, configuration& conf, const emitter_settings& es)
    : kernel_(k)
    , config_(conf)
    , es_(es)
    , texture_( config_.get_object<texture>(es_.texture_) )
    , enabled_(false)
    {
        start();
    }
    
    void emitter::reset()
    {
        rate_ = es_.max_particles_ / es_.life_span_.mid();
        elapsed_time_ = 0.0f;
        particle_count_ = 0;
        emit_counter_ = 0.0f;

        particles_.clear();
        particles_.resize(es_.max_particles_);

        quads_.clear();
        quads_.resize(es_.max_particles_);

        indices_.clear();
        indices_.resize(es_.max_particles_ * 6);
        
        for( int i = 0; i < es_.max_particles_; ++i )
        {
            indices_[i*6+0] = i*4+0;
            indices_[i*6+1] = i*4+1;
            indices_[i*6+2] = i*4+2;
            
            indices_[i*6+5] = i*4+3;
            indices_[i*6+4] = i*4+2;
            indices_[i*6+3] = i*4+1;
        }
        
        for(int i = 0; i < es_.max_particles_; ++i)
        {
            quads_[i].bl.texture.x = 0;
            quads_[i].bl.texture.y = 0;
            
            quads_[i].br.texture.x = texture_->maxs();
            quads_[i].br.texture.y = 0;
            
            quads_[i].tl.texture.x = 0;
            quads_[i].tl.texture.y = texture_->maxt();
            
            quads_[i].tr.texture.x = texture_->maxs();
            quads_[i].tr.texture.y = texture_->maxt();
        }
        
        glGenBuffers(1, &vertices_id_);
        glBindBuffer(GL_ARRAY_BUFFER, vertices_id_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad) * es_.max_particles_, &quads_.at(0), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);        
    }
    
    void emitter::stop()
    {
        enabled_ = false;
        elapsed_time_ = 0;
        emit_counter_ = 0;
    }
    
    void emitter::start()
    {
        enabled_ = true;
        reset();
    }
    
    void emitter::update(const clock_info& clock)
    {
        if(enabled_ && rate_)
        {
            float rate = 1.0f/rate_;
            emit_counter_ += clock.delta_time;
            
            while(particle_count_ < es_.max_particles_ && emit_counter_ > rate)
            {
                particle *p = &particles_[particle_count_++];
                p->init(kernel_.max_frame_rate(), es_);
                emit_counter_ -= rate;
            }
            
            elapsed_time_ += clock.delta_time;
            if(es_.duration_ != -1 && es_.duration_ < elapsed_time_)
            {
                stop();
            }
        }
        
        particle_index_ = 0;
        while(particle_index_ < particle_count_)
        {            
            particle *p = &particles_[particle_index_];
            
            p->time_to_live_ -= clock.delta_time;
            
            if(p->time_to_live_ > 0.0f)
            {
                if (es_.type_ == 1) // TODO: use enum
                {
                    p->angle_ += p->degrees_per_second_ * clock.delta_time;
                    p->radius_ -= p->radius_delta_;
                    
                    glm::vec2 tmp;
                    tmp.x = (*es_.position_).x - cosf(p->angle_) * p->radius_;
                    tmp.y = (*es_.position_).y - sinf(p->angle_) * p->radius_;
                    p->position_ = tmp;
                    
                    if (p->radius_ < *es_.min_radius_)
                    {
                        p->time_to_live_ = 0.0f;
                    }
                }
                else
                {
                    glm::vec2 tmp, radial, tangential;
                    
                    glm::vec2 diff = p->start_pos_;
                    p->position_ -=  diff;
                    
                    if (p->position_.x || p->position_.y)
                    {
                        radial = glm::normalize(p->position_);
                    }
                    
                    tangential = radial;
                    radial *= p->radial_acceleration_;
                    
                    float newy = tangential.x;
                    tangential.x = -tangential.y;
                    tangential.y = newy;
                    tangential *= p->tangential_acceleration_;
                    
                    tmp = (radial + tangential) + *es_.gravity_;
                    tmp *= (float)clock.delta_time;
                    p->direction_ += tmp;
                    tmp = p->direction_ * (float)clock.delta_time;
                    p->position_ += tmp;
                    p->position_ += diff;
                }
                
                p->color_.r += p->delta_color_.r;
                p->color_.g += p->delta_color_.g;
                p->color_.b += p->delta_color_.b;
                p->color_.a += p->delta_color_.a;
                
                p->size_ += p->size_delta_;
                p->rotation_ += (p->rotation_delta_ * clock.delta_time);

                float half_size = p->size_ * 0.5f;
                
                if (p->rotation_)
                {
                    float x1 = -half_size;
                    float y1 = -half_size;
                    float x2 = half_size;
                    float y2 = half_size;
                    float x = p->position_.x;
                    float y = p->position_.y;
                    float r = glm::radians(p->rotation_);
                    float cr = cosf(r);
                    float sr = sinf(r);
                    float ax = x1 * cr - y1 * sr + x;
                    float ay = x1 * sr + y1 * cr + y;
                    float bx = x2 * cr - y1 * sr + x;
                    float by = x2 * sr + y1 * cr + y;
                    float cx = x2 * cr - y2 * sr + x;
                    float cy = x2 * sr + y2 * cr + y;
                    float dx = x1 * cr - y2 * sr + x;
                    float dy = x1 * sr + y2 * cr + y;
                    
                    quads_[particle_index_].bl.vertex.x = ax;
                    quads_[particle_index_].bl.vertex.y = ay;
                    quads_[particle_index_].bl.color = p->color_;
                    
                    quads_[particle_index_].br.vertex.x = bx;
                    quads_[particle_index_].br.vertex.y = by;
                    quads_[particle_index_].br.color = p->color_;
                    
                    quads_[particle_index_].tl.vertex.x = dx;
                    quads_[particle_index_].tl.vertex.y = dy;
                    quads_[particle_index_].tl.color = p->color_;
                    
                    quads_[particle_index_].tr.vertex.x = cx;
                    quads_[particle_index_].tr.vertex.y = cy;
                    quads_[particle_index_].tr.color = p->color_;
                }
                else
                {
                    quads_[particle_index_].bl.vertex.x = p->position_.x - half_size;
                    quads_[particle_index_].bl.vertex.y = p->position_.y - half_size;
                    quads_[particle_index_].bl.color = p->color_;
                    
                    quads_[particle_index_].br.vertex.x = p->position_.x + half_size;
                    quads_[particle_index_].br.vertex.y = p->position_.y - half_size;
                    quads_[particle_index_].br.color = p->color_;
                    
                    quads_[particle_index_].tl.vertex.x = p->position_.x - half_size;
                    quads_[particle_index_].tl.vertex.y = p->position_.y + half_size;
                    quads_[particle_index_].tl.color = p->color_;
                    
                    quads_[particle_index_].tr.vertex.x = p->position_.x + half_size;
                    quads_[particle_index_].tr.vertex.y = p->position_.y + half_size;
                    quads_[particle_index_].tr.color = p->color_;
                }
                
                ++particle_index_;
            } else {
                
                if(particle_index_ != particle_count_ - 1)
                {
                    particles_[particle_index_] = particles_[particle_count_ - 1];
                }
                
                --particle_count_;
            }
        }
    }
    
    void emitter::draw()
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glEnable(GL_BLEND);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertices_id_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particle_quad) * particle_index_, &quads_.at(0));
        
        glVertexPointer(2, GL_FLOAT, sizeof(textured_colored_vertex), 0);
        glColorPointer(4, GL_FLOAT, sizeof(textured_colored_vertex), (GLvoid*) offsetof(textured_colored_vertex, color));
        glTexCoordPointer(2, GL_FLOAT, sizeof(textured_colored_vertex), (GLvoid*) offsetof(textured_colored_vertex, texture));
        
        graphics_engine::instance().bind_texture(texture_->name());
        glBlendFunc(es_.blend_src_, es_.blend_dst_);
        
        glDrawElements(GL_TRIANGLES, particle_index_ * 6, GL_UNSIGNED_SHORT, &indices_.at(0));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDisable(GL_BLEND);
        glDisableClientState(GL_COLOR_ARRAY);
    }
    
} // namespace particle_system
} // namespace x2d