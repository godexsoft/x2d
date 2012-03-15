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
#include "font.h"
#include "sound_resource.h"
#include "camera.h"
#include "viewport.h"
#include "input_manager.h"
#include "context.h"
#include "zone.h"

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
         * @param[in] pivot   The pivot
         * @param[in] flip_x  Sets flipping on x axis
         * @param[in] flip_y  Sets flipping on x axis         
         */
        sprite_cfg(configuration& c,
                   const config_key& t, 
                   const point& p, 
                   const size& s,
                   const glm::vec2& pivot=glm::vec2(0,0),
                   bool flip_x=false,
                   bool flip_y=false
        )
        : config_(c)
        , texture_(t)
        , origin_(p)
        , size_(s)
        , pivot_(pivot)
        , flip_x_(flip_x)
        , flip_y_(flip_y)
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
        glm::vec2                pivot_;
        bool                     flip_x_;
        bool                     flip_y_;
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
         */
        animation_cfg(configuration& cfg)
        : config_(cfg)
        , duration_(0.0f)
        , pivot_(glm::vec2(0,0))
        , flip_x_(false)
        , flip_y_(false)
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
         * Sets the pivot.
         * @param[in] p The pivot
         */
        void set_pivot(const glm::vec2& p)
        {
            pivot_ = p;
        }
        
        /**
         * Sets x flipping.
         * @param[in] f true or false
         */
        void set_flip_x(bool f)
        {
            flip_x_ = f;
        }

        /**
         * Sets y flipping.
         * @param[in] f true or false
         */
        void set_flip_y(bool f)
        {
            flip_y_ = f;
        }
        
        /**
         * Create a new animation instance and return it without saving a local copy.
         */
        boost::shared_ptr<animation> create();        
        
    private:
        configuration&              config_;
        float                       duration_;
        glm::vec2                   pivot_;
        bool                        flip_x_;
        bool                        flip_y_;
        std::vector<frame_cfg>      frames_;
        boost::weak_ptr<animation>  inst_;
    };
    
    /**
     * @brief Support for '<font>' configuration node.
     */ 
    class font_cfg
    : public cfg_base
    {
    public:  
        /**
         * @param[in] cfg     Configuration object
         * @param[in] h       Height of the font
         * @param[in] s       Spacing on x and y
         * @param[in] tk      Texture key
         * @param[in] ch      Characters supported by this font         
         * @param[in] wid     Width of each character
         */
        font_cfg(configuration& cfg,
                 int h, const size& s,
                 const std::string& tk,
                 const std::string& ch,
                 const std::vector<int>& wid)
        : config_(cfg)
        , height_(h)
        , spacing_(s)        
        , texture_(tk)
        , characters_(ch)
        , widths_(wid)
        {            
        }
        
        /**
         * Create if required and return a shared version of the font.
         */
        boost::shared_ptr<font> get();
        
    private:
        configuration&             config_;
        int                        height_;
        size                       spacing_;
        std::string                texture_;
        std::string                characters_;
        std::vector<int>           widths_;
        boost::weak_ptr<font>      inst_;
    };
    
    /**
     * @brief Support for '<music>' configuration node.
     */ 
    class music_cfg
    : public cfg_base
    {
    public:  
        /**
         * @param[in] rm     Resource manager
         * @param[in] k      The kernel
         * @param[in] path   Path to resource
         * @param[in] loop   Loop the music? true or false
         * @param[in] gain   Volume level
         */
        music_cfg(resource_manager& rm, kernel& k, const std::string& path, 
                  bool loop, float gain, float fade_in, float fade_out,
                  float start, float end)
        : res_man_(rm)
        , kernel_(k)
        , path_(path)
        , loop_(loop)
        , gain_(gain)
        , fade_in_(fade_in)
        , fade_out_(fade_out)
        , start_(start)
        , end_(end)
        {                        
        }
        
        /**
         * Create if required and return a shared version of the music.
         */
        boost::shared_ptr<music> get()
        {
            if( boost::shared_ptr<music> p = inst_.lock() )
            {            
                // already exists outside of cfg
                return p;
            }
            else
            {
                boost::shared_ptr<music> r = 
                    boost::shared_ptr<music>( new music(kernel_, res_man_.get<ifdstream>(path_), 
                        loop_, gain_, fade_in_, fade_out_, start_, end_) );
                inst_ = r;
                return r;
            }
        }
        
    private:
        resource_manager&       res_man_;
        kernel&                 kernel_;
        const std::string       path_;
        bool                    loop_;
        float                   gain_;
        float                   fade_in_;
        float                   fade_out_;
        float                   start_;
        float                   end_;
        
        boost::weak_ptr<music>  inst_;
    };

    /**
     * @brief Support for '<sfx>' configuration node.
     */ 
    class sfx_cfg
    : public cfg_base
    {
    public:  
        /**
         * @param[in] conf   The configuration
         * @param[in] path   Path to resource
         * @param[in] loop   Loop the sfx? true or false
         * @param[in] pitch  Pitch for the sfx
         */
        sfx_cfg(configuration& conf, const std::string& path, 
                bool loop, const value_holder<float>& pitch)
        : config_(conf)
        , path_(path)
        , loop_(loop)
        , pitch_(pitch)
        {                        
        }
        
        /**
         * Create if required and return a shared version of the sfx.
         */
        boost::shared_ptr<sfx> get();
        
    private:
        configuration&             config_;
        const std::string          path_;
        bool                       loop_;
        value_holder<float>        pitch_;
        
        boost::weak_ptr<sfx>       inst_;
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
                   const value_holder<float>& zm, const value_holder<glm::vec2>& pos)
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
        value_holder<glm::vec2>  position_;
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
     * @brief Support for '<spawner>' configuration node.
     */ 
    class spawner_cfg
    : public cfg_base
    {
    public:        
        
        spawner_cfg(configuration& cfg, const value_holder<std::vector<std::string> >& objects,
                    const glm::vec3& pos, int wave_size, float wave_delay)
        : config_(cfg)
        , obj_lst_(objects)
        , position_(pos)
        , wave_size_(wave_size)
        , wave_delay_(wave_delay)
        {            
        }
        
        /**
         * Create a new spawner instance and return it without saving a local copy.
         */
        boost::shared_ptr<spawner> create();        
        
    private:
        configuration&                          config_;
        value_holder<std::vector<std::string> > obj_lst_;
        glm::vec3                               position_;
        int                                     wave_size_;
        float                                   wave_delay_;
    };
    
    /**
     * @brief Support for '<context>' configuration node.
     */ 
    class context_cfg
    : public cfg_base
    {
    public:        
        
        /**
         * Create if required and return a shared version of the context.
         */
        boost::shared_ptr<context> get()
        {
            if( boost::shared_ptr<context> p = inst_.lock() )
            {            
                // already exists outside of cfg
                return p;
            }
            else
            {
                boost::shared_ptr<context> r = boost::shared_ptr<context>( new context() );
                inst_ = r;
                return r;
            }
        }
        
    private:
        boost::weak_ptr<context>  inst_;        
    };

    
    /**
     * @brief Support for '<zone>' configuration node.
     */ 
    class zone_cfg
    : public cfg_base
    {
    public:        
        
        zone_cfg(configuration& cfg, const rect& box, const std::vector<std::string>& ctx)
        : config_(cfg)
        , box_(box)
        , ctx_lst_(ctx)
        {            
        }
        
        /**
         * Create if required and return a shared version of the zone.
         */
        boost::shared_ptr<zone> get();
        
    private:
        boost::weak_ptr<zone>       inst_;        
        
        configuration&              config_;
        rect                        box_; 
        std::vector<std::string>    ctx_lst_;
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
            
            return p;
        }
        
        /**
         * Create a new object and return a shared_ptr containing it.
         */
        const boost::shared_ptr<object> create()
        {
            boost::shared_ptr<object> p = boost::shared_ptr<object>( new object(kernel_, config_, traits_) );
            add_children(p);
            
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
        
        configuration&              config_;
        kernel&                     kernel_;
        object_traits               traits_;
        
        std::vector<config_key>     children_;
    };

    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_OBJECT_SUPPORT_H__
