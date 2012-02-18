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
#include "value_holder.h"

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
                    new value_cfg<float>( value_parser<float>::parse(data->value())) );
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
                new value_cfg<float>( value_parser<float>::parse(value->value())) );
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
                    new value_cfg<int>( value_parser<int>::parse(data->value())) );
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
                new value_cfg<int>( value_parser<int>::parse(value->value())) );
        }
    }
    
    void configuration::parse_vector(xml_node* node, const config_key& key)
    {
        // must have:
        // n:      name
        // can have:
        // value:  the value
        // or value can be provided as child
        
        xml_attr* path = node->first_attribute("n");
        if(!path) 
        {
            throw parse_exception("Vector must have 'n' defined.");
        }
        
        xml_attr* value = node->first_attribute("value");
        if(!value && !node->first_node()) 
        {
            throw parse_exception("Vector must have either 'value' or inner data defined.");
        }
        
        if(!value)
        {
            xml_node* data = node->first_node();
            
            if( data->type() == rx::node_data )
            {
                config_[key] = boost::shared_ptr< value_cfg<vector_25d> >(
                    new value_cfg<vector_25d>( value_parser<vector_25d>::parse(data->value())) );
            }
            else
            {
                // check if it's random
                if( std::string("random") == data->name() )
                {
                    LOG("Random detected.");
                    parse_random<vector_25d>(data, key);
                }
                else
                {
                    throw parse_exception("Data element can be either <random> or plain value.");
                }
            }
        }
        else
        {        
            config_[key] = boost::shared_ptr< value_cfg<vector_25d> >( 
                new value_cfg<vector_25d>( value_parser<vector_25d>::parse(value->value())) );
        }
    }
    
    void configuration::parse_string(xml_node* node, const config_key& key)
    {
        // must have:
        // n:      name
        // can have:
        // value:  the value
        // or value can be provided as child

        LOG("Parsing n in string");
        std::string n = get_mandatory_attr<std::string>(node, key, "n",
            parse_exception("String type must have 'n' defined.")).get(*this);

        xml_attr* value = node->first_attribute("value");
        if(!value && !node->first_node()) 
        {
            throw parse_exception("String must have either 'value' or inner data defined.");
        }
        
        if(!value)
        {
            xml_node* data = node->first_node();
            
            if( data->type() == rx::node_data || data->type() == rx::node_cdata)
            {
                config_[key] = boost::shared_ptr< value_cfg<std::string> >(
                    new value_cfg<std::string>( value_parser<std::string>::parse(data->value())) );
            }
            else
            {
                throw parse_exception("Data element of String must be plain value or CDATA.");
            }
        }
        else
        {        
            config_[key] = boost::shared_ptr< value_cfg<std::string> >( 
                new value_cfg<std::string>( value_parser<std::string>::parse(value->value())) );
        }
    }    
    
    void configuration::parse_namespace(xml_node* node, const config_key& key)
    {
        // must have:
        // n: name of the namespace

        // No need to do anything :)
    }
    
    void configuration::parse_include(xml_node* node, const config_key& key)
    {
        // must have:
        // path:    path to other configuration file
        
        std::string path = get_mandatory_attr<std::string>(node, key, "path",
            parse_exception("Include type must have 'path' defined.")).get(*this);
        
        // parse the whole config into this configuration
        parse_file( path );
    }
    
    void configuration::parse_texture(xml_node* node, const config_key& key)
    {
        // must have:
        // n:       name of the element
        // path:    path to texture file
        
        std::string path = get_mandatory_attr<std::string>(node, key, "path",
            parse_exception("Texture type must have 'path' defined.")).get(*this);
        
        config_[key] = boost::shared_ptr<texture_cfg>( new texture_cfg(res_man_, path) );
    }
    
    void configuration::parse_sprite(xml_node* node, const config_key& key)
    {
        // must have:
        // n: name of the element
        // x: x offset inside texture
        // y: y offset inside texture
        // w: width of sprite
        // h: height of sprite
        
        int x = get_mandatory_attr<int>(node, key, "x",
            parse_exception("Sprite type must have 'x' defined.")).get(*this);
        
        int y = get_mandatory_attr<int>(node, key, "y",
            parse_exception("Sprite type must have 'y' defined.")).get(*this);
        
        int w = get_mandatory_attr<int>(node, key, "w",
            parse_exception("Sprite type must have 'w' defined.")).get(*this);
        
        int h = get_mandatory_attr<int>(node, key, "h",
            parse_exception("Sprite type must have 'h' defined.")).get(*this);

        // parent must be a texture
        xml_node* parent = node->parent();
        if(! parent || std::string("texture") != parent->name())
        {
            throw structure_exception("Sprite must be a child element of a Texture object.");
        }
        
        config_key parent_key = key.remove_last_path_component();        

        config_[key] =  boost::shared_ptr<sprite_cfg>( 
            new sprite_cfg(*this, parent_key, 
                point(x, y), size(w, h)) );
    }

    void configuration::parse_animation(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        // duration: default duration for every frame
        
        float dur = get_mandatory_attr<float>(node, key, "duration",
            parse_exception("Animation type must have 'duration' defined.")).get(*this);

        if( config_.find(key) == config_.end() )
        {
            // No animation object exists! No frames defined?
            throw structure_exception("Animation has no frames. Invalid animation.");
        }
        else
        {
            // Set duration
            static_cast<animation_cfg*>(&(*config_[key]))
                ->set_duration(dur);   
        }
    }
    
    void configuration::parse_frame(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        // sprite:   key for the sprite item to use
        // can have:
        // duration: duration for this frame
        
        std::string spr = get_mandatory_attr<std::string>(node, key, "sprite",
            parse_exception("Frame type must have 'sprite' defined.")).get(*this);

        // parent must be an animation
        xml_node* parent = node->parent();
        if(! parent || std::string("animation") != parent->name())
        {
            throw structure_exception("Frame must be a child element of an Animation object.");
        }
        
        config_key parent_key = key.remove_last_path_component();
        if( config_.find(parent_key) == config_.end() )
        {
            // No animation exists yet. Must create one as this is the first frame.
            config_[parent_key] = boost::shared_ptr<animation_cfg>( 
                new animation_cfg(*this) );
        }
        
        // custom duration?
        xml_attr* dur = node->first_attribute("duration");
        if(dur) 
        {
            // add this frame with custom duration
            static_cast<animation_cfg*>(&(*config_[parent_key]))->add( 
                frame_cfg(spr, value_parser<float>::parse(dur->value())) );   
        }
        else
        {
            // add this frame
            static_cast<animation_cfg*>(&(*config_[parent_key]))->add( frame_cfg(spr) );
        }
    }
    
    void configuration::parse_font(xml_node* node, const config_key& key)
    {
        // must have:
        // n:          name of the element
        // height:     height of the font
        // spacing:    spacing between sprites
        // texture:    texture object key
        // characters: the characters supported by this font
        // widths:     list of widths per character (given as string with spaces)
        
        int height = get_mandatory_attr<int>(node, key, "height",
            parse_exception("Font type must have 'height' defined.")).get(*this);
        
        size spacing = get_mandatory_attr<size>(node, key, "spacing", 
            parse_exception("Font type must have 'spacing' defined.")).get(*this);
        
        std::string txtr = get_mandatory_attr<std::string>(node, key, "texture", 
            parse_exception("Font type must have 'texture' defined.")).get(*this);
        
        std::string characters = get_mandatory_attr<std::string>(node, key, "characters", 
            parse_exception("Font type must have 'characters' defined.")).get(*this);
        
        std::string widths = get_mandatory_attr<std::string>(node, key, "widths",
            parse_exception("Font type must have 'widths' defined.")).get(*this);

        // get vector of ints out of string widths
        int v;
        std::vector<int> widths_vec;

        std::stringstream ss;
        ss << widths;
        
        while( ss >> v )
        {
            widths_vec.push_back(v);
        }
        
        LOG("Parsed %d widths from font.widths string.", widths_vec.size());
        
        config_[key] =  
            boost::shared_ptr<font_cfg>( 
                new font_cfg(*this, height, spacing, txtr, characters, widths_vec));

    }
    
    void configuration::parse_music(xml_node* node, const config_key& key)
    {
        // must have:
        // n:          name of the element
        // path:       path to resource
        //
        // can have:
        // loop:       loop the music? true or false. false is default.
        // volume:     defaults to 1.0 - full gain
        
        std::string path = get_mandatory_attr<std::string>(node, key, "path", 
            parse_exception("Music type must have 'path' defined.")).get(*this);
        
        bool loop = get_attr<bool>(node, key, "loop", false).get(*this);
        float gain = get_attr<float>(node, key, "volume", 1.0f).get(*this);
        
        config_[key] =  
            boost::shared_ptr<music_cfg>( 
                new music_cfg(res_man_, path, loop, gain));
        
    }

    void configuration::parse_sfx(xml_node* node, const config_key& key)
    {
        // must have:
        // n:          name of the element
        // path:       path to resource
        //
        // can have:
        // pitch:      pitch for the effect. defaults to 1.0f
        // loop:       loop the sfx? false is default.
        
        std::string path = get_mandatory_attr<std::string>(node, key, "path", 
            parse_exception("Sfx type must have 'path' defined.")).get(*this);
        
        bool loop   = get_attr<bool>(node, key, "loop", false).get(*this);
        value_holder<float> pitch = get_attr<float>(node, key, "pitch", 1.0f);        
        
        config_[key] =  
        boost::shared_ptr<sfx_cfg>( 
            new sfx_cfg(*this, path, loop, pitch));
        
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
               
        size frustum = get_mandatory_attr<size>(node, key, "frustum",
            parse_exception("Camera type must have 'frustum' defined (format: 'width height').")).get(*this);

        config_[key] =  
            boost::shared_ptr<camera_cfg>( 
                new camera_cfg(*this, frustum, 
                    get_attr<float>(node, key, "rotation", 0.0f),
                    get_attr<float>(node, key, "zoom", 1.0f), 
                    get_attr<vector_2d>(node, key, "position", vector_2d(0.0f, 0.0f))) );
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

        rect box = get_mandatory_attr<rect>(node, key, "box",
            parse_exception("Viewport type must have 'box' defined (format: 'x y width height').")).get(*this);

        std::string cam = get_mandatory_attr<std::string>(node, key, "camera",
            parse_exception("Viewport type must have 'camera' defined.")).get(*this);
        
        color_info ci = get_attr<color_info>(node, key, "bgcolor", color_info(0.0f, 0.0f, 0.0f)).get(*this);
        
        boost::shared_ptr<viewport_cfg> vp = 
            boost::shared_ptr<viewport_cfg>( new viewport_cfg(
                *this, box, cam, ci) );
        
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
                kernel_, want_touch, want_accel) );
        
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
        // space:     'world' (default), 'screen' or 'camera' space
        
        object_traits tr;
        
        xml_attr* parent = node->first_attribute("parent");
        if(parent) 
        {
            static_cast<object_cfg*>(&(*config_[parent->value()]))->add( key );
            tr.has_parent = true;
        }
        
        tr.position =   get_attr<vector_25d>(node, key, "position", vector_25d(0.0f, 0.0f, 0.0f));
        tr.scale =      get_attr<float>(node, key, "scale", 1.0f);
        tr.rotation =   get_attr<float>(node, key, "rotation", 0.0f);
        
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
                
        xml_attr* sp = node->first_attribute("space");
        if(sp) 
        {
            tr.obj_space = value_parser<space>::parse( sp->value() );
        }
        else
        {
            tr.obj_space = WORLD_SPACE;
        }
        
        config_[key] = 
            boost::shared_ptr<object_cfg>( 
                new object_cfg(*this, kernel_, tr) );
    }
    
} // namespace config
} // namespace x2d