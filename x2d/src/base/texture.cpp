//
//  texture.cpp
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "texture.h"
#include "graphics_engine.h"
#include "log.h"

#include <vector>
#include "picopng.h"

namespace x2d {
namespace base {
          
    texture::texture(ifdstream stream)
    : name_(0)
    , size_(0,0)
    , width_(0)
    , height_(0)
    , max_s_(0)
    , max_t_(0)
    {
        LOG("Create a texture");
        
        std::vector<unsigned char> dat;
        dat.resize(stream.size());
        
        stream.read((char*)(&dat.at(0)), stream.size());
        init_texture( const_cast<const unsigned char*>(&dat.at(0)), dat.size() );        
    }
    
    texture::~texture()
    {
        if(name_) 
        {
            glDeleteTextures(1, &name_);
        }
    }
    
    void texture::init_texture(const unsigned char* data, int sz)
    {
        LOG("Texture initialization..");        
        std::vector<unsigned char> out;

        decodePNG(out, width_, height_, data, sz);
        x2d::math::size image_size(width_, height_);

        LOG("Decoded texture from png file. Size is %dx%d", width_, height_);
        
        unsigned int width = width_;
        unsigned int height = height_;
        bool size_to_fit = false;
        unsigned int i;
        
        if((width != 1) && (width & (width - 1))) {
            i = 1;
            while((size_to_fit ? 2 * i : i) < width)
                i *= 2;
            width = i;
        }
        height = image_size.height;
        if((height != 1) && (height & (height - 1))) {
            i = 1;
            while((size_to_fit ? 2 * i : i) < height)
                i *= 2;
            height = i;
        }
        while((width > MAX_TEXTURE_SIZE) || (height > MAX_TEXTURE_SIZE)) {
            width /= 2;
            height /= 2;
            image_size.width *= 0.5;
            image_size.height *= 0.5;
        }
        
        // Now we need a new buffer to make it power of two
        std::vector<unsigned char>pot_data(width*height*4, 0);
        
        // Copy the image data; FIXME: not optimal
        int ww = width_*4;
        int ww2 = width*4;
        for(int i=0; i<height_; ++i) {
            for(int j=0; j<ww; j+=4) {
                std::copy(out.begin()+(i*ww+j), out.begin()+(i*ww+j+4), pot_data.begin()+(i*ww2+j));
            }
        }
       
        LOG("Init OpenGL texture: width %d, height %d, size = [%dx%d]", width, height, width_, height_);
        int savename;
        
        glGenTextures(1, &name_);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &savename);
        graphics_engine::instance().bind_texture(name_);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pot_data.at(0));
        graphics_engine::instance().bind_texture(savename);
        
        
        size_ = x2d::math::size(width_, height_);
        
        width_ = width;
        height_ = height;
        max_s_ = size_.width / (float)width;
        max_t_ = size_.height / (float)height;
        
        LOG("Created width=%d, height=%d", width_, height_);
        LOG("Created maxs=%f, maxt=%f", max_s_, max_t_);
    }
    
    void texture::draw() const
    {
        glPushMatrix();
        glEnable(GL_BLEND);
        
        float coordinates[] =
        {
            0,              max_t_,
            max_s_,         max_t_,
            0,              0,
            max_s_,         0
        };
        
        float width = (float)width_ * max_s_,
        height = (float)height_ * max_t_;
        float vertices[] =
        {      
            -width / 2,     -height / 2,    0.0,
            width / 2,      -height / 2,    0.0,
            -width / 2,     height / 2,     0.0,
            width / 2,      height / 2,     0.0
        };
        
        graphics_engine::instance().bind_texture(name_);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisable(GL_BLEND);
        
        glPopMatrix();
    }

} // namespace base
} // namespace x2d
