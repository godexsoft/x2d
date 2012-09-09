//
//  null_drivers.cpp
//  x2d
//
//  Created by Alex Kremer on 4/09/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//
//  Happy birthday Vladislav Kolskiy!
//

#include "sound.h"
#include "log.h"

namespace x2d {
namespace snd {
    
    template<>
    void music_obj<null_audio_driver>::play()
    {
        LOG("Playback of music file via NULL driver...");
    }
    
    template<>
    void music_obj<null_audio_driver>::stop()
    {
    }
    
    template<>
    bool music_obj<null_audio_driver>::is_playing() const
    {
        return false;
    }
    
    template<>
    void music_obj<null_audio_driver>::pause()
    {
    }
    
    template<>
    void music_obj<null_audio_driver>::resume()
    {
    }
    
    template<>
    void music_obj<null_audio_driver>::reset()
    {
    }

    template<>
    void music_obj<null_audio_driver>::volume(float v)
    {
    }
    
    template<>
    const float music_obj<null_audio_driver>::volume() const
    {
        return 100.0f;
    }

    template<>
    music_obj<null_audio_driver>::music_obj(const std::string& file_path, bool loop, float gain,
                                            float start, float end)
    {
    }
    
    template<>
    music_obj<null_audio_driver>::music_obj(const boost::shared_ptr<ifdstream>& ifd, bool loop, float gain,
    										float start, float end)
    {
    }
    
    template<>
    music_obj<null_audio_driver>::~music_obj()
    {
    }
    
    template<>
    void music_obj<null_audio_driver>::on_volume_change()
    {
    }
    
    template<>
    sfx_obj<null_audio_driver>::sfx_obj(const boost::shared_ptr<ifdstream>& ifd, bool loop, float pitch, float volume)
    {
    }
    
    template<>
    sfx_obj<null_audio_driver>::~sfx_obj()
    {
    }
    
    template<>
    void sfx_obj<null_audio_driver>::play()
    {
    }
    
    template<>
    void sfx_obj<null_audio_driver>::stop()
    {
    }
    
    template<>
    void sfx_obj<null_audio_driver>::reset()
    {
    }
    
    template<>
    void sfx_obj<null_audio_driver>::pitch(float p)
    {
    }
    
    template<>
    void sfx_obj<null_audio_driver>::on_volume_change()
    {
    }
    
} // namespace snd
namespace snd_driver {

	null_audio_driver::null_audio_driver()
    {
        LOG("NULL Sound engine initializing..");
        LOG("Sound engine started.");
    }
    
	null_audio_driver::~null_audio_driver()
    {
        LOG("NULL Sound engine shutting down..");
        LOG("Sound engine stopped.");
    }
    
} // namespace snd_driver
} // namespace x2d
