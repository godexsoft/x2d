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
#include <boost/function.hpp>

#include <AudioToolbox/AudioQueue.h>
#include <AudioToolbox/AudioFile.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "filesystem.h"

#define NUM_BUFFERS 4 // maximum 4 buffers
#define BUFFER_SIZE_BYTES 1048576 // 1 MB buffers


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
        music_obj(const std::string& file_path, bool loop=false, float gain=1.0f, 
                  float start=0.0f, float end=0.0f);
        
        /**
         * Open AudioQueue from resource inside liverpool fs
         */
        music_obj(const boost::shared_ptr<ifdstream>& ifd, bool loop=false, float gain=1.0f,
                  float start=0.0f, float end=0.0f);
                
        ~music_obj();
        
        void set_callback(const boost::function<void(bool)>& cb)
        {
            on_playback_ = cb;
        }
        
        const float total_time() const
        {
            return total_time_;
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
        
    private:
        
        void calculate_seek(float start, float end);
        
        static void buffer_cb(   void *user_data, AudioQueueRef aq, AudioQueueBufferRef buf)
        {
            static_cast<music_obj<D>*>(user_data)->buffer_callback(buf);        
        }
        
        static void playback_cb( void *user_data, AudioQueueRef aq, AudioQueuePropertyID property_id)
        {
            static_cast<music_obj<D>*>(user_data)->playback_callback();
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
        void playback_callback();
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
        float                           total_time_;
        
        UInt32							num_packets_to_read_;
        UInt32                          max_packet_size_;
        AudioStreamPacketDescription	*packet_descriptions_;
        AudioQueueBufferRef				buffers_[NUM_BUFFERS];	
        
        boost::shared_ptr<ifdstream>    ifd_;
        
        float							volume_;
        bool                            loop_;
        bool                            is_paused_;
        
        boost::function<void(bool)>     on_playback_;
    };
        
} // namespace snd
    
namespace snd_driver {
           
    class audio_queue_driver
    {
    public:
        audio_queue_driver();
        ~audio_queue_driver();
                
    private:
        ALCdevice	*device_;
        ALCcontext  *context_;
    };
    
} // namespace snd_driver
} // namespace x2d
using namespace x2d::snd_driver;

#endif // __X2D_AUDIO_QUEUE_H__
