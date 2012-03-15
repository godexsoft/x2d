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
#include "value_holder.h"

namespace x2d { 
    
    class object;
    
namespace config {
    
    class configuration;
    
} // namespace config
using namespace x2d::config;
    
namespace base {

    class spawner
    {
        friend class object;
        
    public:
        spawner(kernel& k, configuration& conf, 
                const value_holder<std::vector<std::string> >& obj_lst,
                const glm::vec3& pos=glm::vec3(0,0,0),
                int ws=1, float wd=0.0f, float lt=0.0f);
        
        virtual ~spawner()
        {            
        }
        
        void wave_size(int s)
        {
            wave_size_ = s;
        }
        
        void wave_delay(float d)
        {
            wave_delay_ = d;
        }
        
        void life_time(float lt)
        {
            life_time_ = lt;
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
            if(wave_delay_ > 0.0f)
                timer_.set(wave_delay_);
        }
        
        /**
         * Stop generating objects on timer.
         */
        void stop()
        {            
            timer_.cancel();
        }
        
    protected:
        void update(const clock_info& clock) 
        {
            spawn(); // generate one wave
        }

        void set_parent(object* o)
        {
            parent_ = o;
        }
        
    private:
        typedef std::vector<boost::shared_ptr<object> >     objects_vec;

        kernel&         kernel_;
        configuration&  config_;
        timer           timer_;
        
        value_holder<std::vector<std::string> >  obj_lst_;
        objects_vec                              objects_;

        glm::vec3       position_;
        
        int             wave_size_;
        float           wave_delay_;
        float           life_time_;
        
        object*         parent_;
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_SPAWNER_H__
