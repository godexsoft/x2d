//
//  base_object.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __X2D_BASE_OBJECT_H__
#define __X2D_BASE_OBJECT_H__

#include "log.h"
#include "kernel.h"
#include "clock.h"
#include "timer.h"
#include "interpolator.h"
#include "graphics_engine.h"
#include "texture.h"

namespace x2d 
{

    class base_object 
    {
        friend class kernel;
        
    public:
        base_object(kernel& k)
        : kernel_(k)
        , alpha_(0.0f)
        , li_(alpha_, 0.0f, 0.1f, 2.0f, 0.0f, 1.0f)
        , height_(1.0f)
        , qi_(height_, 1.0f, 3.0f, 1.0f, 1.0f)
        , texture_()
        {        
            kernel_.connect_update( this );
            kernel_.connect_render( this );
        }
            
    private:
        kernel&     kernel_;
        
        float               alpha_;
        cubic_interpolator  li_;
        
        float               height_;
        quadratic_interpolator qi_;
        
        texture     texture_;
        
    protected:
        
        virtual void update(const clock_info& clock) 
        { 
            static int fps = 0;
            static double start = clock.time;
            
            if(clock.time-start >= 1.0)
            {
                LOG("FPS: %d on %f", fps, clock.time);
                start = clock.time;
                fps = 0;
            }
            else
            {
                ++fps;
            }

            li_.update(clock.delta_time);            
            qi_.update(clock.delta_time);
        }

        virtual void render(const clock_info& clock) 
        {            
            static float rotation = 0.01f;
            rotation += clock.delta_time*30;
            
            glLoadIdentity();        

            glEnable(GL_BLEND);
            glColor4f(0.5, 0.4, 0.6, alpha_);
            
            if(rotation > 360.0f)
            {
                rotation -= 360.0f;
            }

            glTranslatef(0, 0, 0);
//            glRotatef(rotation, 0, 0, 1);
            glScalef(alpha_+1.0f, height_, 1.0f);
            
            texture_.draw_at_point(point(160, 240));
                        
            glDisable(GL_BLEND);
        }
    };

} // namespace x2d
using namespace x2d;

#endif // __X2D_BASE_OBJECT_H__
