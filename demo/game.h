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
#include "title.h"

#include "configuration.h"
#include "resource_manager.h"

#include "player.h"

class game 
{
public:
    game(liverpool_manager& lvp_man, kernel& k)    
    : rm_(lvp_man)
    , k_(k)
    , config_(k_, rm_, "res/main.xml")
    {
        // bindings
        config_.bind<player>("objects.player");    
        
        // init first scene
        cur_scene_ = boost::shared_ptr<base_object>( new title(k_, config_, *this) );
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
    
    void start_game()
    {        
        cur_scene_ = boost::shared_ptr<base_object>( new scene(k_, config_, *this) );   
    }
    
private:
    resource_manager    rm_;
    x2d::kernel&        k_;

    configuration                    config_;
    boost::shared_ptr<base_object>   cur_scene_;
};

#endif // __X2D_GAME_H__
