//
//  sound_resource.cpp
//  x2d
//
//  Created by Alex Kremer on 2/18/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "sound_resource.h"
#include "sound.h"
#include "platform.h"

namespace x2d {
namespace snd {
     
    sound::sound()
    {
        sound_engine::instance().reg(this);
    }
    
    sound::~sound()
    {
        sound_engine::instance().unreg(this);
    }
    
    void sfx::play()
    {
        obj_->reset();
        obj_->pitch(pitch_.get(conf_));
        obj_->play();
    }     

    void sfx::stop()
    {
        obj_->reset();
    }     
    
    void music::update(const clock_info& ci)
    {
        // fade stuff goes here
        if(start_ != 0.0)
        {
            double t = ci.time-start_;            
            
            if(t < fade_in_)
            {
                volume_level_ = (t/fade_in_) * want_volume_level_;
                obj_->volume(volume_level_);
            }
            else if(fade_out_start_ > 0.0 && t >= fade_out_start_ && t-fade_out_start_<fade_out_)
            {
                volume_level_ = want_volume_level_-(((t-fade_out_start_)/fade_out_) * want_volume_level_);
                obj_->volume(volume_level_);
            }
            
            if(loop_ && t >= total_len_)
            {
                // reset
                start_ = platform::time::current_time();
                if(fade_in_ == 0.0f)
                {
                    // set volume straight away
                    obj_->volume(want_volume_level_);
                }
            }
        }
    }

    void music::play()
    {
        obj_->play();
    }

    void music::on_playback(bool playing)
    {
        LOG("Playing: %s", playing? "yes":"no");
        if(playing)
        {
            start_ = platform::time::current_time();
        }
        else
        {
            start_ = 0.0;
        }
    }
    
} // namespace snd
} // namespace x2d   
