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

#if defined X2D_SND_DRIVER
    #include "audio_queue.h"
    #include "openal_fx.h"
#else
    #error Selected audio driver does not exist.
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
    {
    public:
        music(const boost::shared_ptr<ifdstream>& ifd, bool loop, float gain)        
        : obj_( new music_obj<X2D_SND_DRIVER>( ifd, loop, gain ) )
        {         
            LOG("Music resource volume level: %f", gain);
            LOG("Music resource loop: %s", loop?"yes":"no");
        }
        
        void play() 
        {
            obj_->play();
        }
        
        // updates
        void on_volume_change() { obj_->on_volume_change(); }
        
    private:
        boost::shared_ptr< music_obj<X2D_SND_DRIVER> > obj_;
    };
        
} // namespace snd
} // namespace x2d   
using namespace x2d::snd;

#endif // __X2D_SOUND_RESOURCE_H__
