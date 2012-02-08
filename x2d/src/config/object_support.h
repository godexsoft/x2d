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
#include "input_manager.h"

#include "rapidxml.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace x2d {
namespace config {
        
    namespace rx = rapidxml;
    
    typedef rx::xml_attribute<char> xml_attr;
    typedef rx::xml_node<char>      xml_node;
    typedef rx::xml_document<char>  xml_doc;	
    
    /**
     * Configuration key is a string.
     * Every component is separated by a dot.
     */
    typedef basic_path<char, '.'>   config_key;
    
    /**
     * @brief Base config class.     
     * Used with boost::shared_ptr to allow placing child classes into same stl container.
     */
    class cfg_base
    {        
    public:
        cfg_base()
        {            
        }
    };
    
    typedef boost::shared_ptr<cfg_base> cfg_base_ptr;
    
    /**
     * @brief Support for '<texture>' configuration node.
     */ 
    class texture_cfg
    : public cfg_base
    { 
    public:
        /**
         * @param[in] res_man The resource manager
         * @param[in] p       Path to texture inside the resource manager
         */
        texture_cfg(resource_manager& res_man, const std::string& p)
        : res_man_(res_man)
        , path_(p)
        {
        }
        
        /**
         * Create if required and return a shared version of the texture.
         */
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
        resource_manager&         res_man_;
        std::string               path_;
        boost::weak_ptr<texture>  inst_;
    };
    
    /**
     * @brief Support for '<sprite>' configuration node.
     */ 
    class sprite_cfg
    : public cfg_base
    {
    public:        
        /**
         * @param[in] t       Texture to use
         * @param[in] p       Offset inside texture
         * @param[in] s       Size of the sprite
         */
        sprite_cfg(configuration& c,
                   const config_key& t, 
                   const point& p, 
                   const size& s)
        : config_(c)
        , texture_(t)
        , origin_(p)
        , size_(s)
        {            
        }
        
        /**
         * Create if required and return a shared version of the sprite.
         */
        boost::shared_ptr<sprite> get();
        
    private:
        configuration&           config_;
        config_key               texture_;
        point                    origin_;
        size                     size_;        
        boost::weak_ptr<sprite>  inst_;
    };
    
    class animation_cfg;
    
    /**
     * @brief Support for '<frame>' configuration node.
     * This class describes one frame of the animation.
     * @see animation_cfg
     */ 
    class frame_cfg
    {
        friend class animation_cfg;
        
    public:        
        /**
         * @param[in] spr   Sprite to use
         * @param[in] d     Duration for this frame
         */
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
    
    /**
     * @brief Support for '<animation>' configuration node.
     * @see frame_cfg
     */ 
    class animation_cfg
    : public cfg_base
    {
    public:  
        /**
         * @param[in] cfg     Configuration object
         * @param[in] d       Default duration for every frame in the animation
         */
        animation_cfg(configuration& cfg)
        : config_(cfg)
        , duration_(0.0f)
        {            
        }
        
        /**
         * Used to add a frame to the animation.
         * @param[in] f     The frame config
         * @see frame_cfg
         */
        void add(const frame_cfg& f)
        {
            frames_.push_back(f);
        }
        
        /**
         * Sets default duration.
         * @param[in] d The duration
         */
        void set_duration(float d)
        {
            duration_ = d;
        }
        
        /**
         * Create if required and return a shared version of the animation.
         */
        boost::shared_ptr<animation> get();

        /**
         * Create a new animation instance and return it without saving a local copy.
         */
        boost::shared_ptr<animation> create();        
        
    private:
        configuration&              config_;
        float                       duration_;
        std::vector<frame_cfg>      frames_;
        boost::weak_ptr<animation>  inst_;
    };
    
    /**
     * @brief Support for '<camera>' configuration node.
     * @see viewport_cfg
     */ 
    class camera_cfg
    : public cfg_base
    {
    public:    
        /**
         * @param[in] c       The configuration
         * @param[in] f       Camera frustum
         * @param[in] rot     Rotation expressed in degrees
         * @param[in] zm      Zoom level (scale)
         * @param[in] pos     Position of the camera (given in world space)
         */
        camera_cfg(configuration& c, const size& f, const value_holder<float>& rot, 
                   const value_holder<float>& zm, const value_holder<vector_2d>& pos)
        : config_(c)
        , frustum_(f)
        , rotation_(rot)
        , zoom_(zm)
        , position_(pos)
        {            
        }
         
        /**
         * Create if required and return a shared version of the camera.
         */
        boost::shared_ptr<camera> get();
        
    private:
        configuration&           config_;
        size                     frustum_;
        value_holder<float>      rotation_;
        value_holder<float>      zoom_;
        value_holder<vector_2d>  position_;
        boost::weak_ptr<camera>  inst_;
    };
    
    /**
     * @brief Support for '<viewport>' configuration node.
     * @see camera_cfg
     */ 
    class viewport_cfg
    : public cfg_base
    {
    public:  
        /**
         * @param[in] cfg     Configuration object
         * @param[in] b       Viewport rectangle
         * @param[in] cam_key Camera configuration key (name) to use
         * @param[in] ci      Background color information
         */
        viewport_cfg(configuration& cfg, const rect& b, 
                     const std::string& cam_key, 
                     const color_info& ci)
        : config_(cfg)
        , box_(b)
        , camera_key_(cam_key)
        , bg_color_(ci)
        {            
        }
        
        /**
         * Create if required and return a shared version of the viewport.
         */
        boost::shared_ptr<viewport> get();
        
    private:
        configuration&             config_;
        rect                       box_;
        std::string                camera_key_;
        color_info                 bg_color_;
        boost::weak_ptr<viewport>  inst_;
    };

    /**
     * @brief Support for '<input>' configuration node.
     */ 
    class input_cfg
    : public cfg_base
    {
    public:      
        /**
         * @param[in] k             Kernel object
         * @param[in] want_touch    true if touch input required; false otherwise
         * @param[in] want_accel    true if accelerometer input required; false otherwise
         */
        input_cfg(kernel& k, bool want_touch, bool want_accel)
        : kernel_(k)
        , want_touch_(want_touch)
        , want_accel_(want_accel)
        {            
        }
        
        /**
         * Create if required and return a shared version of the input manager.
         */
        boost::shared_ptr<input_manager> get()
        {
            if( boost::shared_ptr<input_manager> p = inst_.lock() )
            {            
                // already exists outside of cfg
                return p;
            }
            else
            {
                boost::shared_ptr<input_manager> r = 
                    boost::shared_ptr<input_manager>( 
                        new input_manager(kernel_, want_touch_, want_accel_) );
                inst_ = r;
                return r;
            }
        }
        
    private:
        kernel&                         kernel_;
        bool                            want_touch_;
        bool                            want_accel_;
        boost::weak_ptr<input_manager>  inst_;
    };
    
    /**
     * @brief Support for '<object>' configuration node.
     */ 
    class object_cfg
    : public cfg_base
    {
    public:        
        /**
         * @param[in] c         Configuration object
         * @param[in] k         Kernel object
         * @param[in] t         Object traits
         */
        object_cfg(configuration& c, kernel& k, const object_traits& t)
        : config_(c)
        , kernel_(k)
        , traits_(t)
        {        
        }
        
        /**
         * Create a new custom type object and return a shared_ptr containing it
         */
        template <typename T>
        const boost::shared_ptr<T> create()
        {
            boost::shared_ptr<T> p = boost::shared_ptr<T>( new T(kernel_, config_, traits_) );            
            add_children(p);
            
            if(traits_.obj_space == CAMERA_SPACE)
            {
                kernel_.add_camera_space_object( p );                
            }
            
            return p;
        }
        
        /**
         * Create a new object and return a shared_ptr containing it.
         */
        const boost::shared_ptr<object> create()
        {
            boost::shared_ptr<object> p = boost::shared_ptr<object>( new object(kernel_, config_, traits_) );
            add_children(p);
                        
            if(traits_.obj_space == CAMERA_SPACE)
            {
                kernel_.add_camera_space_object( p );                
            }
            
            return p;
        }
                
        /**
         * Add a child object.
         * @param[in] k Configuration key of child object
         */
        void add(const config_key& k)
        {
            children_.push_back(k);
        }
        
    private:        
        /**
         * Create and add children to object
         */
        void add_children(const boost::shared_ptr<object>& p);
        
        configuration&  config_;
        kernel&         kernel_;
        object_traits   traits_;
        
        std::vector<config_key> children_;
    };

    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_OBJECT_SUPPORT_H__
