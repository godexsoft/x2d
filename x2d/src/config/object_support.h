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
#include "body.h"
#include "body_part.h"
#include "scene.h"

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
    
    #include "elements/texture.incl"    
    #include "elements/sprite.incl"
    #include "elements/frame.incl"
    #include "elements/animation.incl"
    #include "elements/font.incl"
    #include "elements/sound.incl"
    #include "elements/camera.incl"
    #include "elements/viewport.incl"
    #include "elements/input.incl"
    #include "elements/spawner.incl"
    #include "elements/context.incl"
    #include "elements/zone.incl"
    #include "elements/physics.incl"
    #include "elements/script.incl"
    #include "elements/event.incl"
    #include "elements/object.incl"
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_OBJECT_SUPPORT_H__
