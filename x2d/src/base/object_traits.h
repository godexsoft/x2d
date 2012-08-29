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
        
        void merge(const object_traits& other)
        {
            name = other.name;
            path = other.path;
            proto = other.proto;
            
            if(other.position.is_set())
            {
                position = other.position;
            }
            
            if(other.scale.is_set())
            {
                scale = other.scale;
            }

            if(other.rotation.is_set())
            {
                rotation = other.rotation;
            }

            if(other.box.is_set())
            {
                box = other.box;
            }

            if(other.pivot.is_set())
            {
                pivot = other.pivot;
            }
            
            if(other.has_bgcolor)
            {
                has_bgcolor = true;
                bgcolor = other.bgcolor;
            }
            
            if(other.lifetime.is_set())
            {
                lifetime = other.lifetime;
            }

            if(other.want_screen_touch_input.is_set())
            {
                want_screen_touch_input = other.want_screen_touch_input;
            }
            
            if(other.want_world_touch_input.is_set())
            {
                want_world_touch_input = other.want_world_touch_input;
            }
            
            if(other.want_accelerometer_input.is_set())
            {
                want_accelerometer_input = other.want_accelerometer_input;
            }
            
            if(other.visible.is_set())
            {
                visible = other.visible;
            }
            
            if(other.has_animation)
            {
                has_animation = true;
                animation = other.animation;
            }
            
            if(other.has_sprite)
            {
                has_sprite = true;
                sprite = other.sprite;
            }

            if(other.has_spawner)
            {
                has_spawner = true;
                spawner = other.spawner;
            }

            if(other.has_zone)
            {
                has_zone = true;
                zone = other.zone;
            }
            
            if(other.has_text)
            {
                has_text = true;
                text = other.text;
                font = other.font.empty()?font:other.font;
                align = other.align; // TODO: fix this
            }

            if(other.has_body)
            {
                has_body = true;
                body = other.body;
            }
            
            if(!other.camera.empty())
            {
                camera = other.camera;
            }

            // TODO: fix this
//            obj_space =
//            parent_space    par_space;
            
            if(other.has_parent)
            {
                has_parent = true;
            }

            // FIXME: not really good
            if(!other.children.empty())
            {
                children = other.children;
            }
            
            // FIXME: not really good
            if(!other.contexts.empty())
            {
                contexts = other.contexts;
            }
            
            if(!other.on_create.empty())
            {
                on_create = other.on_create;
            }

            if(!other.on_destroy.empty())
            {
                on_destroy = other.on_destroy;
            }
        }
        
        // basic object properties
        std::string              name;
        basic_path<char, '.'>    path;
        basic_path<char, '.'>    proto;
        value_holder<glm::vec3>  position;
        value_holder<float>      scale;
        value_holder<float>      rotation;
        value_holder<size>       box;
        value_holder<glm::vec2>  pivot;
        
        bool                     has_bgcolor;
        color_info               bgcolor;
        
        value_holder<float>      lifetime;
        
        // input methods requested by object
        value_holder<bool>       want_screen_touch_input;
        value_holder<bool>       want_world_touch_input;
        value_holder<bool>       want_accelerometer_input;
        
        value_holder<bool>       visible;
        
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
        std::string on_create;
        std::string on_destroy;
    };

} // namespace x2d
using namespace x2d;

#endif // __X2D_OBJECT_TRAITS_H__
