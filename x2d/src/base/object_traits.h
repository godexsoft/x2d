//
//  object_traits.h
//  x2d
//
//  Created by Alex Kremer on 01/03/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_OBJECT_TRAITS_H__
#define __X2D_OBJECT_TRAITS_H__

#include "value_holder.h"
#include "math_util.h"
#include "glm.hpp"
#include "filesystem.h"

#include <vector>
#include <string>

namespace x2d {

    /**
     * @brief Object traits
     */
    struct object_traits
    {
        object_traits();
        
        // basic object properties
        std::string              name;
        basic_path<char, '.'>    path;
        value_holder<glm::vec3>  position;
        value_holder<float>      scale;
        value_holder<float>      rotation;
        size                     box;
        glm::vec2                pivot;
        
        bool                     has_bgcolor;
        color_info               bgcolor;
        
        value_holder<float>      lifetime;
        
        // input methods requested by object
        bool        want_screen_touch_input;
        bool        want_world_touch_input;
        bool        want_accelerometer_input;
        
        bool        visible;
        
        // graphics
        bool        has_animation;
        std::string animation;
        
        bool        has_sprite;
        std::string sprite;
        
        bool        has_spawner;
        std::string spawner;

        bool        has_zone;
        std::string zone;
        
        // text and font        
        std::string text;
        bool        has_text;
        std::string font;
        alignment   align;
        
        // physics
        bool        has_body;
        std::string body;
        
        std::string camera;
        
        // space
        space           obj_space;        
        parent_space    par_space;
        
        bool            has_parent;
        std::vector<std::string>    children;
        
        // context list
        std::vector<std::string>    contexts;
        
        // scripts
        bool has_on_create;
        bool has_on_destroy;
    };

} // namespace x2d
using namespace x2d;

#endif // __X2D_OBJECT_TRAITS_H__
