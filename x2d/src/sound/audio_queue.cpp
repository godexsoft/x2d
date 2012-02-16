//
//  audio_queue.cpp
//  x2d
//
//  Created by Alex Kremer on 2/13/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "audio_queue.h"
#include "math_util.h"
#include "exceptions.h"
#include "log.h"
#include <string>
#include <boost/lexical_cast.hpp>

namespace x2d {
namespace snd {
    
    template<>
    void music_obj<audio_queue_driver>::play()
    {
        LOG("Playback of audio_queue file.");
        AudioQueueStart(queue_, NULL);
    }
    
    template<>
    void music_obj<audio_queue_driver>::stop()
    {
        AudioQueueStop(queue_, true);
    }
    
    template<>
    bool music_obj<audio_queue_driver>::is_playing() const
    {
        UInt32 size, is_running;
        
        AudioQueueGetPropertySize(queue_, kAudioQueueProperty_IsRunning, &size);
        AudioQueueGetProperty(queue_, kAudioQueueProperty_IsRunning, &is_running, &size);
        
        return is_running;
    }
    
    template<>
    void music_obj<audio_queue_driver>::pause()
    {    	
        AudioQueuePause(queue_);    
    }
    
    template<>
    void music_obj<audio_queue_driver>::resume()
    {
        AudioQueueStart(queue_, NULL);
    }
    
    template<>
    void music_obj<audio_queue_driver>::reset()
    {
    }
    
    template<>
    UInt32 music_obj<audio_queue_driver>::read_packets(AudioQueueBufferRef buffer)
    {
        UInt32 num_bytes, num_packets;
        num_packets = num_packets_to_read_;
        
        AudioFileReadPackets(audio_file_, false, &num_bytes, packet_descriptions_, packet_index_, &num_packets, buffer->mAudioData);
        if (num_packets > 0)
        {
            buffer->mAudioDataByteSize = num_bytes;
            AudioQueueEnqueueBuffer(queue_, buffer, (packet_descriptions_ ? num_packets : 0), packet_descriptions_);
            packet_index_ += num_packets;
            
            if(stop_packet_index_ != 0 && packet_index_ >= stop_packet_index_) {
                if(loop_)
                    packet_index_ = start_packet_index_;
                else
                    return 0;
            }
        }

        return num_packets;
    }
    
    template<>
    void music_obj<audio_queue_driver>::prime()
    {
        UInt32 i;
        for (i = 0; i < NUM_BUFFERS; ++i) 
        {
            AudioQueueAllocateBuffer(queue_, BUFFER_SIZE_BYTES, &buffers_[i]);
            if (read_packets(buffers_[i]) == 0)
            {
                break;
            } 
        }
        
        UInt32 frames_primed;
        AudioQueuePrime(queue_, i, &frames_primed);
        
        LOG("Primed %d frames", frames_primed);
    }
    
    template<>
    music_obj<audio_queue_driver>::music_obj(const std::string& file_path, bool loop)
    : packet_index_(0)
    , start_packet_index_(0)
    , stop_packet_index_(0)
    , volume_(1.0f) // default is full gain
    , loop_(loop)
    {
        CFURLRef file_url = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *)file_path.c_str(), file_path.size(), false);
        OSStatus res = AudioFileOpenURL(file_url, kAudioFileReadPermission, kAudioFileCAFType, &audio_file_);
        CFRelease(file_url);
        
        if(res)
        {
            throw sys_exception("audio_queue_driver: couldn't open audio file at '" + file_path + "'");
        }
        
        UInt32 size = sizeof(data_format_);
        AudioFileGetProperty(audio_file_, kAudioFilePropertyDataFormat, &size, &data_format_);
        
        AudioQueueNewOutput(&data_format_, &music_obj<audio_queue_driver>::buffer_cb, this, NULL, NULL, 0, &queue_);        
        AudioQueueAddPropertyListener(queue_, kAudioQueueProperty_IsRunning, &music_obj<audio_queue_driver>::playback_cb, this);
        
        if (data_format_.mBytesPerPacket == 0 || data_format_.mFramesPerPacket == 0)
        {
            size = sizeof(max_packet_size_);
            AudioFileGetProperty(audio_file_, kAudioFilePropertyPacketSizeUpperBound, &size, &max_packet_size_);
            if (max_packet_size_ > BUFFER_SIZE_BYTES)
            {
                max_packet_size_ = BUFFER_SIZE_BYTES;
            }
            
            num_packets_to_read_ = BUFFER_SIZE_BYTES / max_packet_size_;
            packet_descriptions_ = (AudioStreamPacketDescription*)malloc(sizeof(AudioStreamPacketDescription) * num_packets_to_read_);
        }
        else
        {
            num_packets_to_read_ = BUFFER_SIZE_BYTES / data_format_.mBytesPerPacket;
            packet_descriptions_ = NULL;
        }
        
        AudioFileGetPropertyInfo(audio_file_, kAudioFilePropertyMagicCookieData, &size, NULL);
        if (size > 0)
        {
            char* cookie = (char*)malloc(sizeof(char) * size);
            AudioFileGetProperty(audio_file_, kAudioFilePropertyMagicCookieData, &size, cookie);
            AudioQueueSetProperty(queue_, kAudioQueueProperty_MagicCookie, cookie, size);
            free(cookie);
        }
        
        prime();
    }
    
    template<>
    music_obj<audio_queue_driver>::music_obj(resource_manager& rm, const std::string& path, bool loop)
    : packet_index_(0)
    , start_packet_index_(0)
    , stop_packet_index_(0)
    , volume_(1.0f) // default is full gain
    , loop_(loop)
    {
        // get the ifdstream
        ifd_ = rm.get<ifdstream>(path);
        
        LOG("Got ifdstream from path..");
        
        OSStatus res = AudioFileOpenWithCallbacks(this, &music_obj::af_read_cb, &music_obj::af_write_cb,
                            &music_obj::af_get_size_cb, &music_obj::af_set_size_cb, 
                                kAudioFileCAFType, &audio_file_);
        
        if(res)
        {
            throw sys_exception("audio_queue_driver: couldn't open audio file in liverpool fs at '" + path 
                                + "'. AudioFile returned " + boost::lexical_cast<std::string>(res));
        }
        
        UInt32 size = sizeof(data_format_);
        AudioFileGetProperty(audio_file_, kAudioFilePropertyDataFormat, &size, &data_format_);
        
        AudioQueueNewOutput(&data_format_, &music_obj<audio_queue_driver>::buffer_cb, this, NULL, NULL, 0, &queue_);        
        AudioQueueAddPropertyListener(queue_, kAudioQueueProperty_IsRunning, &music_obj<audio_queue_driver>::playback_cb, this);
        
        if (data_format_.mBytesPerPacket == 0 || data_format_.mFramesPerPacket == 0)
        {
            size = sizeof(max_packet_size_);
            AudioFileGetProperty(audio_file_, kAudioFilePropertyPacketSizeUpperBound, &size, &max_packet_size_);
            if (max_packet_size_ > BUFFER_SIZE_BYTES)
            {
                max_packet_size_ = BUFFER_SIZE_BYTES;
            }
            
            num_packets_to_read_ = BUFFER_SIZE_BYTES / max_packet_size_;
            packet_descriptions_ = (AudioStreamPacketDescription*)malloc(sizeof(AudioStreamPacketDescription) * num_packets_to_read_);
        }
        else
        {
            num_packets_to_read_ = BUFFER_SIZE_BYTES / data_format_.mBytesPerPacket;
            packet_descriptions_ = NULL;
        }
        
        AudioFileGetPropertyInfo(audio_file_, kAudioFilePropertyMagicCookieData, &size, NULL);
        if (size > 0)
        {
            char* cookie = (char*)malloc(sizeof(char) * size);
            AudioFileGetProperty(audio_file_, kAudioFilePropertyMagicCookieData, &size, cookie);
            AudioQueueSetProperty(queue_, kAudioQueueProperty_MagicCookie, cookie, size);
            free(cookie);
        }
        
        prime();   
    }
    
    template<>
    music_obj<audio_queue_driver>::~music_obj()
    {
        if (is_playing()) 
        {
            AudioQueueStop(queue_, true);
        }
        
        AudioQueueDispose(queue_, true);
		AudioFileClose(audio_file_);   
        
        if(packet_descriptions_ != NULL)
        {
            free(packet_descriptions_);
        }
    }
    
    
    template<>
    void music_obj<audio_queue_driver>::buffer_callback(AudioQueueBufferRef buffer)
    {
        if (read_packets(buffer) == 0)
        {
            if(loop_)
            {
                packet_index_ = start_packet_index_;
                read_packets(buffer);
            }
        }
    }
    
    template<>
    OSStatus music_obj<audio_queue_driver>::af_read_callback(SInt64 pos, UInt32	req_cnt, void *buffer, UInt32 *actual_cnt)
    {
        ifd_->seekg(pos, std::ios_base::beg);
        *actual_cnt = ifd_->read((char*)buffer, req_cnt);
        
        return noErr;
    }
    
    template<>
    OSStatus music_obj<audio_queue_driver>::af_write_callback(SInt64 pos, UInt32 req_cnt, const void *buffer, UInt32 *actual_cnt)
    {
        // Should not be called. we are only reading.
        return noErr;
    }
    
    template<>
    SInt64 music_obj<audio_queue_driver>::af_get_size_callback()
    {
        return ifd_->size();
    }

    template<>
    OSStatus music_obj<audio_queue_driver>::af_set_size_callback(SInt64 sz)
    {
        // Should not be called.
        return noErr;
    }
    
} // namespace snd
namespace snd_driver {

    void audio_queue_driver::master_volume(float v)
    {
        master_volume_ = clamp(v, 0.0f, 1.0f);
    }
    
    void audio_queue_driver::music_volume(float v)
    {
        music_volume_ = clamp(v, 0.0f, 1.0f);
    }
    
    void audio_queue_driver::sfx_volume(float v)
    {
        sfx_volume_ = clamp(v, 0.0f, 1.0f);
    }
    
    const float audio_queue_driver::master_volume() const
    {
        return master_volume_;
    }
    
    const float audio_queue_driver::music_volume() const
    {
        return music_volume_;
    }
    
    const float audio_queue_driver::sfx_volume() const
    {
        return sfx_volume_;
    }
    
    
    audio_queue_driver::audio_queue_driver()
    : device_(NULL)
    , context_(NULL)
    , master_volume_(1.0f)
    , music_volume_(1.0f)
    , sfx_volume_(1.0f)
    {
        LOG("Sound engine initializing..");
        
        device_ = alcOpenDevice(NULL);
        
        if (device_) 
        {
            context_ = alcCreateContext(device_, NULL);
            alcMakeContextCurrent(context_);
            
            LOG("Got OpenAL context.");
        }
        else
        {
            throw sys_exception("OpenAL: failed to create device.");
        }
        
        LOG("Sound engine started.");
    }
    
    audio_queue_driver::~audio_queue_driver()
    {
        LOG("Sound engine shutting down..");
        
        alcDestroyContext(context_);
        alcCloseDevice(device_);
        
        LOG("Sound engine stopped.");
    }
    
} // namespace snd_driver
} // namespace x2d
