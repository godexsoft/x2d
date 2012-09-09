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

// #include "x2d.h"
#include "app_framework.h"
#include "scene.h"

class game : public app_framework
{
public:
    void main()
    {
        LOG("Run user defined main method...");
        cur_scene_ = boost::shared_ptr<scene>( new scene(*this) );
    }
    
private:
    boost::shared_ptr<scene>   cur_scene_;
};

#endif // __X2D_GAME_H__
