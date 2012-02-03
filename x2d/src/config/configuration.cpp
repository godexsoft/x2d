//
//  configuration.cpp
//  x2d
//
//  Created by Alex Kremer on 1/30/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"
#include "sprite.h"
#include "exceptions.h"

#include <boost/bind.hpp>

namespace x2d {
namespace config {
    
    configuration::configuration(kernel& k, resource_manager& res_man, const std::string& cfg_path)
    : kernel_(k)
    , res_man_(res_man)
    {
        // add supported parsers
        parsers_["float"]       = boost::bind(&configuration::parse_float, this, _1, _2);
        parsers_["int"]         = boost::bind(&configuration::parse_int, this, _1, _2);
        
        parsers_["camera"]      = boost::bind(&configuration::parse_camera, this, _1, _2);
        parsers_["viewport"]    = boost::bind(&configuration::parse_viewport, this, _1, _2);
        parsers_["input"]       = boost::bind(&configuration::parse_input, this, _1, _2);
        
        parsers_["namespace"]   = boost::bind(&configuration::parse_namespace, this, _1, _2);
        parsers_["include"]     = boost::bind(&configuration::parse_include, this, _1, _2);
        parsers_["texture"]     = boost::bind(&configuration::parse_texture, this, _1, _2);
        parsers_["sprite"]      = boost::bind(&configuration::parse_sprite, this, _1, _2);
        parsers_["animation"]   = boost::bind(&configuration::parse_animation, this, _1, _2);
        parsers_["frame"]       = boost::bind(&configuration::parse_frame, this, _1, _2);
        
        // finally parse the config
        parse_file(cfg_path);
    }
    
    void configuration::parse_file(const std::string& cfg_path)
    {
        boost::shared_ptr<conf> cfg = res_man_.get<conf>(cfg_path);
        
		xml_doc doc;
		try 
        {
			doc.parse<0>(&(cfg->data.at(0)));
		} 
        catch( rx::parse_error& ex ) 
        {
			throw parse_exception( std::string("RapidXML parse error: ") + ex.what() );
		}
		
		xml_node *root = doc.first_node( "x2d" );
        
		if( !root ) 
        {
			throw structure_exception("Not a valid x2d config xml. must begin with <x2d> root element.");
		}
		
		xml_attr *attr = root->first_attribute();
		if( !attr || std::string("version") != attr->name() 
           || std::string("1.0") != attr->value()) 
		{
            throw structure_exception("Not a valid x2d config xml file version (or version not specified).");
		}
        
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
                parsers_[root->name()](root, key);
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
    boost::shared_ptr<sprite> configuration::get_object<sprite>(const config_key& key)
    {
        return static_cast<sprite_cfg*>( &(*config_[key]) )->get();
    }

    template <>
    boost::shared_ptr<animation> configuration::get_object<animation>(const config_key& key)
    {
        return static_cast<animation_cfg*>( &(*config_[key]) )->get();
    }

    template <>
    boost::shared_ptr<camera> configuration::get_object<camera>(const config_key& key)
    {
        return static_cast<camera_cfg*>( &(*config_[key]) )->get();
    }

    template <>
    boost::shared_ptr<viewport> configuration::get_object<viewport>(const config_key& key)
    {
        return static_cast<viewport_cfg*>( &(*config_[key]) )->get();
    }
    
    // getters for primitive metrics
    template <>
    float configuration::get_value<float>(const config_key& key)
    {
        typedef value_cfg<float> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }

    template <>
    int configuration::get_value<int>(const config_key& key)
    {
        typedef value_cfg<int> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }

    
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
                r->add( frame( config_.get_object<sprite>( frames_.at(i).sprite_key_), 
                    frames_.at(i).duration_>0.0f?frames_.at(i).duration_:duration_ ) );
            }
            
            inst_ = r;
            return r;
        }
    }
    
    boost::shared_ptr<viewport> viewport_cfg::get()
    {
        if( boost::shared_ptr<viewport> p = inst_.lock() )
        {            
            // already exists outside of cfg
            return p;
        }
        else
        {
            boost::shared_ptr<viewport> r = 
                boost::shared_ptr<viewport>(
                    new viewport( box_, config_.get_object<camera>(camera_key_), bg_color_ ) );
            inst_ = r;
            return r;
        }
    }
    
} // namespace config    
} // namespace x2d