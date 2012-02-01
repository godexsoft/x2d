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

#include <sstream>

namespace x2d {
namespace config {
 
    void configuration::parse_include(xml_node* node, const config_key& key)
    {
        // must have:
        // path:    path to other configuration file
        
        xml_attr* path = node->first_attribute("path");
        if(!path) 
        {
            LOG("Parsing exception: Include must have 'path' defined.");
            throw std::exception();
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
            LOG("Parsing exception: Texture type must have 'n' defined.");
            throw std::exception();
        }
        
        xml_attr* path = node->first_attribute("path");
        if(!path) 
        {
            LOG("Parsing exception: Texture type must have 'path' defined.");
            throw std::exception();
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
            LOG("Parsing exception: Sprite type must have 'n' defined.");
            throw std::exception();
        }
        
        xml_attr* x = node->first_attribute("x");
        if(!x) 
        {
            LOG("Parsing exception: Sprite type must have 'x' defined.");
            throw std::exception();
        }
        
        xml_attr* y = node->first_attribute("y");
        if(!y) 
        {
            LOG("Parsing exception: Sprite type must have 'y' defined.");
            throw std::exception();
        }
        
        xml_attr* w = node->first_attribute("w");
        if(!w) 
        {
            LOG("Parsing exception: Sprite type must have 'w' defined.");
            throw std::exception();
        }
        
        xml_attr* h = node->first_attribute("h");
        if(!h) 
        {
            LOG("Parsing exception: Sprite type must have 'h' defined.");
            throw std::exception();
        }
        
        // parent must be a texture
        xml_node* parent = node->parent();
        if(! parent || std::string("texture") != parent->name())
        {
            LOG("Structure exception: Sprite must be a child element of a Texture object.");
            throw std::exception();
        }
        
        config_key parent_key = key.remove_last_path_component();        
        if( config_.find(parent_key) == config_.end() )
        {
            LOG("Structure exception: Sprite's texture is not found: '%s'", parent_key.string().c_str());
            throw std::exception();        
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

} // namespace config
} // namespace x2d