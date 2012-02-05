//
//  game.h
//  x2d
//
//  Created by Alex Kremer on 2/2/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_GAME_H__
#define __X2D_GAME_H__

#include "kernel.h"
#include "liverpool_manager.h"
#include "scene.h"
#include "configuration.h"
#include "resource_manager.h"

class game 
{
public:
    game(liverpool_manager& lvp_man, kernel& k)    
    : rm_(lvp_man)
    , k_(k)
    , config_(k_, rm_, "res/main.xml")
    , scene_(k_, config_)
    {
    }
    
    inline void step()
    {
        k_.step();
    }

    inline void pause()
    {
        k_.pause();
    }

    inline void resume()
    {
        k_.resume();
    }
    
private:
    resource_manager    rm_;
    x2d::kernel&        k_;

    configuration       config_;
    scene               scene_;
};

#endif // __X2D_GAME_H__
