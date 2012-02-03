//
//  object_support.h
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_OBJECT_SUPPORT_H__
#define __X2D_OBJECT_SUPPORT_H__

#include "resource_manager.h"
#include "math_util.h"
#include "filesystem.h"
#include "sprite.h"
#include "animation.h"
#include "camera.h"
#include "viewport.h"

#include "rapidxml.hpp"
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
    
    class animation_cfg;
    
    class frame_cfg
    {
        friend class animation_cfg;
        
    public:        
        frame_cfg(const std::string& spr, const float& d=0.0f)
        : sprite_key_(spr)
        , duration_(d) // 0.0 or negative means use animation's default
        {            
        }
        
    private:
        std::string                 sprite_key_;
        float                       duration_;
    };
    
    class configuration;
    
    class animation_cfg
    : public cfg_base
    {
    public:        
        animation_cfg(resource_manager& res_man, configuration& cfg, const float& d)
        : cfg_base(res_man)
        , config_(cfg)
        , duration_(d)
        {            
        }
        
        void add(const frame_cfg& f)
        {
            frames_.push_back(f);
        }
        
        boost::shared_ptr<animation> get();
        
    private:
        configuration&              config_;
        float                       duration_;
        std::vector<frame_cfg>      frames_;
        boost::weak_ptr<animation>  inst_;
    };
    
    class camera_cfg
    : public cfg_base
    {
    public:        
        camera_cfg(resource_manager& res_man, const size& f, 
                   float rot, float zm, const vector_2d& pos)
        : cfg_base(res_man)
        , frustum_(f)
        , rotation_(rot)
        , zoom_(zm)
        , position_(pos)
        {            
        }
                
        boost::shared_ptr<camera> get()
        {
            if( boost::shared_ptr<camera> p = inst_.lock() )
            {            
                // already exists outside of cfg
                return p;
            }
            else
            {
                boost::shared_ptr<camera> r = boost::shared_ptr<camera>( new camera(frustum_) );
                r->position(position_);                
                r->zoom(zoom_);
                r->rotation(rotation_);
                inst_ = r;
                return r;
            }
        }
        
    private:
        size                     frustum_;
        float                    rotation_;
        float                    zoom_;
        vector_2d                position_;
        boost::weak_ptr<camera>  inst_;
    };
    
    class viewport_cfg
    : public cfg_base
    {
    public:        
        viewport_cfg(resource_manager& res_man, configuration& cfg, 
                     const rect& b, const std::string& cam_key, 
                     const color_info& ci)
        : cfg_base(res_man)
        , config_(cfg)
        , box_(b)
        , camera_key_(cam_key)
        , bg_color_(ci)
        {            
        }
        
        boost::shared_ptr<viewport> get();
        
    private:
        configuration&             config_;
        rect                       box_;
        std::string                camera_key_;
        color_info                 bg_color_;
        boost::weak_ptr<viewport>  inst_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_OBJECT_SUPPORT_H__
