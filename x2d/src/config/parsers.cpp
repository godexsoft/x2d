//
//  parsers.cpp
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"
#include "texture.h"
#include "sprite.h"
#include "exceptions.h"
#include "kernel.h"
#include "object.h"

#include <sstream>

namespace x2d {
namespace config {
    
    void configuration::parse_float(xml_node* node, const config_key& key)
    {
        // must have:
        // n:      name
        // can have:
        // value:  the value
        // or value can be provided as child
        
        xml_attr* path = node->first_attribute("n");
        if(!path) 
        {
            throw parse_exception("Float must have 'n' defined.");
        }
        
        xml_attr* value = node->first_attribute("value");
        if(!value && !node->first_node()) 
        {
            throw parse_exception("Float must have either 'value' or inner data defined.");
        }
        
        if(!value)
        {
            xml_node* data = node->first_node();
            
            if( data->type() == rx::node_data )
            {
                config_[key] = boost::shared_ptr< value_cfg<float> >( 
                    new value_cfg<float>(res_man_, value_parser<float>::parse(data->value())) );
            }
            else
            {
                // check if it's random
                if( std::string("random") == data->name() )
                {
                    LOG("Random detected.");
                    parse_random<float>(data, key);
                }
                else
                {
                    throw parse_exception("Data element can be either <random> or plain value.");
                }
            }
        }
        else
        {   
            config_[key] = boost::shared_ptr< value_cfg<float> >( 
                new value_cfg<float>(res_man_, value_parser<float>::parse(value->value())) );
        }
    }
    
    void configuration::parse_int(xml_node* node, const config_key& key)
    {
        // must have:
        // n:      name
        // can have:
        // value:  the value
        // or value can be provided as child
        
        xml_attr* path = node->first_attribute("n");
        if(!path) 
        {
            throw parse_exception("Int must have 'n' defined.");
        }
        
        xml_attr* value = node->first_attribute("value");
        if(!value && !node->first_node()) 
        {
            throw parse_exception("Int must have either 'value' or inner data defined.");
        }
        
        if(!value)
        {
            xml_node* data = node->first_node();
            
            if( data->type() == rx::node_data )
            {
                config_[key] = boost::shared_ptr< value_cfg<int> >(
                    new value_cfg<int>(res_man_, value_parser<int>::parse(data->value())) );
            }
            else
            {
                // check if it's random
                if( std::string("random") == data->name() )
                {
                    LOG("Random detected.");
                    parse_random<int>(data, key);
                }
                else
                {
                    throw parse_exception("Data element can be either <random> or plain value.");
                }
            }
        }
        else
        {        
            config_[key] = boost::shared_ptr< value_cfg<int> >( 
                new value_cfg<int>(res_man_, value_parser<int>::parse(value->value())) );
        }
    }
    
    void configuration::parse_namespace(xml_node* node, const config_key& key)
    {
        // must have:
        // n: name of the namespace
        
        xml_attr* path = node->first_attribute("n");
        if(!path) 
        {
            throw parse_exception("Namespace must have 'n' defined.");
        }
        
        // No need to do anything :)
    }
    
    void configuration::parse_include(xml_node* node, const config_key& key)
    {
        // must have:
        // path:    path to other configuration file
        
        xml_attr* path = node->first_attribute("path");
        if(!path) 
        {
            throw parse_exception("Include must have 'path' defined.");
        }
        
        // parse the whole config into this configuration
        parse_file( path->value() );
    }
    
    void configuration::parse_texture(xml_node* node, const config_key& key)
    {
        // must have:
        // n:       name of the element
        // path:    path to texture file
        
        xml_attr* name = node->first_attribute("n");
        if(!name) 
        {
            throw parse_exception("Texture type must have 'n' defined.");
        }
        
        xml_attr* path = node->first_attribute("path");
        if(!path) 
        {
            throw parse_exception("Texture type must have 'path' defined.");
        }
        
        config_[key] = boost::shared_ptr<texture_cfg>( new texture_cfg(res_man_, path->value()) );
    }
    
    void configuration::parse_sprite(xml_node* node, const config_key& key)
    {
        // must have:
        // n: name of the element
        // x: x offset inside texture
        // y: y offset inside texture
        // w: width of sprite
        // h: height of sprite
        
        xml_attr* name = node->first_attribute("n");
        if(!name) 
        {
            throw parse_exception("Sprite type must have 'n' defined.");
        }
        
        xml_attr* x = node->first_attribute("x");
        if(!x) 
        {
            throw parse_exception("Sprite type must have 'x' defined.");
        }
        
        xml_attr* y = node->first_attribute("y");
        if(!y) 
        {
            throw parse_exception("Sprite type must have 'y' defined.");
        }
        
        xml_attr* w = node->first_attribute("w");
        if(!w) 
        {
            throw parse_exception("Sprite type must have 'w' defined.");
        }
        
        xml_attr* h = node->first_attribute("h");
        if(!h) 
        {
            throw parse_exception("Sprite type must have 'h' defined.");
        }
        
        // parent must be a texture
        xml_node* parent = node->parent();
        if(! parent || std::string("texture") != parent->name())
        {
            throw structure_exception("Sprite must be a child element of a Texture object.");
        }
        
        config_key parent_key = key.remove_last_path_component();        
        if( config_.find(parent_key) == config_.end() )
        {
            throw structure_exception("Sprite's texture is not found: '" + parent_key.string() + "'");
        }

        // TODO: why not have it as box="x y w h" ?        
        int xval, yval, wval, hval;
        xval = value_parser<int>::parse(x->value());
        yval = value_parser<int>::parse(y->value());
        wval = value_parser<int>::parse(w->value());
        hval = value_parser<int>::parse(h->value());
        
        config_[key] =  boost::shared_ptr<sprite_cfg>( 
            new sprite_cfg(res_man_, *static_cast<texture_cfg*>(&(*config_[parent_key])), 
                point(xval, yval), size(wval, hval)) );
    }

    void configuration::parse_animation(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        // duration: default duration for every frame
        
        xml_attr* name = node->first_attribute("n");
        if(!name) 
        {
            throw parse_exception("Animation type must have 'n' defined.");
        }
        
        xml_attr* dur = node->first_attribute("duration");
        if(!dur) 
        {
            throw parse_exception("Animation type must have 'duration' defined.");
        }

        config_[key] =  boost::shared_ptr<animation_cfg>( 
            new animation_cfg(res_man_, *this, value_parser<float>::parse(dur->value())) );
    }
    
    void configuration::parse_frame(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        // sprite:   key for the sprite item to use
        // can have:
        // duration: duration for this frame
        
        xml_attr* name = node->first_attribute("n");
        if(!name) 
        {
            throw parse_exception("Frame type must have 'n' defined.");
        }
        
        xml_attr* spr = node->first_attribute("sprite");
        if(!spr) 
        {
            throw parse_exception("Frame type must have 'sprite' defined.");
        }
        
        // parent must be an animation
        xml_node* parent = node->parent();
        if(! parent || std::string("animation") != parent->name())
        {
            throw structure_exception("Frame must be a child element of an Animation object.");
        }
        
        config_key parent_key = key.remove_last_path_component();        
        if( config_.find(parent_key) == config_.end() )
        {
            throw structure_exception("Frame's animation object is not found: '" + parent_key.string() + "'");
        }

        // custom duration?
        xml_attr* dur = node->first_attribute("duration");
        if(dur) 
        {
            // add this frame with custom duration
            static_cast<animation_cfg*>(&(*config_[parent_key]))->add( 
                frame_cfg(spr->value(), value_parser<float>::parse(dur->value())) );   
        }
        else
        {
            // add this frame
            static_cast<animation_cfg*>(&(*config_[parent_key]))->add( frame_cfg(spr->value()) );
        }
    }
    
    void configuration::parse_camera(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        // frustum:  frustum of this camera
        //
        // can have:
        // rotation: initial rotation (defaults to 0.0)
        // zoom:     initial zoom (defaults to 1.0)
        // position: initial position (defaults to 0,0)
        
        xml_attr* name = node->first_attribute("n");
        if(!name) 
        {
            throw parse_exception("Camera type must have 'n' defined.");
        }
        
        xml_attr* frustum = node->first_attribute("frustum");
        if(!frustum) 
        {
            throw parse_exception("Camera type must have 'frustum' defined (format: 'width height').");
        }

        size frus = value_parser<size>::parse(frustum->value());

        // optional
        float rot = 0.0f;
        float zm = 1.0f;
        vector_2d pos(0.0f, 0.0f);
        
        xml_attr* rotation = node->first_attribute("rotation");
        if(rotation) 
        {            
            rot = value_parser<float>::parse(rotation->value());
        }

        xml_attr* zoom = node->first_attribute("zoom");
        if(zoom) 
        {            
            zm = value_parser<float>::parse(zoom->value());
        }

        xml_attr* position = node->first_attribute("position");
        if(position) 
        {            
            pos = value_parser<vector_2d>::parse(position->value());
        }

        config_[key] =  
            boost::shared_ptr<camera_cfg>( 
                new camera_cfg(res_man_, frus, rot, zm, pos) );
    }

    void configuration::parse_viewport(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        // box:      position and size of this viewport
        // camera:   camera to use with this viewport        
        //
        // can have:
        // bgcolor:  background color. defaults to black.
        
        xml_attr* name = node->first_attribute("n");
        if(!name) 
        {
            throw parse_exception("Viewport type must have 'n' defined.");
        }
        
        xml_attr* box = node->first_attribute("box");
        if(!box) 
        {
            throw parse_exception("Viewport type must have 'box' defined (format: 'x y width height').");
        }

        xml_attr* cam = node->first_attribute("camera");
        if(!cam) 
        {
            throw parse_exception("Viewport type must have 'camera' defined.");
        }

        color_info ci(0.0f, 0.0f, 0.0f);
        rect bx = value_parser<rect>::parse(box->value());        
        
        xml_attr* bg = node->first_attribute("bgcolor");
        if(bg) 
        {
            ci = value_parser<color_info>::parse(bg->value());
        }
        
        boost::shared_ptr<viewport_cfg> vp = 
            boost::shared_ptr<viewport_cfg>( new viewport_cfg(
                res_man_, *this, bx, cam->value(), ci) );
        
        config_[key] = vp;
        
        // and add this viewport to the kernel
        kernel_.add_viewport(vp->get());
    }

    void configuration::parse_input(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        //
        // can have:
        // touch:           boolean; enable/disable touch input. defaults to true
        // accelerometer:   boolean; enable/disable accelerometer input. defaults to false
        
        xml_attr* name = node->first_attribute("n");
        if(!name) 
        {
            throw parse_exception("Input type must have 'n' defined.");
        }
        
        bool want_touch = true;
        xml_attr* tch = node->first_attribute("touch");
        if(tch) 
        {
            want_touch = value_parser<bool>::parse(tch->value());
        }
        
        bool want_accel = false;
        xml_attr* accel = node->first_attribute("accelerometer");
        if(accel) 
        {
            want_accel = value_parser<bool>::parse(accel->value());
        }
                
        boost::shared_ptr<input_cfg> inp = 
            boost::shared_ptr<input_cfg>( new input_cfg(
                res_man_, kernel_, want_touch, want_accel) );
        
        config_[key] = inp;
        
        // and bind this input manager with the kernel
        kernel_.set_input_manager(inp->get());
    }

    void configuration::parse_object(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        //
        // can have:
        // parent:    parent object (inherit space - position, rotation, scale, etc.)
        // position:  position vector2d
        // scale:     scale as float
        // rotation:  rotation as float angle in degrees
        // sprite:    sprite to draw
        // animation: animation to draw        
        
        xml_attr* name = node->first_attribute("n");
        if(!name) 
        {
            throw parse_exception("Object type must have 'n' defined.");
        }
        
        object_traits tr;
        
        xml_attr* parent = node->first_attribute("parent");
        if(parent) 
        {
            static_cast<object_cfg*>(&(*config_[parent->value()]))->add( key );
        }
        
        xml_attr* position = node->first_attribute("position");
        if(position) 
        {
            tr.position = value_parser<vector_2d>::parse(position->value());
        }
        
        xml_attr* scale = node->first_attribute("scale");
        if(scale) 
        {
            tr.scale = value_parser<float>::parse(scale->value());
        }
        
        xml_attr* rotation = node->first_attribute("rotation");
        if(rotation) 
        {
            tr.rotation = value_parser<float>::parse(rotation->value());
        }
                
        xml_attr* anim = node->first_attribute("animation");
        if(anim) 
        {
            tr.animation = anim->value();
            tr.has_animation = true;
        }

        xml_attr* spr = node->first_attribute("sprite");
        if(spr) 
        {
            tr.sprite = spr->value();
            tr.has_sprite = true;
        }

        config_[key] = 
            boost::shared_ptr<object_cfg>( 
                new object_cfg(res_man_, *this, kernel_, tr) );
    }
    
} // namespace config
} // namespace x2d