//
//  sound.cpp
//  x2d
//
//  Created by Alex Kremer on 2/12/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "sound.h"
#include "math_util.h"
#include "exceptions.h"
#include "log.h"

namespace x2d {
namespace snd {
    
    void sound_engine_bare::play( const boost::shared_ptr<music>& m )
    {
        
    }
    
    void sound_engine_bare::play( const boost::shared_ptr<sfx>& m )
    {
        
    }
    
    void sound_engine_bare::play(const std::string& file_path)
    {
        LOG("Requested to play file '%s'", file_path.c_str());        
        bg_music_ = boost::shared_ptr< music_obj<X2D_SND_DRIVER> >( 
                        new music_obj<X2D_SND_DRIVER>( file_path, true ) );
        
        bg_music_->play();
        LOG("Should be playing music now..");
    }

    void sound_engine_bare::play(resource_manager& rm, const std::string& path)
    {
        try 
        {
            LOG("Requested to play liverpool resource '%s'", path.c_str());        
            bg_music_ = boost::shared_ptr< music_obj<X2D_SND_DRIVER> >( 
                        new music_obj<X2D_SND_DRIVER>( rm, path, true ) );
        
            bg_music_->play();
            LOG("Should be playing music now..");
        }
        catch(sys_exception& e)
        {
            LOG("Caught exception: %s", e.what());
        }
    }

    boost::shared_ptr<sfx_obj<X2D_SND_DRIVER> > 
    sound_engine_bare::get_sfx(resource_manager& rm, const std::string& path)
    {
        try 
        {
            LOG("Requested to play liverpool SFX resource '%s'", path.c_str());        
            return boost::shared_ptr< sfx_obj<X2D_SND_DRIVER> >( 
                    new sfx_obj<X2D_SND_DRIVER>( rm, path ) );
        }
        catch(sys_exception& e)
        {
            LOG("Caught exception: %s", e.what());
        }
        
        return boost::shared_ptr< sfx_obj<X2D_SND_DRIVER> >(); // Empty
    }

    
    
    void sound_engine_bare::stop()
    {
        bg_music_->stop();
    }
    
    void sound_engine_bare::pause()
    {
        
    }
    
    void sound_engine_bare::resume()
    {
        
    }
    
    void sound_engine_bare::reset()
    {
        
    }
    
    void sound_engine_bare::master_volume(float v)
    {
        driver_.master_volume(v);
    }
    
    void sound_engine_bare::music_volume(float v)
    {
        driver_.music_volume(v);
    }
    
    void sound_engine_bare::sfx_volume(float v)
    {
        driver_.sfx_volume(v);
    }

    const float sound_engine_bare::master_volume() const
    {
        return driver_.master_volume();
    }
    
    const float sound_engine_bare::music_volume() const
    {
        return driver_.music_volume();
    }
    
    const float sound_engine_bare::sfx_volume() const
    {
        return driver_.sfx_volume();
    }

    
} // namespace snd
} // namespace x2d
