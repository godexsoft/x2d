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
#include "configuration.h"
#include "object.h"
#include "log.h"

namespace x2d {
namespace base {

    class spawner
    {
    public:
        spawner(kernel& k, configuration& conf, int ws=1, float wd=0.0f, float lt=0.0f)
        : kernel_(k)
        , config_(conf)
        , timer_(k)
        , wave_size_(ws)  // spawn 1 object
        , wave_delay_(wd) // manual
        , life_time_(lt)  // forever
        {     
            timer_.handler( boost::bind(&spawner::update, this, _1) );
        }
        
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
        
        /**
         * Generate one wave.
         */
        void spawn()
        {
            for(int i=0; i<wave_size_; ++i)
            {
                boost::shared_ptr<object> o = config_.create_object("objects.obj1");
//                o->position( template_.position.get(config_) );
                objects_.push_back( o );
            }
            
            LOG("Spawned objects.. new size: %d", objects_.size());
        }
        
        /**
         * Start generating objects on timer.
         */
        void start()
        {    
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

    private:
        typedef std::vector<boost::shared_ptr<object> >     objects_vec;

        kernel&         kernel_;
        configuration&  config_;
        timer           timer_;
        
        objects_vec     objects_;
        
        int             wave_size_;
        float           wave_delay_;
        float           life_time_;
        
        object_traits   template_;
        
        
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_SPAWNER_H__
