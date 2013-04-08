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

    spawner::spawner(kernel& k, configuration& conf, 
                     const list_value<std::string>& obj_lst,
                     const glm::vec3& pos, const value<int>& ws,
                     const value<float>& wd, float lt)
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
        LOG("SPAWNER CREATED WITH: %d elements", obj_lst_.get_list().size());
        for(int i=0; i<obj_lst_.get_list().size(); ++i)
        {
            LOG("OBJECT IN SPAWNER: %s", obj_lst_.get_list().at(i).c_str());
        }
        
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
        for(int i=0; i<*wave_size_; ++i)
        {
            boost::shared_ptr<object> o = config_.create_object_1(*obj_lst_, this);
            objects_.push_back( o );
        }
        
        LOG("Spawned objects.. new size: %d", objects_.size());
    }
    
    void spawner::release(object& obj)
    {
        // TODO: this code is ugly. do something :)
        for( objects_vec::iterator it = objects_.begin();
            it != objects_.end(); ++it)
        {
            if( boost::shared_ptr<object> l = (*it).lock() )
            {
                if(l.get() == &obj)
                {
                    LOG("ERASE object link from spawner on request");
                    objects_.erase(it);
                    return;
                }
            }
        }
        
        LOG("couldn't find object in spawner ownership!");
    }

} // namespace base
} // namespace x2d