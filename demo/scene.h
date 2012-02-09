//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __SCENE_H__
#define __SCENE_H__

#include "clock.h"
#include "timer.h"
#include "interpolator.h"
#include "graphics_engine.h"
#include "texture.h"
#include "liverpool_manager.h"
#include "resource_manager.h"
#include "sprite.h"
#include "configuration.h"
#include "fps_counter.h"

#include <vector>

#define lerp(A, B, T)          ((A) + ((T) * ((B) - (A))))

class scene 
: public base_object
{
public:
    scene(kernel& k, configuration& conf)
    : base_object(k)
    , config_(conf)
    {        
        connect_accelerometer_input();
        
        objects_.push_back( config_.create_object("objects.background") );
        objects_.push_back( config_.create_object("objects.road") );
        objects_.push_back( config_.create_object("objects.player") );
        
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
private:
    
    void accelerometer_input(const vector_2d& acceleration) 
    {        
        if(vector_2d::mag_squared(acceleration) > 0.2f)
        {
            static vector_2d r(0,-1);
            
            r = vector_2d ( lerp(r.X(), acceleration.X(), 0.1f), 
                            lerp(r.Y(), acceleration.Y(), 0.1f) );
            
            float d2 = r.X() * r.X() + r.Y() * r.Y();
            float theta = (d2 == 0.0) ? 0.0 : atan2(r.Y(), r.X());                            
            
            config_.get_object<camera>("camera")
                ->rotation( to_deg( 1.570796327f + theta ) );
        }          

    }
    
    configuration&  config_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
