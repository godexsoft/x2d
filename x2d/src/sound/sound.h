//
//  sound.h
//  x2d
//
//  Created by Alex Kremer on 2/12/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_SOUND_H__
#define __X2D_SOUND_H__

#include "resource_manager.h"
#include <boost/pool/detail/singleton.hpp>
#include <boost/shared_ptr.hpp>

#include "config.h"

#if defined X2D_SND_DRIVER
    #include "audio_queue.h"
    #include "openal_fx.h"
#else
    #error Selected audio driver does not exist.
#endif

namespace x2d {
namespace snd {
    
    class sound;
    
    /**
     * @brief Common sound engine
     */
    class sound_engine_bare
    {
        friend class sound;
        friend class boost::details::pool::singleton_default<sound_engine_bare>;
        
    public:
        void master_volume(float v);
        void music_volume(float v);
        void sfx_volume(float v);
                
        const float master_volume() const;
        const float music_volume() const;
        const float sfx_volume() const;
        
    private:
        sound_engine_bare()
        : master_volume_(1.0f)
        , music_volume_(1.0f)
        , sfx_volume_(1.0f)
        {            
        }
        
        // register sfx/music
        void reg(sound* p)
        {
            active_objects_.push_back(p);
        }
        
        // deregister sfx/music
        void unreg(sound* p)
        {
            active_objects_.erase(
                std::remove( active_objects_.begin(), active_objects_.end(), p),
                    active_objects_.end());
        }
        
        X2D_SND_DRIVER driver_;
        std::vector<sound*> active_objects_;
        
        float master_volume_;
        float music_volume_;
        float sfx_volume_; 
    };
    
    typedef boost::details::pool::singleton_default<sound_engine_bare> sound_engine;
    
} // namespace snd
} // namespace x2d
using namespace x2d::snd;

#endif // __X2D_SOUND_H__
