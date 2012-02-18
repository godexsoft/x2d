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
#include "platform.h"
#include "exceptions.h"

#include "rapidxml.hpp"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/random.hpp>

#include "object_support.h"
#include "value_support.h"

namespace x2d {
namespace config {
    
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
    public:
        /**
         * @param[in] k         The kernel
         * @param[in] res_man   The resource manager to use
         * @param[in] cfg_path  Path to the main config file inside res_man
         */
        configuration(kernel& k, resource_manager& res_man, const std::string& cfg_path);
        
        resource_manager& resman() const
        {
            return res_man_;
        }
        
        /**
         * Bind c++ type to configuration key
         * @param[in] key Configuration key
         */
        template <typename T>
        void bind(const config_key& key)
        {
            create_obj_bindings_.insert( 
                std::make_pair(key, boost::bind(&configuration::create_object<T>, this, _1)) );
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
         * Create new object without explicit type specification.
         *
         * Note: internally, x2d will look for a user type binding under the given config key
         * and if found it will instantiate a custom user type instead of the generic object type.
         *
         * @param[in] key Configuration key
         */
        const boost::shared_ptr<object> create_object(const config_key& key);
        
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
        
    private:
        /**
         * Parse given file into this configuration instance.
         * @param[in] cfg_path Path to configuration xml
         */
        void parse_file(const std::string& cfg_path);
        
        /**
         * Parse given xml node and store under given key.
         * Note: this method is called recursively.
         *
         * @param[in] root  Root node to parse from
         * @param[in] key   Configuration key
         */
        void parse(xml_node* root, const config_key& key=config_key(""));

        // getting attribute
        template<typename T>
        value_holder<T> get_attr(xml_node* node, const config_key& key, 
                                 const std::string& name, const T& default_value)
        {
            xml_attr* at = node->first_attribute(name.c_str());
            if(at) 
            {
                return value_holder<T>(key / name, value_parser<T>::parse(at->value()));
            }
            else if(config_.find(key / name) != config_.end() )
            {
                return value_holder<T>(key / name, default_value);
            }
            
            return value_holder<T>("", default_value);
        }

        // getting mandatory attribute
        template<typename T>
        value_holder<T> get_mandatory_attr(xml_node* node, const config_key& key, 
                                           const std::string& name, const std::exception& e)
        {
            xml_attr* at = node->first_attribute(name.c_str());
            if(at) 
            {
                return value_holder<T>(key / name, value_parser<T>::parse(at->value()));
            }
            else if(config_.find(key / name) != config_.end() )
            {
                return value_holder<T>(key / name, T());
            }
            
            throw e;
        }

        // parsers for primitive value types
        void parse_float(xml_node* node, const config_key& key);
        void parse_int(xml_node* node, const config_key& key);
        void parse_string(xml_node* node, const config_key& key);
        void parse_vector(xml_node* node, const config_key& key);
        
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
        
        // objects
        void parse_object(xml_node* node, const config_key& key);
        
        typedef boost::function<void(xml_node*,const config_key&)>              parser_type;        
        typedef boost::function<boost::shared_ptr<object>(const config_key&)>   binding_type;           
        
        kernel&                                 kernel_;
        resource_manager&                       res_man_;
        std::map<config_key, cfg_base_ptr>      config_;
        std::map<std::string, parser_type>      parsers_;

        std::map<config_key, binding_type>     create_obj_bindings_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_CONFIGURATION_H__
