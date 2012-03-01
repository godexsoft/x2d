//
//  spawner.cpp
//  x2d
//
//  Created by Alex Kremer on 01/03/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "spawner.h"
#include "configuration.h"

namespace x2d {    
namespace base {

    spawner::spawner(kernel& k, configuration& conf, int ws, float wd, float lt)
    : kernel_(k)
    , config_(conf)
    , timer_(k)
    , wave_size_(ws)  // spawn 1 object
    , wave_delay_(wd) // manual
    , life_time_(lt)  // forever
    {     
        timer_.handler( boost::bind(&spawner::update, this, _1) );        
        start();
    }
    
    void spawner::spawn()
    {
        for(int i=0; i<wave_size_; ++i)
        {
            boost::shared_ptr<object> o = config_.create_object("objects.obj1");
            //                o->position( template_.position.get(config_) );
            objects_.push_back( o );
        }
        
        LOG("Spawned objects.. new size: %d", objects_.size());
    }
    
} // namespace base
} // namespace x2d