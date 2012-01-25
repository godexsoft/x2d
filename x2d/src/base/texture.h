//
//  texture.h
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_TEXTURE_H__
#define __X2D_TEXTURE_H__

#include "math_util.h"

#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_3_0
#include <OpenGLES/ES1/glext.h>
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#endif

namespace x2d {
namespace base {

    class texture
    {
    protected:
        typedef enum 
        {
            AUTO = 0,
            RGBA8888,
            RGB565,
            A8,
        } pixel_format;
        
        void init_texture(const void* data, pixel_format pf, unsigned int width, unsigned int height, const struct size& content_size);
        
    private:
        GLuint              name_;
        size                size_;
        unsigned int        width_, height_;
        pixel_format        format_;
        GLfloat             max_s_, max_t_;
        
    public:
        texture()
        : name_(0)
        , size_(0,0)
        , width_(0)
        , height_(0)
        , format_(AUTO)
        , max_s_(0)
        , max_t_(0)
        {
        }
        
        ~texture();
        
        virtual inline size size() const { return size_; }
        
        virtual void draw_in_rect(rect box) const;
        virtual void draw_at_point(point pos) const;
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_TEXTURE_H__
