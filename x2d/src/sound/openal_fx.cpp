//
//  openal_fx.cpp
//  x2d
//
//  Created by Alex Kremer on 2/16/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "openal_fx.h"
#include "exceptions.h"
#include <boost/lexical_cast.hpp>

namespace x2d {
namespace snd {
        
    template<>
    sfx_obj<audio_queue_driver>::sfx_obj(resource_manager& rm, const std::string& path)    
    {
        // get the ifdstream
        ifd_ = rm.get<ifdstream>(path);
        LOG("Got ifdstream from path..");
        
        OSStatus res = AudioFileOpenWithCallbacks(this, &sfx_obj::af_read_cb, &sfx_obj::af_write_cb,
                                                  &sfx_obj::af_get_size_cb, &sfx_obj::af_set_size_cb, 
                                                  kAudioFileCAFType, &audio_file_);
        
        if(res)
        {
            throw sys_exception("audio_queue_driver: couldn't open SFX file in liverpool fs at '" + path 
                                + "'. AudioFileOpen returned " + boost::lexical_cast<std::string>(res));
        }
        
        UInt32 size = sizeof(data_format_);
        AudioFileGetProperty(audio_file_, kAudioFilePropertyDataFormat, &size, &data_format_);
        
        if (res != 0) 
        {
            throw sys_exception("audio_queue_driver: couldn't load SFX file from liverpool path '" + path 
                                + "'. AudioFileReadBytes returned " + boost::lexical_cast<std::string>(res));
        }
        
        alGenBuffers(NUM_BUFFERS, buffers_);
        alGenSources(1, &source_); 
        
        alSource3f(source_, AL_POSITION, 0.0, 0.0, 0.0);
        alSource3f(source_, AL_VELOCITY, 0.0, 0.0, 0.0);
        alSource3f(source_, AL_DIRECTION, 0.0, 0.0, 0.0);
        
        alSourcef(source_, AL_PITCH, 1.0f);
        alSourcef(source_, AL_GAIN, 1.0f);
        
        alSourcei(source_, AL_LOOPING, AL_FALSE);	
        alSourcef(source_, AL_ROLLOFF_FACTOR, 0.0);
        alSourcei(source_, AL_SOURCE_RELATIVE, AL_TRUE);        
        
        if (data_format_.mBytesPerPacket == 0 || data_format_.mFramesPerPacket == 0)
        {
            LOG("Variable bitrate SFX");
            size = sizeof(max_packet_size_);
            AudioFileGetProperty(audio_file_, kAudioFilePropertyPacketSizeUpperBound, &size, &max_packet_size_);
            if (max_packet_size_ > BUFFER_SIZE_BYTES)
            {
                max_packet_size_ = BUFFER_SIZE_BYTES;
            }
            
            num_packets_to_read_ = BUFFER_SIZE_BYTES / max_packet_size_;
        }
        else
        {
            LOG("Constant bitrate SFX");
            num_packets_to_read_ = BUFFER_SIZE_BYTES / data_format_.mBytesPerPacket;
        }
        
        UInt32 offset = 0;
        
        std::vector<unsigned char> dat;
        
        for( int i=0; i<NUM_BUFFERS; ++i)
        {
            UInt32 packets = num_packets_to_read_;
            dat.resize(BUFFER_SIZE_BYTES);        
            res = AudioFileReadPackets (audio_file_, false, &size, NULL, offset, &packets, &dat.at(0));            
   
            if(res != noErr)
            {
                throw sys_exception("AudioFileReadPackets returned error: " + boost::lexical_cast<std::string>(res));
            }
            
            if (size == 0) 
            {
                LOG("All buffered into %d buffers total.", i+1);
                break; // done
            }
            
            dat.resize(size);
            LOG("Read %d bytes", size);
        
            LOG("Buffering data.. size is %d", dat.size());
            alBufferData(buffers_[i], AL_FORMAT_STEREO16, &dat.at(0), dat.size(), 44100);              
            alSourcei(source_, AL_BUFFER, buffers_[i]);
            
            offset += packets;
        }
        
        AudioFileClose(audio_file_);
    }
    
    template<>
    sfx_obj<audio_queue_driver>::~sfx_obj()
    {
        alSourceStop(source_);
        
        alDeleteSources(1, &source_);
        alDeleteBuffers(NUM_BUFFERS, buffers_);
    }
    
    
    template<>
    OSStatus sfx_obj<audio_queue_driver>::af_read_callback(SInt64 pos, UInt32	req_cnt, void *buffer, UInt32 *actual_cnt)
    {
        ifd_->seekg(pos, std::ios_base::beg);
        *actual_cnt = ifd_->read((char*)buffer, req_cnt);
        
        return noErr;
    }
    
    template<>
    OSStatus sfx_obj<audio_queue_driver>::af_write_callback(SInt64 pos, UInt32 req_cnt, const void *buffer, UInt32 *actual_cnt)
    {
        // Should not be called. we are only reading.
        return noErr;
    }
    
    template<>
    SInt64 sfx_obj<audio_queue_driver>::af_get_size_callback()
    {
        return ifd_->size();
    }
    
    template<>
    OSStatus sfx_obj<audio_queue_driver>::af_set_size_callback(SInt64 sz)
    {
        // Should not be called.
        return noErr;
    }
    

    template<>
    void sfx_obj<audio_queue_driver>::play()
    {
        LOG("Play SFX..");
        alSourcePlay(source_);
    }
    
    template<>
    void sfx_obj<audio_queue_driver>::stop()
    {
        alSourceStop(source_);
    }
    
    template<>
    void sfx_obj<audio_queue_driver>::reset()
    {
        alSourceRewind(source_);
    }
    
} // namespace snd
} // namespace x2d