//
//  scene.cpp
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "scene.h"

bool scene::inp1_began(const glm::vec2& location)
{
//    LOG("++++++++++++++++ INPUT HIT IN BUTTON1");
    but1_->set_bgcolor(color_info(1,0,0,1));
    return true;
}

bool scene::inp2_began(const glm::vec2& location)
{
//    LOG("++++++++++++++++ INPUT HIT IN BUTTON2");
    but2_->set_bgcolor(color_info(0,1,0,1));
    return true;
}

bool scene::inp3_began(const glm::vec2& location)
{
    but3_->set_bgcolor(color_info(0,0,1,1));
//    LOG("++++++++++++++++ INPUT HIT IN BUTTON3");
    return true;
}

bool scene::inp1_ended(const glm::vec2& location)
{
    but1_->set_bgcolor(color_info(1,0,0,0.2));
//    LOG("++++++++++++++++ INPUT HIT IN BUTTON1");
    return true;
}

bool scene::inp2_ended(const glm::vec2& location)
{
    but2_->set_bgcolor(color_info(0,1,0,0.2));
//    LOG("++++++++++++++++ INPUT HIT IN BUTTON2");
    return true;
}

bool scene::inp3_ended(const glm::vec2& location)
{
    but3_->set_bgcolor(color_info(0,0,1,0.2));
//    LOG("++++++++++++++++ INPUT HIT IN BUTTON3");
    return true;
}