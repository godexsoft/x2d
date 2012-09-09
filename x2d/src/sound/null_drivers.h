//
//  null_drivers.h
//  x2d
//
//  Created by Alex Kremer on 4/09/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//
//  Happy birthday Vladislav Kolskiy!
//

#pragma once
#ifndef __X2D_NULL_AUDIO_H__
#define __X2D_NULL_AUDIO_H__

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <string>

#include "filesystem.h"

namespace x2d {
namespace snd_driver {

    class null_audio_driver;
    
} // namespace snd_driver
namespace snd {
    
    template<typename D>
    class music_obj
    {
        friend class snd_driver::null_audio_driver;
        
    public:
        music_obj(const std::string& file_path, bool loop=false, float gain=1.0f, 
                  float start=0.0f, float end=0.0f);
        
        music_obj(const boost::shared_ptr<ifdstream>& ifd, bool loop=false, float gain=1.0f,
                  float start=0.0f, float end=0.0f);
                
        ~music_obj();
        
        void set_callback(const boost::function<void(bool)>& cb)
        {
        }
        
        const float total_time() const
        {
        	return 0.0f;
        }
        
        void play();
        void stop();
        void pause();
        void resume();
        void reset();
        bool is_playing() const;

        void volume(float v);
        const float volume() const;
        
        void on_volume_change();
    };
    
    template<typename D>
    class sfx_obj
    {
        friend class snd_driver::null_audio_driver;
        
    public:
        sfx_obj(const boost::shared_ptr<ifdstream>& ifd, bool loop=false, float pitch=1.0f, float volume=1.0f);
        ~sfx_obj();
        
        void play();
        void stop();
        void reset();
        void pitch(float p);
        
        void on_volume_change();
    };
    
} // namespace snd
    
namespace snd_driver {
           
    class null_audio_driver
    {
    public:
    	null_audio_driver();
        ~null_audio_driver();
    };
    
} // namespace snd_driver
} // namespace x2d
using namespace x2d::snd_driver;

#endif // __X2D_NULL_AUDIO_H__
