//
//  audio_queue.h
//  x2d
//
//  Created by Alex Kremer on 2/13/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_AUDIO_QUEUE_H__
#define __X2D_AUDIO_QUEUE_H__

#include <boost/shared_ptr.hpp>
#include "resource_manager.h"
#include "sound_resource.h"

#include <AudioToolbox/AudioQueue.h>
#include <AudioToolbox/AudioFile.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define NUM_BUFFERS 3
#define BUFFER_SIZE_BYTES 131072     // 128 KB buffers


namespace x2d {
namespace snd_driver {

    class audio_queue_driver;
    
} // namespace snd_driver
namespace snd {
    
    template<typename D>
    class music_obj
    {
        friend class snd_driver::audio_queue_driver;
        
    public:
        /**
         * Open AudioQueue from a file on the fs
         */
        music_obj(const std::string& file_path, bool loop=false);
        
        /**
         * Open AudioQueue from resource inside liverpool fs
         */
        music_obj(resource_manager& rm, const std::string& path, bool loop=false);
        
        ~music_obj();
        
        void play();
        void stop();
        void pause();
        void resume();
        void reset();
        bool is_playing() const;
        
    private:
        
        static void buffer_cb(   void *user_data, AudioQueueRef aq, AudioQueueBufferRef buf)
        {
            static_cast<music_obj<D>*>(user_data)->buffer_callback(buf);        
        }
        
        static void playback_cb( void *user_data, AudioQueueRef aq, AudioQueuePropertyID property_id)
        {
            
        }
        
        static OSStatus af_read_cb(void *user_data, SInt64 pos, UInt32	req_cnt, void *buffer, UInt32 *actual_cnt)
        {
            return static_cast<music_obj<D>*>(user_data)->af_read_callback(pos, req_cnt, buffer, actual_cnt);
        }
        
        static OSStatus af_write_cb(void *user_data, SInt64 pos, UInt32 req_cnt, const void *buffer, UInt32 *actual_cnt)
        {
            return static_cast<music_obj<D>*>(user_data)->af_write_callback(pos, req_cnt, buffer, actual_cnt);
        }

        static SInt64 af_get_size_cb(void *user_data)
        {
            return static_cast<music_obj<D>*>(user_data)->af_get_size_callback(); 
        }
        
        static OSStatus af_set_size_cb(void *user_data, SInt64 sz)
        {
            return static_cast<music_obj<D>*>(user_data)->af_set_size_callback(sz);
        }
        
        void buffer_callback(AudioQueueBufferRef buffer);        
        OSStatus af_read_callback(SInt64 pos, UInt32	req_cnt, void *buffer, UInt32 *actual_cnt);
        OSStatus af_write_callback(SInt64 pos, UInt32 req_cnt, const void *buffer, UInt32 *actual_cnt);
        SInt64 af_get_size_callback();
        OSStatus af_set_size_callback(SInt64 sz);
        
        UInt32 read_packets(AudioQueueBufferRef buffer);
        void prime();
        
        AudioFileID						audio_file_;
        AudioStreamBasicDescription		data_format_;
        AudioQueueRef					queue_;
        
        UInt64							packet_index_;
        UInt64							start_packet_index_;
        UInt64							stop_packet_index_;
        
        UInt32							num_packets_to_read_;
        UInt32                          max_packet_size_;
        AudioStreamPacketDescription	*packet_descriptions_;
        AudioQueueBufferRef				buffers_[NUM_BUFFERS];	
        
        boost::shared_ptr<ifdstream>    ifd_;
        
        float							volume_;
        bool                            loop_;
    };
        
} // namespace snd
    
namespace snd_driver {
           
    class audio_queue_driver
    {
    public:
        audio_queue_driver();
        ~audio_queue_driver();
        
        void master_volume(float v);
        void music_volume(float v);
        void sfx_volume(float v);
        
        const float master_volume() const;
        const float music_volume() const;
        const float sfx_volume() const;
        
    private:
        ALCdevice	*device_;
        ALCcontext  *context_;
        
        float master_volume_;
        float music_volume_;
        float sfx_volume_;        
    };
    
} // namespace snd_driver
} // namespace x2d
using namespace x2d::snd_driver;

#endif // __X2D_AUDIO_QUEUE_H__
