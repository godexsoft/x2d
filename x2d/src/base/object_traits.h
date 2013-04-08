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

#include "value.h"
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
            
            if(other.position)
            {
                position.set(other.position);
            }
            
            if(other.scale)
            {
                scale.set(other.scale);
            }

            if(other.rotation)
            {
                rotation.set(other.rotation);
            }

            if(other.box)
            {
                box.set(other.box);
            }

            if(other.pivot)
            {
                pivot.set(other.pivot);
            }
            
            if(other.has_bgcolor)
            {
                has_bgcolor = true;
                bgcolor = other.bgcolor;
            }
            
            if(other.lifetime)
            {
                lifetime.set(other.lifetime);
            }

            if(other.want_screen_touch_input)
            {
                want_screen_touch_input.set(other.want_screen_touch_input);
            }
            
            if(other.want_world_touch_input)
            {
                want_world_touch_input.set(other.want_world_touch_input);
            }
            
            if(other.want_accelerometer_input)
            {
                want_accelerometer_input.set(other.want_accelerometer_input);
            }
            
            if(other.visible)
            {
                visible.set(other.visible);
            }

            if(other.enabled)
            {
                enabled.set(other.enabled);
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
            
            if(other.has_emitter)
            {
                has_emitter = true;
                emitter = other.emitter;
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

            if(!other.children.empty())
            {
                if(children.empty())
                {
                    // just copy
                    children = other.children;
                }
                else
                {
                    std::vector<std::string> merged_children;
                    std::vector<std::string> other_copy = other.children;

                    // merge
                    for(std::vector<std::string>::iterator it = children.begin(); it != children.end();)
                    {
                        basic_path<char, '.'> kthis(*it);
                        std::string to_merge = *it; // this will be the default

                        for(std::vector<std::string>::iterator jit = other_copy.begin(); jit != other_copy.end();)
                        {
                            // the rule is simple. we can't have more than one child with the same key
                            // so if the last path component is same - override from the 'other' child.
                            basic_path<char, '.'> kother(*jit);

                            if(kthis.last_path_component() == kother.last_path_component())
                            {
                                to_merge = *jit; // merge other child path
                                other_copy.erase(jit); // remove it because we already used this one now
                                break; // go on to the next key
                            }

                            ++jit;
                        }

                        it = children.erase(it); // remove this one as we already handled it
                        merged_children.push_back(to_merge);
                    }

                    // Now all left in other children is just something that was not found in the prototype children
                    // so just merge that in as well.
                    for(std::vector<std::string>::iterator jit = other_copy.begin(); jit != other_copy.end(); ++jit)
                    {
                        merged_children.push_back(*jit);
                    }

                    children = merged_children;
                }
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

            if(!other.on_input_began.empty())
            {
                on_input_began = other.on_input_began;
            }
            
            if(!other.on_input_moved.empty())
            {
                on_input_moved = other.on_input_moved;
            }
            
            if(!other.on_input_ended.empty())
            {
                on_input_ended = other.on_input_ended;
            }
            
            if(!other.on_transition_from.empty())
            {
                on_transition_from = other.on_transition_from;
            }
            
            if(!other.on_transition_to.empty())
            {
                on_transition_to = other.on_transition_to;
            }
        }
        
        // basic object properties
        std::string              name;
        basic_path<char, '.'>    path;
        basic_path<char, '.'>    proto;
        
        optional_value<glm::vec3>  position;
        optional_value<float>      scale;
        optional_value<float>      rotation;
        optional_value<size>       box;
        optional_value<glm::vec2>  pivot;
        
        bool              has_bgcolor;
        color_info        bgcolor;
        
        optional_value<float>      lifetime;
        
        // input methods requested by object
        optional_value<bool>       want_screen_touch_input;
        optional_value<bool>       want_world_touch_input;
        optional_value<bool>       want_accelerometer_input;

        optional_value<bool>       enabled;
        optional_value<bool>       visible;

        // graphics
        bool        has_animation;
        std::string animation;
        
        bool        has_sprite;
        std::string sprite;
        
        bool        has_spawner;
        std::string spawner;
        
        bool        has_emitter;
        std::string emitter;

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
        std::string on_input_began;
        std::string on_input_moved;
        std::string on_input_ended;
        
        // scene scripts
        std::string on_transition_to;
        std::string on_transition_from;
    };

} // namespace x2d
using namespace x2d;

#endif // __X2D_OBJECT_TRAITS_H__
