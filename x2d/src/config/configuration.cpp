//
//  configuration.cpp
//  x2d
//
//  Created by Alex Kremer on 1/30/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"
#include "sprite.h"

#include <boost/bind.hpp>
#include <sstream>

namespace x2d {
namespace config {
    
    configuration::configuration(resource_manager& res_man, const std::string& cfg_path)
    : res_man_(res_man)
    {
        boost::shared_ptr<conf> cfg = res_man_.get<conf>(cfg_path);

		xml_doc doc;
		try 
        {
			doc.parse<0>(&(cfg->data.at(0)));
		} 
        catch( rx::parse_error& ex ) 
        {
			LOG( "RapidXML parse error: %s", ex.what() );
			throw std::exception();
		}
		
		xml_node *root = doc.first_node( "x2d" );
        
		if( !root ) 
        {
            LOG("-- Not a valid x2d config xml. must begin with <x2d> root element.");
			throw std::exception();
		}
		
		xml_attr *attr = root->first_attribute();
		if( !attr || std::string("version") != attr->name() 
           || std::string("1.0") != attr->value()) 
		{
            LOG("-- Not a valid x2d config xml file version (or version not specified).");
            throw std::exception();
		}
        
        // add supported parsers
        parsers_["texture"] = boost::bind(&configuration::parse_texture, this, _1, _2);
        parsers_["sprite"] = boost::bind(&configuration::parse_sprite, this, _1, _2);
        
        
        // finally parse the config
        parse(root);
    }
    
    void configuration::parse(xml_node* root, const config_key& key)
    {
        // first parse this node and see if it's usable for us
        if( root->type() == rx::node_element )
        {
         	LOG("-- Parsing: %s", root->name());
            if( parsers_.find( root->name() ) != parsers_.end() )
            {            
                config_[key] = parsers_[root->name()](root, key);
                LOG("Saved under key '%s'", key.string().c_str());
            }
            else
            {
                LOG("'%s' is not supported.", root->name());
            }
        }
        
        // now try to parse everything inside this node if it's a container
        for(xml_node* node = root->first_node(); node; node = node->next_sibling()) 
		{
			if(node->type() == rx::node_element) 
            {
				LOG("-- NAME: %s", node->name());
                
                xml_attr* name = node->first_attribute("n");
				if(name) 
                {                    
					config_key cur_key = key / config_key(name->value());
                    LOG("-- key: %s", cur_key.string().c_str());
					parse( node, cur_key );
				}
                else
                {
                    // still have to parse inner stuff if any
                    parse( node, key );
                }
			}
		}		        
    }
    
    template <>
    boost::shared_ptr<sprite> configuration::get<sprite>(const config_key& key)
    {
        sprite_cfg cfg = *static_cast<sprite_cfg*>(&(*config_[key]));
        return cfg.get();
    }

    
    // parsers
    cfg_base_ptr configuration::parse_texture(xml_node* node, const config_key& key)
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
        
        return boost::shared_ptr<texture_cfg>( new texture_cfg(res_man_, path->value()) );
    }

    cfg_base_ptr configuration::parse_sprite(xml_node* node, const config_key& key)
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
        
        return boost::shared_ptr<sprite_cfg>( 
            new sprite_cfg(res_man_, *static_cast<texture_cfg*>(&(*config_[parent_key])), 
                point(xval, yval), size(wval, hval)) );
    }

    
} // namespace config    
} // namespace x2d