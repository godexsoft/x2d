//
//  spawner.h
//  x2d
//
//  Created by Alex Kremer on 2/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_SPAWNER_H__
#define __X2D_SPAWNER_H__

#include "timer.h"
#include "kernel.h"
#include "log.h"
#include "value.h"

namespace x2d { 
    
    class object;
    
namespace config {
    
    class configuration;
    
} // namespace config
using namespace x2d::config;
    
namespace base {

    class spawner
    {
        friend class x2d::object;
        
    public:
        spawner(kernel& k, configuration& conf, 
                const list_value<std::string>& obj_lst,
                const glm::vec3& pos=glm::vec3(0,0,0),
                const value<int>& ws=1, const value<float>& wd=0.0f,
                float lt=0.0f);
        
        virtual ~spawner()
        {            
        }
        
        void wave_size(const value<int>& s)
        {
            wave_size_ = s;
        }
        
        void wave_delay(const value<float>& d)
        {
            wave_delay_ = d;
        }
        
        void life_time(float lt)
        {
            life_time_ = lt;
        }
        
        void objects(const std::vector<std::string>& v)
        {
            obj_lst_ = list_value<std::string>(v);
        }
        
        void position(const glm::vec3& pos)
        {
            position_ = pos;
        }
        
        const glm::vec3 position() const 
        {
            return position_;
        }
        
        const glm::vec3 world_position() const;
        
        /**
         * Generate one wave.
         */
        void spawn();
        
        /**
         * Start generating objects on timer.
         */
        void start()
        {
            if(wave_delay_.mid() > 0)
            {
                timer_.set(*wave_delay_);
            }
        }
        
        /**
         * Stop generating objects on timer.
         */
        void stop()
        {            
            timer_.cancel();
        }
        
        /**
         * Release ownership of some object spawned by this spawner
         */
        void release(object& obj);
        
    protected:
        void update(const clock_info& clock) 
        {
            spawn(); // generate one wave
            timer_.set(*wave_delay_); // reset with new random delay
        }

        void set_parent(object* o)
        {
            parent_ = o;
        }
        
    private:
        typedef std::vector<boost::weak_ptr<object> >     objects_vec;

        kernel&         kernel_;
        configuration&  config_;
        timer           timer_;
        
        list_value<std::string>     obj_lst_;
        objects_vec                 objects_;

        glm::vec3       position_;
        
        value<int>      wave_size_;
        value<float>    wave_delay_;
        float           life_time_;
        
        object*         parent_;
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_SPAWNER_H__
