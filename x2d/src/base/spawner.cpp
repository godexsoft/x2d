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

    spawner::spawner(kernel& k, configuration& conf, const std::vector<std::string>& obj_lst,
                     const glm::vec3& pos, int ws, float wd, float lt)
    : kernel_(k)
    , config_(conf)
    , timer_(k)
    , obj_lst_(obj_lst)
    , position_(pos)  // 0,0,0 by default
    , wave_size_(ws)  // spawn 1 object
    , wave_delay_(wd) // manual
    , life_time_(lt)  // forever
    , parent_(NULL)
    {     
        timer_.handler( boost::bind(&spawner::update, this, _1) );        
        start();
    }
    
    const glm::vec3 spawner::world_position() const
    {
        if(parent_)
            return parent_->world_position() + position_;
        return position_;
    }

    void spawner::spawn()
    {
        for(int i=0; i<wave_size_; ++i)
        {
            boost::shared_ptr<object> o = config_.create_object(obj_lst_.at(0));
            
            glm::vec3 pos = o->position();
            pos += world_position();            
            o->position( pos );
            
            objects_.push_back( o );
        }
        
        LOG("Spawned objects.. new size: %d", objects_.size());
    }
    
} // namespace base
} // namespace x2d