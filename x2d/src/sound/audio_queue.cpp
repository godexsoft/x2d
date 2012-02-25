//
//  audio_queue.cpp
//  x2d
//
//  Created by Alex Kremer on 2/13/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "audio_queue.h"
#include "glm.hpp"
#include "exceptions.h"
#include "sound.h"
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
    void music_obj<audio_queue_driver>::volume(float v)
    {
        volume_ = glm::clamp(v, 0.0f, 1.0f);
        AudioQueueSetParameter(queue_, kAudioQueueParam_Volume, volume_);        
    }
    
    template<>
    const float music_obj<audio_queue_driver>::volume() const
    {
        return volume_;
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
    void music_obj<audio_queue_driver>::calculate_seek(float start, float end)
    {
        bool want_start = start != 0.0f;
        bool want_end   = end != 0.0f;
   
        total_time_ = 0.0f;
        
        // variable bitrate requires calculations per packet
        if (data_format_.mBytesPerPacket == 0 || data_format_.mFramesPerPacket == 0) 
        {		
            UInt32 num_packets = num_packets_to_read_;
            UInt32 num_bytes;
            
            void* dummy_data = malloc(max_packet_size_*num_packets);
            
            // read all file
            while(true) 
            {
                AudioFileReadPackets(audio_file_, false, &num_bytes, packet_descriptions_, packet_index_, &num_packets, dummy_data);
                
                if (num_packets > 0) 
                {
                    for(int i=0; i<num_packets; ++i) {
                        
                        if(packet_descriptions_[i].mVariableFramesInPacket == 0) 
                        {
                            total_time_ += data_format_.mFramesPerPacket / data_format_.mSampleRate;
                        } 
                        else 
                        {
                            total_time_ += packet_descriptions_[i].mVariableFramesInPacket / data_format_.mSampleRate;
                        }
                        
                        if(want_start && total_time_ >= start)
                        {
                            start_packet_index_ = packet_index_;
                            want_start = false;
                        }
                        
                        if(want_end && total_time_ >= end)
                        {
                            stop_packet_index_ = packet_index_;
                            want_end = false;
                        }
                        
                        packet_index_ += 1;
                    }
                } 
                else 
                { 
                    break; // done
                }
            }
            
            free(dummy_data);            
        } 
        else 
        {
            UInt32 size = sizeof(UInt64);
            UInt64 total_packets;
            
            OSStatus err = AudioFileGetProperty(audio_file_, kAudioFilePropertyAudioDataPacketCount, &size, &total_packets);
            if(err != noErr)
            {
                throw sys_exception("audio_queue: Can't get total packet count from CBR audio file. " 
                        + boost::lexical_cast<std::string>(err));
            }

            float packet_time =  data_format_.mFramesPerPacket / data_format_.mSampleRate;
 
            if(want_start)
            {
                start_packet_index_ = start/packet_time;
            }
            
            if(want_end)
            {
                stop_packet_index_ = end/packet_time;
            }
            
            total_time_ = total_packets*packet_time;
        }
        
        LOG("Total length of track: %f", total_time_);
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
    music_obj<audio_queue_driver>::music_obj(const std::string& file_path, bool loop, float gain,
                                             float start, float end)
    : packet_index_(0)
    , start_packet_index_(0)
    , stop_packet_index_(0)
    , volume_(gain)
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
        
        calculate_seek(start, end);
        volume(volume_);
        prime();
    }
    
    template<>
    music_obj<audio_queue_driver>::music_obj(const boost::shared_ptr<ifdstream>& ifd, bool loop, float gain,
                                             float start, float end)
    : packet_index_(0)
    , start_packet_index_(0)
    , stop_packet_index_(0)
    , volume_(gain)
    , loop_(loop)
    , ifd_(ifd)
    {        
        LOG("Got ifdstream from path..");
        
        OSStatus res = AudioFileOpenWithCallbacks(this, &music_obj::af_read_cb, &music_obj::af_write_cb,
                            &music_obj::af_get_size_cb, &music_obj::af_set_size_cb, 
                                kAudioFileCAFType, &audio_file_);
        
        if(res)
        {
            throw sys_exception("audio_queue_driver: couldn't open audio file in liverpool fs. AudioFile returned " 
                                    + boost::lexical_cast<std::string>(res));
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
        
        calculate_seek(start, end);        
        volume(volume_);
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
    void music_obj<audio_queue_driver>::playback_callback()
    {
        // if the callback is requested
        if(on_playback_) 
        {
            if(! is_playing() ) 
            {
                // stopped
                on_playback_(false);
            } 
            else 
            {
                // started
                on_playback_(true);
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
    
    template<>
    void music_obj<audio_queue_driver>::on_volume_change()
    {
        AudioQueueSetParameter(queue_, kAudioQueueParam_Volume, 
            volume_ * sound_engine::instance().master_volume() * sound_engine::instance().music_volume());
    }

    
} // namespace snd
namespace snd_driver {

    audio_queue_driver::audio_queue_driver()
    : device_(NULL)
    , context_(NULL)
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
