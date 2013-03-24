//
//  game.h
//  x2d - UberDemo
//
//  Created by Alex Kremer on 3/24/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_GAME_H__
#define __X2D_GAME_H__

#include "app_framework.h"

class game
: public app_framework
{
public:
    void main()
    {
        // load the first scene
        switch_to( get_config().create_scene("scenes.first") );        
    }
};

#endif // __X2D_GAME_H__
