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
#include "plugin.h"

#include <boost/bind.hpp>

namespace x2d {
namespace config {

    configuration::configuration(kernel& k, resource_manager& res_man)
    : kernel_(k)
    , res_man_(res_man)
    {
        load_core_parsers();
    }
    
    configuration::configuration(kernel& k, resource_manager& res_man, const std::string& cfg_path)
    : kernel_(k)
    , res_man_(res_man)
    {
        load_core_parsers();
        
        // finally parse the config
        parse_file(cfg_path);
    }
    
    void configuration::load_core_parsers()
    {
        // add x2d core parsers
        parsers_["float"]       = boost::bind(&configuration::parse_float, this, _1, _2);
        parsers_["int"]         = boost::bind(&configuration::parse_int, this, _1, _2);
        parsers_["string"]      = boost::bind(&configuration::parse_string, this, _1, _2);
        parsers_["vector"]      = boost::bind(&configuration::parse_vector, this, _1, _2);
        
        parsers_["camera"]      = boost::bind(&configuration::parse_camera, this, _1, _2);
        parsers_["viewport"]    = boost::bind(&configuration::parse_viewport, this, _1, _2);
        parsers_["input"]       = boost::bind(&configuration::parse_input, this, _1, _2);
        
        parsers_["namespace"]   = boost::bind(&configuration::parse_namespace, this, _1, _2);
        parsers_["include"]     = boost::bind(&configuration::parse_include, this, _1, _2);
        parsers_["texture"]     = boost::bind(&configuration::parse_texture, this, _1, _2);
        parsers_["sprite"]      = boost::bind(&configuration::parse_sprite, this, _1, _2);
        parsers_["animation"]   = boost::bind(&configuration::parse_animation, this, _1, _2);
        parsers_["frame"]       = boost::bind(&configuration::parse_frame, this, _1, _2);
        parsers_["font"]        = boost::bind(&configuration::parse_font, this, _1, _2);
        parsers_["music"]       = boost::bind(&configuration::parse_music, this, _1, _2);
        parsers_["sfx"]         = boost::bind(&configuration::parse_sfx, this, _1, _2);                
        parsers_["spawner"]     = boost::bind(&configuration::parse_spawner, this, _1, _2);        
        
        parsers_["object"]      = boost::bind(&configuration::parse_object, this, _1, _2);        
        parsers_["context"]     = boost::bind(&configuration::parse_context, this, _1, _2);        
        parsers_["zone"]        = boost::bind(&configuration::parse_zone, this, _1, _2);
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
        // try to parse everything inside this node if it's a container
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
        
        // now parse the actual root node and see if it's usable for us
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
    }
    
    void configuration::load_plugin(const boost::shared_ptr<plugin>& plg)
    {
        plg->load_extensions( parsers_ );
    }
    
    const boost::shared_ptr<object> configuration::create_object(const config_key& key)
    {
        LOG("Try to lookup binding for '%s'", key.string().c_str());
        // try to find binding first
        if(create_obj_bindings_.find(key) != create_obj_bindings_.end())
        {
            LOG("FOUND!");
            // forward to strong-typed version
            return create_obj_bindings_[key]( key );
        }

        LOG("No binding. return plain object version!");
        return static_cast<object_cfg*>( &(*config_[key]) )->create();
    }
    
    template <>
    boost::shared_ptr<texture> configuration::get_object<texture>(const config_key& key)
    {
        return static_cast<texture_cfg*>( &(*config_[key]) )->get();
    }
    
    template <>
    boost::shared_ptr<sprite> configuration::get_object<sprite>(const config_key& key)
    {
        return static_cast<sprite_cfg*>( &(*config_[key]) )->get();
    }

    template <>
    const boost::shared_ptr<animation> configuration::create_sys_object<animation>(const config_key& key)
    {
        return static_cast<animation_cfg*>( &(*config_[key]) )->create();
    }

    template <>
    const boost::shared_ptr<spawner> configuration::create_sys_object<spawner>(const config_key& key)
    {
        return static_cast<spawner_cfg*>( &(*config_[key]) )->create();
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

    template <>
    boost::shared_ptr<font> configuration::get_object<font>(const config_key& key)
    {
        return static_cast<font_cfg*>( &(*config_[key]) )->get();
    }
    
    template <>
    boost::shared_ptr<music> configuration::get_object<music>(const config_key& key)
    {
        return static_cast<music_cfg*>( &(*config_[key]) )->get();
    }

    template <>
    boost::shared_ptr<sfx> configuration::get_object<sfx>(const config_key& key)
    {
        return static_cast<sfx_cfg*>( &(*config_[key]) )->get();
    }

    template <>
    boost::shared_ptr<context> configuration::get_object<context>(const config_key& key)
    {
        return static_cast<context_cfg*>( &(*config_[key]) )->get();
    }
    
    template <>
    boost::shared_ptr<zone> configuration::get_object<zone>(const config_key& key)
    {
        return static_cast<zone_cfg*>( &(*config_[key]) )->get();
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

    template <>
    bool configuration::get_value<bool>(const config_key& key)
    {
        typedef value_cfg<bool> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }
    
    template <>
    point configuration::get_value<point>(const config_key& key)
    {
        typedef value_cfg<point> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }

    template <>
    size configuration::get_value<size>(const config_key& key)
    {
        typedef value_cfg<size> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }

    template <>
    rect configuration::get_value<rect>(const config_key& key)
    {
        typedef value_cfg<rect> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }
    
    template <>
    std::string configuration::get_value<std::string>(const config_key& key)
    {
        typedef value_cfg<std::string> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }
    
    template <>
    color_info configuration::get_value<color_info>(const config_key& key)
    {
        typedef value_cfg<color_info> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }
    
    template <>
    glm::vec2 configuration::get_value<glm::vec2>(const config_key& key)
    {
        typedef value_cfg<glm::vec2> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }
    
    template <>
    glm::vec3 configuration::get_value<glm::vec3>(const config_key& key)
    {
        typedef value_cfg<glm::vec3> cfg_type;
        return static_cast<cfg_type*>( &(*config_[key]) )->get();
    }
    
    boost::shared_ptr<sprite> sprite_cfg::get()
    {
        if( boost::shared_ptr<sprite> p = inst_.lock() )
        {            
            // already exists outside of cfg
            return p;
        }
        else
        {
            boost::shared_ptr<sprite> r = boost::shared_ptr<sprite>( 
                new sprite( config_.get_object<texture>(texture_), origin_, size_, pivot_) );
            inst_ = r;
            return r;
        }
    }
        
    boost::shared_ptr<animation> animation_cfg::create()
    {
        boost::shared_ptr<animation> r = boost::shared_ptr<animation>( new animation(duration_, pivot_) );
        
        // add all frames
        for(int i=0; i<frames_.size(); ++i)
        {
            r->add( frame( config_.get_object<sprite>( frames_.at(i).sprite_key_), 
                frames_.at(i).duration_>0.0f?frames_.at(i).duration_:duration_ ) );
        }

        return r;
    }    

    boost::shared_ptr<spawner> spawner_cfg::create()
    {
        boost::shared_ptr<spawner> r = boost::shared_ptr<spawner>( 
            new spawner(config_.get_kernel(), config_, obj_lst_, position_, wave_size_, wave_delay_) );        
        return r;
    }    
    
    boost::shared_ptr<camera> camera_cfg::get()
    {
        if( boost::shared_ptr<camera> p = inst_.lock() )
        {            
            // already exists outside of cfg
            return p;
        }
        else
        {
            boost::shared_ptr<camera> r = boost::shared_ptr<camera>( new camera(frustum_) );
            r->position(position_.get(config_));                
            r->zoom(zoom_.get(config_));
            r->rotation(rotation_.get(config_));
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
    
    boost::shared_ptr<font> font_cfg::get()
    {
        if( boost::shared_ptr<font> p = inst_.lock() )
        {            
            // already exists outside of cfg
            return p;
        }
        else
        {
            boost::shared_ptr<font> f = 
                boost::shared_ptr<font>(
                    new font( characters_, widths_, height_, spacing_, config_.get_object<texture>(texture_) ) );
            inst_ = f;
            return f;
        }
    }
    
    boost::shared_ptr<sfx> sfx_cfg::get()
    {
        if( boost::shared_ptr<sfx> p = inst_.lock() )
        {            
            // already exists outside of cfg
            return p;
        }
        else
        {
            boost::shared_ptr<sfx> r = 
            boost::shared_ptr<sfx>( new sfx(config_, 
                config_.get_resman().get<ifdstream>(path_), loop_, pitch_) );
            inst_ = r;
            return r;
        }
    }

    boost::shared_ptr<zone> zone_cfg::get()
    {
        if( boost::shared_ptr<zone> p = inst_.lock() )
        {            
            // already exists outside of cfg
            return p;
        }
        else
        {
            // create and add to all specified contexts. TODO: support '*' context
            // currently always creating rectangular zone
            boost::shared_ptr<zone> r = boost::shared_ptr<zone>( new rectangular_zone(config_.get_kernel(), box_) );
            for(std::vector<std::string>::iterator it = ctx_lst_.begin(); it != ctx_lst_.end(); ++it)
            {
                r->add_context( config_.get_object<context>(*it) );
            }
            
            inst_ = r;
            return r;
        }
    }
    
    void object_cfg::add_children(const boost::shared_ptr<object>& p)
    {
        for(int i=0; i<children_.size(); ++i)
        {
            p->add_child( config_.create_object(children_.at(i)) );
        }
    }
    
} // namespace config    
} // namespace x2d