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
        
    /**
     * @brief Simple viewport (virtual screen)
     */
    class viewport
    {        
    public:
        /**
         * @param[in] cam       The camera to use
         * @param[in] bg_col    Background color
         */
        viewport(const rect& b, const boost::shared_ptr<camera>& cam, 
                 const color_info& bg_col=color_info(0.0f, 0.0f, 0.0f))
        : box_(b)
        , camera_(cam)
        , bg_color_(bg_col)
        {            
            LOG("Creating viewport (%f %f %f %f)", 
                box_.origin.x, box_.origin.y,
                box_.area.width, box_.area.height);
            
            LOG("Done creating viewport...");
        }
        
        ~viewport()
        {
        }
        
        boost::shared_ptr<camera> get_camera()
        {
            return camera_;
        }
        
        rect box() const
        {
            return box_;
        }
        
        /**
         * Get world-space location from screen-space location
         * @param[in] p Screen-space location
         */
        point get_world_location(const point& p) const
        {
            // translate screen-space pixel size to viewport-space pixel size
            float xs = camera_->frustum_.width / box_.area.width;
            float ys = camera_->frustum_.height / box_.area.height;
            point vp = point(p.x*xs, p.y*ys);
            
            LOG("Request world location from viewport location: %f %f", vp.x, vp.y);
            return camera_->inverted_transformation(vp);
        }
        
        /**
         * Start using this viewport.
         * The method sets opengl up to use this viewport.
         */
        void use()
        {               
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();  
            
#ifdef GL_ES_INCLUDED
            glOrthof(0, camera_->frustum_.width, 0, camera_->frustum_.height, -1, 100);
#elif defined (_OPENGL_H)
            glOrtho(0, camera_.frustum_.width, 0, camera_.frustum_.height, -1, 100);
#endif
            glMatrixMode(GL_MODELVIEW);

            // setup our (smaller) viewport and tell opengl we want to cut everything out of view
            glViewport(box_.origin.x, box_.origin.y, box_.area.width, box_.area.height);
            glScissor(box_.origin.x, box_.origin.y, box_.area.width, box_.area.height);
        }

        /**
         * Clear viewport with background color
         */
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
