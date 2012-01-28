//
//  texture.cpp
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "texture.h"
#include "log.h"

#include <vector>

namespace x2d {
namespace base {
          
    texture::texture(ifdstream stream)
    : name_(0)
    , size_(0,0)
    , width_(0)
    , height_(0)
    , format_(AUTO)
    , max_s_(0)
    , max_t_(0)
    {
        LOG("Create a texture");
        
        // FIXME: dirty?
        // get the data
        std::vector<char> dat;
        dat.resize(stream.size());
        
        stream.read(&dat.at(0), stream.size());
        init_texture( (void*)(&dat.at(0)) );
        
        LOG("Texture out of %d bytes.", dat.size());
    }
    
    texture::~texture()
    {
        if(name_) 
        {
            glDeleteTextures(1, &name_);
        }
    }
    
    void texture::init_texture(const void* data)
    {
        LOG("Texture initialization.. TODO");        
    }
    
    void texture::draw_in_rect(rect box) const
    {
        /*
        GLfloat coordinates[] =
        {       
            0, max_t_,
            max_s_, max_t_,
            0, 0,
            max_s_, 0
        };
        
        GLfloat vertices[] =
        { 
            box.origin.x, box.origin.y, 0.0,
            box.origin.x + box.size.width, box.origin.y, 0.0,
            box.origin.x,  box.origin.y + box.size.height, 0.0,
            box.origin.x + box.size.width, box.origin.y + box.size.height, 0.0
        };
        
        glBindTexture(GL_TEXTURE_2D, name_);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
         */      
    }
    
    void texture::draw_at_point(point pos) const
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisable(GL_TEXTURE_2D);
        
        float w = 60;
        float h = 60;
        
        GLfloat vertices[] =
        {         
            -w / 2 + pos.x,   -h / 2 + pos.y,  0.0,
            w / 2 + pos.x,    -h / 2 + pos.y,  0.0,
            -w / 2 + pos.x,   h / 2 + pos.y,   0.0,
            w / 2 + pos.x,    h / 2 + pos.y,   0.0
        };
        
        glEnable(GL_BLEND);
        
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }

} // namespace base
} // namespace x2d
