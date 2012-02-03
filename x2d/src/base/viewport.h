//
//  viewport.h
//  x2d
//
//  Created by Alex Kremer on 2/2/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_VIEWPORT_H__
#define __X2D_VIEWPORT_H__

#include "math_util.h"
#include "log.h"
#include "graphics_engine.h"
#include "camera.h"
#include "color_info.h"

namespace x2d {
namespace base {
        
    class viewport
    {        
    public:
        viewport(const rect& b, const boost::shared_ptr<camera>& cam, 
                 const color_info& bg_col=color_info(0.0f, 0.0f, 0.0f))
        : box_(b)
        , camera_(cam)
        , bg_color_(bg_col)
        {            
            LOG("Creating viewport (%f %f %f %f)", 
                box_.origin.x, box_.origin.y,
                box_.size.width, box_.size.height);
            
            LOG("Done creating viewport...");
        }
        
        ~viewport()
        {
        }
        
        boost::shared_ptr<camera> get_camera()
        {
            return camera_;
        }
        
        /**
         * Start using this viewport.
         * The method sets opengl up to use this viewport.
         */
        void use()
        {               
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();  
            
#ifdef ES1_GLEXT_H_GUARD
            glOrthof(0, camera_->frustum_.width, 0, camera_->frustum_.height, -1, 100);
#elif defined (_OPENGL_H)
            glOrtho(0, camera_.frustum_.width, 0, camera_.frustum_.height, -1, 100);
#endif
            glMatrixMode(GL_MODELVIEW);

            // setup our (smaller) viewport and tell opengl we want to cut everything out of view
            glViewport(box_.origin.x, box_.origin.y, box_.size.width, box_.size.height);
            glScissor(box_.origin.x, box_.origin.y, box_.size.width, box_.size.height);
        }

        void clear()
        {
            // draw bg
            glPushMatrix();
            glLoadIdentity();
            
            float width =  camera_->frustum_.width;
            float height = camera_->frustum_.height;
            
            float vertices[] =
            {      
                0,     0,    0.0,
                width, 0,    0.0,
                0,     height,     0.0,
                width, height,     0.0
            };
            
            glDisable(GL_TEXTURE_2D);
            glColor4f(bg_color_.r, bg_color_.g, bg_color_.b, bg_color_.a);
            glVertexPointer(3, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glEnable(GL_TEXTURE_2D);            
            glPopMatrix();
        }
        
    private:
        rect                        box_;
        boost::shared_ptr<camera>   camera_;
        color_info                  bg_color_;
    };
        
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_VIEWPORT_H__
