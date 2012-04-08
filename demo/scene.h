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
    scene(kernel& k, configuration& conf);
    
protected:
    virtual void update(const clock_info& clock);
    
private:
    
    void on_destroy_object(object& obj);
    
    configuration&  config_;
    boost::shared_ptr<camera> camera_;
    
    std::vector< boost::shared_ptr<base_object> >   objects_;
    
    boost::shared_ptr<object> house_spawner_;
    boost::shared_ptr<object> small_stuff_spawner_;

    boost::shared_ptr<object> house_destroyer_;
    boost::shared_ptr<zone> scenary_destroyer_;
};

#endif // __SCENE_H__
