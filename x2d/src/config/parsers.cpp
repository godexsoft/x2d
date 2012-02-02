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
                std::stringstream ss;
                float out;
                
                ss << data->value();
                ss >> out;

                config_[key] = boost::shared_ptr< value_cfg<float> >( new value_cfg<float>(res_man_, out) );
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
            std::stringstream ss;
            float out;
            
            ss << value->value();
            ss >> out;
            
            config_[key] = boost::shared_ptr< value_cfg<float> >( new value_cfg<float>(res_man_, out) );
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
                std::stringstream ss;
                float out;
                
                ss << data->value();
                ss >> out;
                
                config_[key] = boost::shared_ptr< value_cfg<int> >( new value_cfg<int>(res_man_, out) );
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
            std::stringstream ss;
            float out;
            
            ss << value->value();
            ss >> out;
            
            config_[key] = boost::shared_ptr< value_cfg<int> >( new value_cfg<int>(res_man_, out) );
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
        
        int xval, yval, wval, hval;
        
        // TODO: why not have it as box="x y w h" ?
        std::stringstream ss;
        ss << x->value() << " " << y->value() << " " 
        << w->value() << " " << h->value();
        ss >> xval >> yval >> wval >> hval;        
        
        LOG("Result: %d, %d, %d, %d", xval, yval, wval, hval);
        
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
        
        float duration;
        std::stringstream ss;
        ss << dur->value();
        ss >> duration;
        
        config_[key] =  boost::shared_ptr<animation_cfg>( new animation_cfg(res_man_, *this, duration) );
    }
    
    // TODO: move out of here!
    boost::shared_ptr<animation> animation_cfg::get()
    {
        if( boost::shared_ptr<animation> p = inst_.lock() )
        {            
            // already exists outside of cfg
            return p;
        }
        else
        {
            boost::shared_ptr<animation> r = boost::shared_ptr<animation>( new animation(duration_) );
            
            // add all frames
            for(int i=0; i<frames_.size(); ++i)
            {
                r->add( frame( config_.get_object<sprite>( frames_.at(i).sprite_key_) ) );
            }
            
            inst_ = r;
            return r;
        }
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

        // add this frame
        static_cast<animation_cfg*>(&(*config_[parent_key]))->add( frame_cfg(spr->value()) );
    }
    
} // namespace config
} // namespace x2d