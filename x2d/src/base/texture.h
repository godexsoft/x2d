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
#define MAX_TEXTURE_SIZE 1024
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#define MAX_TEXTURE_SIZE 2048
#endif

#include "filesystem.h"

namespace x2d {
namespace base {
    
    class sprite; // see sprite.h

    class texture
    {
        friend class sprite;
        
    protected:
        void init_texture(const unsigned char* data, int sz);
        
    private:
        GLuint              name_;
        size                size_;
        unsigned long       width_, height_;
        GLfloat             max_s_, max_t_;
        
    public:
        texture()
        : name_(0)
        , size_(0,0)
        , width_(0)
        , height_(0)
        , max_s_(0)
        , max_t_(0)
        {
        }
        
        texture(ifdstream stream);
        
        ~texture();
        
        virtual inline size size() const { return size_; }
        
        virtual void draw_in_rect(rect box) const;
        virtual void draw_at_point(point pos) const;
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_TEXTURE_H__
