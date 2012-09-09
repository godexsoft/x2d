//
//  sound_resource.h
//  x2d
//
//  Created by Alex Kremer on 2/13/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __X2D_SOUND_RESOURCE_H__
#define __X2D_SOUND_RESOURCE_H__

#include <boost/shared_ptr.hpp>
#include "config.h"
#include "value_holder.h"
#include "base_object.h"
#include "interpolator.h"
#include "sound.h"

#if defined X2D_SND_DRIVER
#	if defined X2D_NULL_SOUND
#		include "null_drivers.h"
#	elif defined X2D_IOS_SOUND
#		include "audio_queue.h"
#		include "openal_fx.h"
#	endif
#else
#	error Selected audio driver does not exist.
#endif

namespace x2d {
namespace snd {
    
    /**
     * @brief Base class for both music and sfx
     */
    struct sound
    {    
        sound();
        virtual ~sound();        
        
        // update from the engine
        virtual void on_volume_change() = 0;
    };
    
    /**
     * @brief Sound effect resource
     */
    class sfx
    : public sound
    {
    public:
        sfx(configuration& conf, const boost::shared_ptr<ifdstream>& ifd, 
            bool loop, const value_holder<float>& pitch)
        : conf_(conf)
        , obj_( new sfx_obj<X2D_SND_DRIVER>( ifd, loop ) )
        , pitch_(pitch)
        {            
        }
        
        void play();
        void stop();
        
        // updates
        void on_volume_change() { obj_->on_volume_change(); }
        
    private:
        configuration&                               conf_;
        boost::shared_ptr< sfx_obj<X2D_SND_DRIVER> > obj_;
        value_holder<float>                          pitch_;
    };
    
    /**
     * @brief Background music resource
     */
    class music
    : public sound
    , public base_object
    {
    public:
        music(kernel& k, const boost::shared_ptr<ifdstream>& ifd, 
              bool loop, float gain, float fi, float fo, 
              float start, float end)        
        : base_object(k)
        , obj_( new music_obj<X2D_SND_DRIVER>( ifd, loop, fi!=0.0f?0.0f:gain, start, end ) )
        , fade_in_(fi)
        , fade_out_(fo)
        , fade_out_start_(end>0.0f?end-fo:obj_->total_time()-fo)
        , volume_level_(0.0f)
        , want_volume_level_(gain)
        , start_(0.0)
        , total_len_((end>0.0f?end:obj_->total_time())-start)
        , loop_(loop)
        {      
            if(fi != 0.0f || fo != 0.0f)
            {
                // fade requires updates from the kernel
                connect_update();
                
                // and connect the callback
                obj_->set_callback( boost::bind(&music::on_playback, this, _1));
            }
        }
        
        void play();
        
        // updates
        void on_volume_change() { obj_->on_volume_change(); }
        
        // base_object stuff
        void update(const clock_info& ci);
        
    private:
        boost::shared_ptr< music_obj<X2D_SND_DRIVER> > obj_;
        
        // callback for lower-level
        void on_playback(bool playing);
        
        float fade_in_;
        float fade_out_;        // length
        float fade_out_start_;  // time
        float volume_level_;
        float want_volume_level_;
        
        double start_;
        float total_len_;
        bool loop_;
    };
        
} // namespace snd
} // namespace x2d   
using namespace x2d::snd;

#endif // __X2D_SOUND_RESOURCE_H__
