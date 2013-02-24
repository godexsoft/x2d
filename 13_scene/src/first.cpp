//
//  first.cpp
//  x2d
//
//  Created by Alex Kremer on 2/24/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#include "first.h"
#include "second.h"

void first_scene::on_transition_to()
{
}

void first_scene::on_transition_from()
{
    set_lifetime(0.3f);
}

void first_scene::touch_input_ended(space s, const std::vector<touch>& touches)
{
    // TODO: disconnect input right here. Otherwise the object
    // can receive more input even though it is in the transition..

    get_app().switch_to( get_config().create_scene<second_scene>("scenes.second") );
}

void first_scene::update(const clock_info& clock)
{
    if(!mover_.is_completed())
    {
        mover_.update(clock.delta_time);
        bg_->camera_space_position(glm::vec2(x_offset_, 0.5f));
    }
}
