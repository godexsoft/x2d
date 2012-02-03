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
#include "platform.h"

#include "rapidxml.hpp"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/random.hpp>

#include "object_support.h"
#include "value_support.h"

namespace x2d {
namespace config {
    
    struct conf 
    {
        conf(ifdstream stream)
        {            
            data.resize(stream.size());
            stream.read(&data.at(0), stream.size());
        }
        
        std::string data;
    };
    
    class configuration
    {
    public:
        configuration(kernel& k, resource_manager& res_man, const std::string& cfg_path);
        
        template <typename T>
        boost::shared_ptr<T> get_object(const config_key& key);

        template <typename T>
        T get_value(const config_key& key);
        
    private:
        void parse_file(const std::string& cfg_path);
        void parse(xml_node* root, const config_key& key=config_key(""));

        // parsers for primitive value types
        void parse_float(xml_node* node, const config_key& key);
        void parse_int(xml_node* node, const config_key& key);
        
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
        
        typedef boost::function<void(xml_node*,const config_key&)> parser_type;
        
        kernel&                                 kernel_;
        resource_manager&                       res_man_;
        std::map<config_key, cfg_base_ptr>      config_;
        std::map<std::string, parser_type>      parsers_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_CONFIGURATION_H__
