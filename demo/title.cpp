//
//  title.cpp
//  x2d
//
//  Created by Alex Kremer on 4/25/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//
//  HAPPY BIRTHDAY, VALERON!!
//

#include "title.h"
#include "game.h"

title::title(kernel& k, configuration& conf, game& g)
: base_object(k)
, config_(conf)
, game_(g)
{       
    connect_touch_input(WORLD_SPACE);
    objects_.push_back( config_.create_object("objects.title") );
}

void title::touch_input_began(space s, const std::vector<touch>& touches) 
{
    // progress to game
    game_.start_game();
}