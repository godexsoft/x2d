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

namespace x2d {
namespace config {
    
    configuration::configuration(resource_manager& res_man, const std::string& cfg_path)
    : res_man_(res_man)
    {
        // add supported parsers
        parsers_["float"] = boost::bind(&configuration::parse_float, this, _1, _2);
        parsers_["int"] = boost::bind(&configuration::parse_int, this, _1, _2);
        
        parsers_["namespace"] = boost::bind(&configuration::parse_namespace, this, _1, _2);
        parsers_["include"] = boost::bind(&configuration::parse_include, this, _1, _2);
        parsers_["texture"] = boost::bind(&configuration::parse_texture, this, _1, _2);
        parsers_["sprite"] = boost::bind(&configuration::parse_sprite, this, _1, _2);
        
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
        return (*static_cast<sprite_cfg*>( &(*config_[key]) )).get();
    }
    
    // getters for primitive metrics
    template <>
    float configuration::get_value<float>(const config_key& key)
    {
        typedef value_cfg<float> cfg_type;
        return (*static_cast<cfg_type*>( &(*config_[key]) )).get();
    }

    template <>
    int configuration::get_value<int>(const config_key& key)
    {
        typedef value_cfg<int> cfg_type;
        return (*static_cast<cfg_type*>( &(*config_[key]) )).get();
    }

    
} // namespace config    
} // namespace x2d