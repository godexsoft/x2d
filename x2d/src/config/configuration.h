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

#include "rapidxml.hpp"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace x2d {
namespace config {
    
    namespace rx = rapidxml;
    
    typedef rx::xml_attribute<char> xml_attr;
    typedef rx::xml_node<char>      xml_node;
    typedef rx::xml_document<char>  xml_doc;	
    
    typedef basic_path<char, '.'>   config_key;
    
    class cfg_base
    {        
    public:
        cfg_base(resource_manager& res_man)
        : res_man_(res_man)
        {            
        }
        
    protected:
        resource_manager& res_man_;
    };

    typedef boost::shared_ptr<cfg_base> cfg_base_ptr;
    
    class texture_cfg
    : public cfg_base
    { 
    public:
        texture_cfg(resource_manager& res_man, const std::string& p)
        : cfg_base(res_man)
        , path_(p)
        {
        }
        
        boost::shared_ptr<texture> get()
        {    
            if( boost::shared_ptr<texture> p = inst_.lock() )
            {            
                // already exists outside of cfg
                return p;
            }
            else
            {
                boost::shared_ptr<texture> r = boost::shared_ptr<texture>( res_man_.get<texture>(path_) );
                inst_ = r;
                return r;
            }
        }
        
    private:        
        std::string               path_;
        boost::weak_ptr<texture>  inst_;
    };
    
    class sprite_cfg
    : public cfg_base
    {
    public:        
        sprite_cfg(resource_manager& res_man, 
                   texture_cfg& t, 
                   const point& p, const size& s)
        : cfg_base(res_man)
        , texture_(t)
        , origin_(p)
        , size_(s)
        {            
        }
                
        boost::shared_ptr<sprite> get()
        {
            if( boost::shared_ptr<sprite> p = inst_.lock() )
            {            
                // already exists outside of cfg
                return p;
            }
            else
            {
                boost::shared_ptr<sprite> r = boost::shared_ptr<sprite>( new sprite(texture_.get(), origin_, size_) );
                inst_ = r;
                return r;
            }
        }

    private:
        texture_cfg&             texture_;
        point                    origin_;
        size                     size_;        
        boost::weak_ptr<sprite>  inst_;
    };
    
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
        configuration(resource_manager& res_man, const std::string& cfg_path);
        
        template <typename T>
        boost::shared_ptr<T> get(const config_key& key);
        
    private:
        void parse_file(const std::string& cfg_path);
        void parse(xml_node* root, const config_key& key=config_key(""));
        
        // various parsers for each supported element type
        void parse_include(xml_node* node, const config_key& key);
        void parse_texture(xml_node* node, const config_key& key);
        void parse_sprite(xml_node* node, const config_key& key);
        
        typedef boost::function<void(xml_node*,const config_key&)> parser_type;
        
        resource_manager&                       res_man_;
        std::map<config_key, cfg_base_ptr>      config_;
        std::map<std::string, parser_type>      parsers_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_CONFIGURATION_H__
