//
//  sound.cpp
//  x2d
//
//  Created by Alex Kremer on 2/12/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "sound.h"
#include "math_util.h"
#include "exceptions.h"
#include "sound_resource.h"
#include "log.h"

namespace x2d {
namespace snd {
    
    void sound_engine_bare::master_volume(float v)
    {
        master_volume_ = glm::clamp(v, 0.0f, 1.0f);

        for(std::vector<sound*>::iterator it = active_objects_.begin();
            it != active_objects_.end(); ++it)
        {
            // notify about the change
            (*it)->on_volume_change();
        }
    }
    
    void sound_engine_bare::music_volume(float v)
    {
        music_volume_ = glm::clamp(v, 0.0f, 1.0f);
        
        for(std::vector<sound*>::iterator it = active_objects_.begin();
            it != active_objects_.end(); ++it)
        {
            // notify about the change
            (*it)->on_volume_change();
        }
    }
    
    void sound_engine_bare::sfx_volume(float v)
    {
        sfx_volume_ = glm::clamp(v, 0.0f, 1.0f);
        
        for(std::vector<sound*>::iterator it = active_objects_.begin();
            it != active_objects_.end(); ++it)
        {
            // notify about the change
            (*it)->on_volume_change();
        }
    }

    const float sound_engine_bare::master_volume() const
    {
        return master_volume_;
    }
    
    const float sound_engine_bare::music_volume() const
    {
        return music_volume_;
    }
    
    const float sound_engine_bare::sfx_volume() const
    {
        return sfx_volume_;
    }
    
} // namespace snd
} // namespace x2d
