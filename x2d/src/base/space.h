//
//  space.h
//  x2d
//
//  Created by Alex Kremer on 2/5/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_SPACE_H__
#define __X2D_SPACE_H__

#include "log.h"

namespace x2d {
namespace base {

    /**
     * @brief Space. World, Camera or Screen
     */
    enum space
    {       
        WORLD_SPACE  = 1,
        SCREEN_SPACE = 2,
        CAMERA_SPACE = 3        
    };
    
    enum parent_space
    {
        PARENT_SPACE_POSITION = 1,
        PARENT_SPACE_BOX = 2,
        PARENT_SPACE_BOTH = 3,
        PARENT_SPACE_NONE = 4
    };
    
    /**
     * @brief This class holds all the spatial properties of an Object
     */
    /*
    class spatial_info
    {
    public:
        spatial_info();
        
        const glm::mat4 final_transform() const;
        
        const glm::vec3 position() const;
        void position(const glm::vec3& p);
        void position(const glm::vec2& p);
        
        const float rotation() const;
        void rotation(float a);
        
        const float scale() const;
        void scale(float s);
        
        void pivot(const glm::vec2& p);
        const glm::vec2 pivot() const;
        
        void box(const size& s);
        const size box() const;
        
    private:
        space           space_;
        parent_space    parent_space_;
        
        glm::vec3   position_;
        glm::vec3   camera_space_position_;
        
        float       scale_;
        float       rotation_;
        
        size        box_;
        size        camera_space_box_;
        
        glm::vec2   pivot_;
        glm::vec2   camera_space_pivot_;
    };
*/    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_SPACE_H__
