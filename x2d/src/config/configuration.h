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

namespace x2d {
namespace config {
    
    struct cfg_base
    {        
    };

    typedef boost::shared_ptr<cfg_base> cfg_base_ptr;
    
    struct texture_cfg
    : public cfg_base
    { 
        texture_cfg(const std::string& p)
        : path_(p)
        {
        }
        
        std::string path_;
    };
    
    struct sprite_cfg
    : public cfg_base
    {
        sprite_cfg(const std::string& t, const point& p, const size& s)
        : texture_(t)
        , origin_(p)
        , size_(s)
        {            
        }
        
        std::string texture_;
        point       origin_;
        size        size_;        
    };
    
    class configuration
    {
    public:
        configuration(resource_manager& res_man, const std::string& cfg_path);
        
        template <typename T>
        resource_ptr<T> get(const std::string& key);
        
    private:
        resource_manager&                       res_man_;
        std::map<std::string, cfg_base_ptr>     config_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_CONFIGURATION_H__
