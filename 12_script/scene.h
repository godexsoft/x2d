//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __SCENE_H__
#define __SCENE_H__

#include "configuration.h"

#include <vector>

class scene 
: public base_object
{
public:
    scene(kernel& k, configuration& conf)
    : base_object(k)
    , config_(conf)
    {
        connect_touch_input(WORLD_SPACE);
        
        listener_ = config_.create_sys_object<listener>("listener");
//        listener_->set_callback(boost::bind(&scene::on_test_event, this));
//        k.get_event_manager()->connect_listener("test", boost::bind(&scene::on_test_event, this));
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches) 
    {
        boost::shared_ptr<script> scr = config_.create_sys_object<script>("ref");
        config_.get_scripting_engine().execute(scr);        
    }
    
private:
    void on_test_event()
    {
        LOG("TEST EVENT HAPPENED IN SCENE!!");
    }
    
    configuration&  config_;
    boost::shared_ptr<listener> listener_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
