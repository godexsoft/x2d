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

    /**
     * @brief Common sound engine
     */
    class sound_engine_bare
    {
    public:
        void play( const std::string& file_path );                
        
        void stop();
        void pause();
        void resume();
        void reset();

        void master_volume(float v);
        void music_volume(float v);
        void sfx_volume(float v);
                
        const float master_volume() const;
        const float music_volume() const;
        const float sfx_volume() const;
        
    private:        
        X2D_SND_DRIVER driver_;
        boost::shared_ptr<music_obj<X2D_SND_DRIVER> > bg_music_;
    };
    
    typedef boost::details::pool::singleton_default<sound_engine_bare> sound_engine;
    
} // namespace snd
} // namespace x2d
using namespace x2d::snd;

#endif // __X2D_SOUND_H__
