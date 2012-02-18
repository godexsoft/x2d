//
//  sound_resource.cpp
//  x2d
//
//  Created by Alex Kremer on 2/18/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "sound_resource.h"
#include "sound.h"

namespace x2d {
namespace snd {
     
    sound::sound()
    {
        sound_engine::instance().reg(this);
    }
    
    sound::~sound()
    {
        sound_engine::instance().unreg(this);
    }
    
    void sfx::play()
    {
        obj_->reset();
        obj_->pitch(pitch_.get(conf_));
        obj_->play();
    }     
    
} // namespace snd
} // namespace x2d   
