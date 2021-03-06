//
//  openal_fx.h
//  x2d
//
//  Created by Alex Kremer on 2/16/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_OPENAL_FX_H__
#define __X2D_OPENAL_FX_H__

#include <boost/shared_ptr.hpp>    
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <AudioToolbox/AudioFile.h>

#include "filesystem.h"

#define NUM_BUFFERS 4 // maximum 4 buffers
#define BUFFER_SIZE_BYTES 1048576 // 1 MB buffers

namespace x2d {
namespace snd_driver {
            
    class audio_queue_driver;
    
} // namespace snd_driver
namespace snd {
    
    template<typename D>
    class sfx_obj
    {
        friend class snd_driver::audio_queue_driver;
        
    public:
        sfx_obj(const boost::shared_ptr<ifdstream>& ifd, bool loop=false, float pitch=1.0f, float volume=1.0f);
        ~sfx_obj();
        
        void play();
        void stop();
        void reset();
        void pitch(float p);
        
        void on_volume_change();
        
        static OSStatus af_read_cb(void *user_data, SInt64 pos, UInt32	req_cnt, void *buffer, UInt32 *actual_cnt)
        {
            return static_cast<sfx_obj<D>*>(user_data)->af_read_callback(pos, req_cnt, buffer, actual_cnt);
        }
        
        static OSStatus af_write_cb(void *user_data, SInt64 pos, UInt32 req_cnt, const void *buffer, UInt32 *actual_cnt)
        {
            return static_cast<sfx_obj<D>*>(user_data)->af_write_callback(pos, req_cnt, buffer, actual_cnt);
        }
        
        static SInt64 af_get_size_cb(void *user_data)
        {
            return static_cast<sfx_obj<D>*>(user_data)->af_get_size_callback(); 
        }
        
        static OSStatus af_set_size_cb(void *user_data, SInt64 sz)
        {
            return static_cast<sfx_obj<D>*>(user_data)->af_set_size_callback(sz);
        }
        
        OSStatus af_read_callback(SInt64 pos, UInt32	req_cnt, void *buffer, UInt32 *actual_cnt);
        OSStatus af_write_callback(SInt64 pos, UInt32 req_cnt, const void *buffer, UInt32 *actual_cnt);
        SInt64 af_get_size_callback();
        OSStatus af_set_size_callback(SInt64 sz);
        
    private:
        ALuint	buffers_[NUM_BUFFERS];	
        ALuint	source_;	
        ALenum	format_;		
        ALsizei freq_;        
        
        boost::shared_ptr<ifdstream>    ifd_;
        AudioFileID						audio_file_;
        AudioStreamBasicDescription		data_format_;

        UInt32							num_packets_to_read_;
        UInt32                          max_packet_size_;
        
        float                           volume_;
    };
    
} // namespace snd
} // namespace x2d
using namespace x2d::snd_driver;
using namespace x2d::snd;

#endif // __X2D_OPENAL_FX_H__
