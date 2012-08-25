//
//  configuration.h
//  x2d
//
//  Created by Alex Kremer on 1/30/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CONFIGURATION_H__
#define __X2D_CONFIGURATION_H__

#include "resource_manager.h"
#include "math_util.h"
#include "filesystem.h"
#include "sprite.h"
#include "object.h"
#include "script.h"
#include "listener.h"
#include "platform.h"
#include "exceptions.h"

#include "rapidxml.hpp"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/random.hpp>

#include "object_support.h"
#include "value_support.h"

#include "scripting_engine.h"
#include "object_input.h"

namespace x2d {
namespace config {
    
    class plugin;
    
    /**
     * @brief Simple data blob to read configuration data into.
     */
    struct conf 
    {
        conf(ifdstream stream)
        {            
            data.resize(stream.size());
            stream.read(&data.at(0), stream.size());
        }
        
        std::string data;
    };
    
    /**
     * @brief Main configuration class.
     *
     * This class is the core of x2d configuration system.
     * It's used to bind custom object types, create objects from configuration,
     * get shared objects and parse xml config files.
     */
    class configuration
    {
        friend class plugin;
        
    public:
        /**
         * @param[in] k         The kernel
         * @param[in] res_man   The resource manager to use
         */
        configuration(kernel& k, resource_manager& res_man);
        
        /**
         * @param[in] k         The kernel
         * @param[in] res_man   The resource manager to use
         * @param[in] cfg_path  Path to the main config file inside res_man
         */
        configuration(kernel& k, resource_manager& res_man, const std::string& cfg_path);
        
        
        /*
         * Object support
         */
        void register_object(const boost::shared_ptr<object>& obj);
        void deregister_object(const boost::shared_ptr<object>& obj);
                
        kernel& get_kernel() const
        {
            return kernel_;
        }
        
        resource_manager& get_resman() const
        {
            return res_man_;
        }

        object_input_manager& get_input_manager()
        {
            return object_input_manager_;
        }
        
        scripting_engine& get_scripting_engine()
        {
            return scripting_engine_;
        }
        
        /**
         * Load an x2d plugin.
         */
        void load_plugin(const boost::shared_ptr<plugin>& plg);
        
        /**
         * Parse given file into this configuration instance.
         * @param[in] cfg_path Path to configuration xml
         */
        void parse_file(const std::string& cfg_path, const std::string& root_key="");
        
        /**
         * Bind c++ type to configuration key
         * @param[in] key Configuration key
         */
        template <typename T>
        void bind(const config_key& key)
        {
            create_obj_bindings_.insert(
                std::make_pair(key, boost::bind(&configuration::create_object<T>, this, _1)) );
            
            create_obj_1_bindings_.insert(
                std::make_pair(key, boost::bind(&configuration::create_object_1<T>, this, _1, _2)) );
        }
        
        /**
         * Create new object of requested custom type
         * @param[in] key Configuration key
         */
        template <typename T>
        const boost::shared_ptr<T> create_object(const config_key& key)
        {
            return static_cast<object_cfg*>( &(*config_[key]) )->create<T>();
        }

        /**
         * Create new object of requested custom type relative to spawner
         * @param[in] key Configuration key
         * @param[in] spwn The spawner
         */
        template <typename T>
        const boost::shared_ptr<T> create_object_1(const config_key& key, spawner* spwn)
        {
            return static_cast<object_cfg*>( &(*config_[key]) )->create<T>(spwn);
        }
        
        /**
         * Create new object without explicit type specification.
         *
         * Note: internally, x2d will look for a user type binding under the given config key
         * and if found it will instantiate a custom user type instead of the generic object type.
         *
         * @param[in] key Configuration key
         */
        const boost::shared_ptr<object> create_object(const config_key& key);
        
        /**
         * Create new object relative to spawner.
         *
         * @param[in] key Configuration key
         * @param[in] spwn The spawner
         */
        const boost::shared_ptr<object> create_object_1(const config_key& key, spawner* spwn);
        
        /**
         * Create if required and return a shared version of a given object.
         * @param[in] key Configuration key
         */
        template <typename T>
        boost::shared_ptr<T> get_object(const config_key& key);

        /**
         * Create a system object such as animation.
         */
        template <typename T>
        const boost::shared_ptr<T> create_sys_object(const config_key& key);
        
        /**
         * Create a system object such as body_part with parameter.
         */
        template <typename T, typename A>
        const boost::shared_ptr<T> create_sys_object_1(const config_key& key, A& a);
        
        /**
         * Get a configuration value (metric)
         * @param[in] key Configuration key
         */
        template <typename T>
        T get_value(const config_key& key);
        
        /**
         * Determine whether some value/object exists under given key.
         * @param[in] key The key to examine
         * @return true if exists; false otherwise
         */
        bool exists(const config_key& key)
        {
            return config_.find(key) != config_.end();
        }
        
        /**
         * Lookup a configuration key based on a user-provided key and current base path
         * @param[in] key The key to find
         * @param[in] base Current config path
         * @return The fully qualified configuration key if found or empty string otherwise
         */
        std::string lookup_key(const std::string& key, const config_key& base);
        
        // getting attribute
        template<typename T>
        static
        value_holder<T> get_attr(configuration& cfg, xml_node* node, const config_key& key, 
                                 const std::string& name, const T& default_value)
        {
            xml_attr* at = node->first_attribute(name.c_str());
            if(at) 
            {
                return value_holder<T>(key / name, value_parser<T>::parse(at->value()));
            }
            else if(cfg.config_.find(key / name) != cfg.config_.end() )
            {
                return value_holder<T>(key / name, default_value);
            }
            
            return value_holder<T>("", default_value);
        }
        
        // getting mandatory attribute
        template<typename T>
        static
        value_holder<T> get_mandatory_attr(configuration& cfg, xml_node* node, const config_key& key, 
                                           const std::string& name, const std::exception& e)
        {
            xml_attr* at = node->first_attribute(name.c_str());
            if(at) 
            {
                return value_holder<T>(key / name, value_parser<T>::parse(at->value()));
            }
            else if(cfg.config_.find(key / name) != cfg.config_.end() )
            {
                return value_holder<T>(key / name, T());
            }
            
            throw e;
        }

        // getting key attribute
        static
        value_holder<std::string> get_key_attr(configuration& cfg, xml_node* node, const config_key& key, 
                                               const std::string& name, const std::string& default_value);
        
        // getting mandatory key attribute
        static
        value_holder<std::string> get_mandatory_key_attr(configuration& cfg, xml_node* node, const config_key& key, 
                                                         const std::string& name, const std::exception& e);
        
        // getting a list attribute
        template<typename T>
        static
        value_holder<std::vector<T> > get_list_attr(configuration& cfg, xml_node* node, const config_key& key, 
                                                    const std::string& name, const std::string& default_value)
        {
            typedef value_holder<std::vector<T> > holder_type;
            
            xml_attr* at = node->first_attribute(name.c_str());
            if(at) 
            {
                return holder_type(key / name, value_parser<std::vector<T> >::parse(at->value()));
            }
            else if(cfg.config_.find(key / name) != cfg.config_.end() )
            {
                return holder_type(key / name, value_parser<std::vector<T> >::parse(default_value));
            }
            
            return holder_type("", value_parser<std::vector<T> >::parse(default_value));
        }

        // getting a mandatory list attribute
        template<typename T>
        static
        value_holder<std::vector<T> > get_mandatory_list_attr(configuration& cfg, xml_node* node, const config_key& key, 
                                                              const std::string& name, const std::exception& e)
        {
            typedef value_holder<std::vector<T> > holder_type;
            
            xml_attr* at = node->first_attribute(name.c_str());
            if(at) 
            {
                return holder_type(key / name, value_parser<std::vector<T> >::parse(at->value()));
            }
            else if(cfg.config_.find(key / name) != cfg.config_.end() )
            {
                return holder_type(key / name, std::vector<T>());
            }
                                
            throw e;
        }
        
    private:

        /**
         * Loads x2d default parsers.
         */
        void load_core_parsers();
                
        /**
         * Parse given xml node and store under given key.
         * Note: this method is called recursively.
         *
         * @param[in] root  Root node to parse from
         * @param[in] key   Configuration key
         */
        void parse(xml_node* root, const config_key& key=config_key(""));

        // parsers for primitive value types
        void parse_float(xml_node* node, const config_key& key);
        void parse_int(xml_node* node, const config_key& key);
        void parse_string(xml_node* node, const config_key& key);
        void parse_vector(xml_node* node, const config_key& key);
        void parse_list(xml_node* node, const config_key& key);
        
        template<typename T>
        void parse_random(xml_node* node, const config_key& key);
        
        // kernel settings
        void parse_camera(xml_node* node, const config_key& key);
        void parse_viewport(xml_node* node, const config_key& key);
        void parse_input(xml_node* node, const config_key& key);
        
        // various parsers for each supported element type
        void parse_namespace(xml_node* node, const config_key& key);
        void parse_include(xml_node* node, const config_key& key);
        void parse_texture(xml_node* node, const config_key& key);
        void parse_sprite(xml_node* node, const config_key& key);
        void parse_animation(xml_node* node, const config_key& key);
        void parse_frame(xml_node* node, const config_key& key);
        void parse_font(xml_node* node, const config_key& key);
        void parse_music(xml_node* node, const config_key& key);
        void parse_sfx(xml_node* node, const config_key& key);
        void parse_spawner(xml_node* node, const config_key& key);        
        
        // object system
        void parse_context(xml_node* node, const config_key& key);
        void parse_zone(xml_node* node, const config_key& key);
        void parse_object(xml_node* node, const config_key& key);

        // physics
        void parse_body(xml_node* node, const config_key& key);
        boost::shared_ptr<body_part_cfg> parse_body_part_common(xml_node* node, const config_key& key);
        void parse_body_part_box(xml_node* node, const config_key& key);
        void parse_body_part_circle(xml_node* node, const config_key& key);
        
        // lua scripting
        void parse_script(xml_node* node, const config_key& key);
        
        // event system
        void parse_listener(xml_node* node, const config_key& key);
        
        typedef boost::function<void(xml_node*,const config_key&)>              parser_type;        
        typedef boost::function<boost::shared_ptr<object>(const config_key&)>             binding_type;
        typedef boost::function<boost::shared_ptr<object>(const config_key&, spawner*)>   binding_1_type;
        
        kernel&                                 kernel_;
        resource_manager&                       res_man_;
        
        std::map<config_key, cfg_base_ptr>      config_;
        std::map<std::string, parser_type>      parsers_;

        std::map<config_key, binding_type>      create_obj_bindings_;
        std::map<config_key, binding_1_type>    create_obj_1_bindings_;
        
        std::deque< boost::shared_ptr<object> > objects_;
        
        object_input_manager                    object_input_manager_;
        scripting_engine                        scripting_engine_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_CONFIGURATION_H__
