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
    , object_input_manager_(kernel_)
    , scripting_engine_(*this)
    {
        load_core_parsers();
    }
    
    configuration::configuration(kernel& k, resource_manager& res_man, const std::string& cfg_path)
    : kernel_(k)
    , res_man_(res_man)
    , object_input_manager_(kernel_)
    , scripting_engine_(*this)
    {
        load_core_parsers();
        
        // finally parse the config
        parse_file(cfg_path);
    }

    void configuration::register_object(const boost::shared_ptr<object>& obj)
    {
        LOG("-- [%s] Registering object", obj->name().c_str());
        objects_.push_back(obj);
        
        // call on_create
        obj->on_create();
    }
    
    void configuration::deregister_object(const boost::shared_ptr<object>& obj)
    {
        get_input_manager().deregister_object(obj.get());
        
        LOG("-- [%s] Deregistering object", obj->name().c_str());
        objects_.erase( std::remove(
            objects_.begin(),
            objects_.end(),
                obj),
                    objects_.end());
        
        // call on_destroy
        obj->on_destroy();
    }
    
    void configuration::switch_to(const boost::shared_ptr<scene>& s)
    {
        if(cur_scene_)
        {
            cur_scene_->on_transition_from();
        }
        
        cur_scene_ = s;
        cur_scene_->on_transition_to();
    }
    
    boost::shared_ptr<scene>& configuration::get_current_scene()
    {
        return cur_scene_;
    }
    
    void configuration::load_core_parsers()
    {
        // add x2d core parsers
        parsers_["float"]       = boost::bind(&configuration::parse_float, this, _1, _2);
        parsers_["int"]         = boost::bind(&configuration::parse_int, this, _1, _2);
        parsers_["string"]      = boost::bind(&configuration::parse_string, this, _1, _2);
        parsers_["vector"]      = boost::bind(&configuration::parse_vector, this, _1, _2);
        parsers_["list"]        = boost::bind(&configuration::parse_list, this, _1, _2);
        parsers_["list:string"] = boost::bind(&configuration::parse_list, this, _1, _2);
        parsers_["list:float"]  = boost::bind(&configuration::parse_float_list, this, _1, _2);
        
        parsers_["camera"]      = boost::bind(&configuration::parse_camera, this, _1, _2);
        parsers_["viewport"]    = boost::bind(&configuration::parse_viewport, this, _1, _2);
        parsers_["input"]       = boost::bind(&configuration::parse_input, this, _1, _2);
        parsers_["keyboard"]    = boost::bind(&configuration::parse_keyboard, this, _1, _2);
        parsers_["key"]         = boost::bind(&configuration::parse_key, this, _1, _2);
        
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
        parsers_["emitter"]     = boost::bind(&configuration::parse_emitter, this, _1, _2);
        
        parsers_["object"]      = boost::bind(&configuration::parse_object, this, _1, _2);
        parsers_["scene"]       = boost::bind(&configuration::parse_scene, this, _1, _2);
        parsers_["context"]     = boost::bind(&configuration::parse_context, this, _1, _2);        
        parsers_["zone"]        = boost::bind(&configuration::parse_zone, this, _1, _2);

        parsers_["script"]      = boost::bind(&configuration::parse_script, this, _1, _2);
        parsers_["listener"]    = boost::bind(&configuration::parse_listener, this, _1, _2);
        
        parsers_["body"]        = boost::bind(&configuration::parse_body, this, _1, _2);        
        parsers_["part:box"]    = boost::bind(&configuration::parse_body_part_box, this, _1, _2);
        parsers_["part:circle"] = boost::bind(&configuration::parse_body_part_circle, this, _1, _2);
        parsers_["part:polygon"]= boost::bind(&configuration::parse_body_part_polygon, this, _1, _2);
    }
    
    void configuration::parse_file(const std::string& cfg_path, const std::string& root_key)
    {
        boost::shared_ptr<conf> cfg = res_man_.get<conf>(cfg_path);

        {
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
            parse(root, root_key);
        }
        
        {
            device_capabilities caps;
            
            // now when default is parsed, try to load and override stuff from
            // resolution-perfect version.
            std::stringstream ss;
            
            // check if it's possible to get a resolution-perfect version
            ss << "/__"
            << static_cast<int>(caps.has_retina? caps.display_size.width*2
                                : caps.display_size.width)
            << "x"
            << static_cast<int>(caps.has_retina? caps.display_size.height*2
                                : caps.display_size.height);
            
            // get first path component. That is the name of the virtual disk
            size_t pos = cfg_path.find('/');
            if(pos == std::string::npos)
            {
                throw structure_exception("Path seems to be wrong: '" + cfg_path + "'.");
            }
            
            std::string disk = cfg_path.substr(0, pos);
            std::string rpv = disk + ss.str() + cfg_path.substr(pos);
            LOG("Resolution-perfect version check: '%s'", rpv.c_str());
            
            try
            {
                boost::shared_ptr<conf> rp_cfg = res_man_.get<conf>(rpv);
                xml_doc doc;
                try
                {
                    doc.parse<0>(&(rp_cfg->data.at(0)));
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
                parse(root, root_key);
            }
            catch(std::exception& e)
            {
                // just not found. ignore
            }
        }
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
    
    std::string configuration::lookup_key(const std::string& key, const config_key& base)
    {
        if( key.find('.') == std::string::npos )
        {
            // this is probably a relative path.
            config_key tmp_key = base;
            
            while(true)
            {                    
                std::string kk = tmp_key / key;
                LOG("Check key: '%s'", kk.c_str());
                
                if(exists(kk))
                {
                    LOG("Found key '%s' from '%s'", kk.c_str(), key.c_str());
                    return kk;
                }
                
                if(tmp_key.string().empty())
                {
                    break; // already checked everything
                }
                
                // try deeper
                tmp_key = tmp_key.remove_last_path_component();
            }
        }
        else if(exists(key)) 
        {
            return key; // it's a fully qualified key
        }
        
        return std::string(); // empty means not found
    }
    
    void configuration::load_plugin(const boost::shared_ptr<plugin>& plg)
    {
        plg->load_extensions( parsers_ );
    }
    
    const boost::shared_ptr<object> configuration::create_object(const config_key& key, config_key proto_key)
    {
        boost::shared_ptr<object> obj;
        
        if(proto_key.empty())
        {
            proto_key = key;
        }
        
        // try to find binding first
        if(create_obj_bindings_.find(proto_key) != create_obj_bindings_.end())
        {
            // forward to strong-typed version
            obj = create_obj_bindings_[proto_key]( key );
        }
        else
        {
            // maybe there is a prototype with a binding for this object?
            config_key proto =
                static_cast<object_cfg*>( &(*config_[proto_key]) )->get_traits().proto;
            
            if(!proto.empty())
            {
                return create_object(key, proto);
            }
            
            obj = static_cast<object_cfg*>( &(*config_[key]) )->create();
        }
        
        register_object(obj);
        return obj;
    }
    
    const boost::shared_ptr<object> configuration::create_object_1(
        const config_key& key, spawner* spwn, config_key proto_key)
    {
        boost::shared_ptr<object> obj;
                
        if(proto_key.empty())
        {
            proto_key = key;
        }
        
        // try to find binding first
        if(create_obj_1_bindings_.find(proto_key) != create_obj_1_bindings_.end())
        {
            // forward to strong-typed version
            obj = create_obj_1_bindings_[proto_key]( key, spwn );
        }
        else
        {
            // maybe there is a prototype with a binding for this object?
            config_key proto =
                static_cast<object_cfg*>( &(*config_[proto_key]) )->get_traits().proto;
            
            if(!proto.empty())
            {
                return create_object_1(key, spwn, proto);
            }
            
            obj = static_cast<object_cfg*>( &(*config_[key]) )->create(spwn);
        }
        
        register_object(obj);
        return obj;
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
    boost::shared_ptr<animation> configuration::get_object<animation>(const config_key& key)
    {
        return static_cast<animation_cfg*>( &(*config_[key]) )->get();
    }

    template <>
    const boost::shared_ptr<sprite> configuration::create_sys_object<sprite>(const config_key& key)
    {
        return static_cast<sprite_cfg*>( &(*config_[key]) )->create();
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
    const boost::shared_ptr<emitter> configuration::create_sys_object<emitter>(const config_key& key)
    {
        return static_cast<emitter_cfg*>( &(*config_[key]) )->create();
    }
    
    template <>
    const boost::shared_ptr<script> configuration::create_sys_object<script>(const config_key& key)
    {
        return static_cast<script_cfg*>( &(*config_[key]) )->create();
    }

    template <>
    const boost::shared_ptr<listener> configuration::create_sys_object<listener>(const config_key& key)
    {
        return static_cast<listener_cfg*>( &(*config_[key]) )->create();
    }
    
    template <>
    const boost::shared_ptr<body> configuration::create_sys_object_1<body, 
        object>(const config_key& key, object& obj)
    {
        return static_cast<body_cfg*>( &(*config_[key]) )->create(obj);
    }

    template <>
    const boost::shared_ptr<body_part> configuration::create_sys_object_1<body_part, 
        boost::shared_ptr<body> >(const config_key& key, boost::shared_ptr<body>& b)
    {
        return static_cast<body_part_cfg*>( &(*config_[key]) )->create(b);
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
    
    value<std::string> configuration::get_key_attr(configuration& cfg, xml_node* node, const config_key& key,
                                                   const std::string& name, const std::string& default_value)
    {
        xml_attr* at = node->first_attribute(name.c_str());
        if(at) 
        {
            std::string k = cfg.lookup_key(value_parser<std::string>::parse(at->value()), key);
            return value<std::string>(k);
        }
        else if( cfg.exists(key / name) )
        {
            std::string k = cfg.lookup_key(*cfg.get_value<std::string>(key / name), key);
            return value<std::string>(k);
        }
        
        return value<std::string>(default_value);
    }
    
    
    value<std::string> configuration::get_mandatory_key_attr(configuration& cfg, xml_node* node, const config_key& key,
                                                             const std::string& name, const std::exception& e)
    {
        xml_attr* at = node->first_attribute(name.c_str());
        if(at) 
        {
            std::string k = cfg.lookup_key(value_parser<std::string>::parse(at->value()), key);
            return value<std::string>(k);
        }
        else if( cfg.exists(key / name) )
        {
            std::string k = cfg.lookup_key(*cfg.get_value<std::string>(key / name), key);
            return value<std::string>(k);
        }
        
        throw e;
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
            boost::shared_ptr<sprite> r = create();
            inst_ = r;
            return r;
        }
    }
    
    boost::shared_ptr<sprite> sprite_cfg::create()
    {
        boost::shared_ptr<sprite> r = boost::shared_ptr<sprite>(
            new sprite( config_.get_object<texture>(texture_), origin_, size_, pivot_, flip_x_, flip_y_) );
        
        return r;
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
            boost::shared_ptr<animation> r = create();
            inst_ = r;
            return r;
        }
    }
    
    boost::shared_ptr<animation> animation_cfg::create()
    {
        boost::shared_ptr<animation> r = boost::shared_ptr<animation>( new animation(duration_, pivot_, flip_x_, flip_y_) );
        
        // add all frames
        for(int i=0; i<frames_.size(); ++i)
        {
            r->add( frame( config_.get_object<sprite>( frames_.at(i).sprite_key_), 
                frames_.at(i).duration_>0.0f?frames_.at(i).duration_:duration_ ) );
        }

        return r;
    }    

    boost::shared_ptr<emitter> emitter_cfg::create()
    {
        boost::shared_ptr<emitter> r = boost::shared_ptr<emitter>(
            new emitter(config_.get_kernel(), config_, *static_cast<emitter_settings*>(this) ) );
        return r;
    }    
    
    boost::shared_ptr<spawner> spawner_cfg::create()
    {
        boost::shared_ptr<spawner> r = boost::shared_ptr<spawner>( 
            new spawner(config_.get_kernel(), config_, obj_lst_, position_, wave_size_, wave_delay_) );        
        return r;
    }    

    boost::shared_ptr<script> script_cfg::create()
    {
        boost::shared_ptr<script> r;
        
        if(!path_.empty())
        {
            boost::shared_ptr<ifdstream> stream = config_.get_resman().get<ifdstream>(path_);
            
            long fsize = stream->size();            
            std::string s(fsize, 0);
            
            if (!stream->read(&s[0], fsize))
            {
                throw sys_exception("Couldn't read script from path " + path_);
            }
            
            r = boost::shared_ptr<script>(new script(s) );
        }
        else if(!ref_.empty())
        {
            return config_.create_sys_object<script>(ref_);
        }
        else
        {
            r = boost::shared_ptr<script>(new script(data_) );
        }
        
        return r;
    }

    boost::shared_ptr<listener> listener_cfg::create()
    {
        boost::shared_ptr<listener> r =
            boost::shared_ptr<listener>(new listener(kernel_, config_, event_) );
        
        if(!script_.empty())
        {
            r->set_script(config_.create_sys_object<script>(script_));
        }
        
        return r;
    }
    
    boost::shared_ptr<body> body_cfg::create(object& obj)
    {
        boost::shared_ptr<body> r = boost::shared_ptr<body>( 
            new body(config_.get_kernel(), config_, obj, type_, bullet_,
                     fixed_rotation_, linear_damping_, pivot_) );
        
        // create all parts and add them
        for(int i=0; i<parts_.size(); ++i)
        {
            r->add_part( 
                config_.create_sys_object_1<body_part, 
                        boost::shared_ptr<body> >(parts_.at(i), r) );            
        }
        
        return r;
    }    
    
    boost::shared_ptr<body_part> body_part_cfg::create(const boost::shared_ptr<body>& b)
    {
        if(type_ == BOX_TYPE)
        {
            return
                boost::shared_ptr<body_part>(
                    new body_part_box(config_, ident_, b, density_, restitution_,
                        friction_, mask_, category_, bl_, tr_, is_sensor_) );
        }
        else if(type_ == CIRCLE_TYPE)
        {
            return
                boost::shared_ptr<body_part>(
                    new body_part_circle(config_, ident_, b, density_,
                        restitution_, friction_, mask_, category_,
                            x_, y_, radius_, is_sensor_) );
        }
        else if(type_ == POLYGON_TYPE)
        {
            return
                boost::shared_ptr<body_part>(
                    new body_part_polygon(config_, ident_, b, density_,
                        restitution_, friction_, mask_, category_, points_, is_sensor_) );
        }

        throw config_exception("Can't create BodyPart with unknown type.");
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
            boost::shared_ptr<camera> r;
            
            if(parent_.empty() == false)
            {
                r = boost::shared_ptr<camera>( new camera(frustum_, config_.get_object<camera>(parent_)) );
            }
            else
            {
                r = boost::shared_ptr<camera>( new camera(frustum_, boost::shared_ptr<camera>()) );
            }
            
            r->position(*position_);
            r->zoom(*zoom_);
            r->rotation(*rotation_);

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
                    new font( size_, baseline_, line_height_, scale_width_,
                    scale_height_, stretch_height_, padding_, spacing_ ) );

            // add pages
            for(std::vector<std::string>::iterator it = pages_.begin(); it != pages_.end(); ++it)
            {
                f->add_page(config_.get_object<texture>(*it));
            }

            // copy glyphs and init them
            f->set_glyphs(glyphs_);
            
            // and the kernings
            f->set_kernings(kernings_);

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
        
} // namespace config    
} // namespace x2d