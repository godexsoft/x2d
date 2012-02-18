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
     * @brief Sound effect resource
     */
    class sfx
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
        
    private:
        configuration&                               conf_;
        boost::shared_ptr< sfx_obj<X2D_SND_DRIVER> > obj_;
        value_holder<float>                          pitch_;
    };
    
    /**
     * @brief Background music resource
     */
    class music
    {
    public:
        music(const boost::shared_ptr<ifdstream>& ifd, bool loop, float gain)        
        : obj_( new music_obj<X2D_SND_DRIVER>( ifd, loop, gain ) )
        {         
            LOG("Music resource volume level: %f", gain);
        }
        
        void play() 
        {
            obj_->play();
        }
        
    private:
        boost::shared_ptr< music_obj<X2D_SND_DRIVER> > obj_;
    };
        
} // namespace snd
} // namespace x2d   
using namespace x2d::snd;

#endif // __X2D_SOUND_RESOURCE_H__
