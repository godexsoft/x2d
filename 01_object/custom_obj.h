//
//  custom_obj.h
//  x2d
//
//  Created by Alex Kremer on 1/25/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CUSTOM_OBJ_H__
#define __X2D_CUSTOM_OBJ_H__

#include "base_object.h"

#include "clock.h"
#include "timer.h"
#include "interpolator.h"
#include "graphics_engine.h"
#include "texture.h"
#include "liverpool_manager.h"
#include "resource_manager.h"
#include "sprite.h"

#include "camera.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, liverpool_manager lvp_man)
    : base_object(k)
    , res_man_(lvp_man)
    , texture_(res_man_.get<texture>("res/test/graphics/chupa.png"))
    , spr_(texture_, point(0, 0), size(120, 120))
    , cam_(size(320, 480))
    , y_(0.0f)
    , liy_(y_, 0.0f, 320.0f, 3.0f)
    , x_(0.0f)
    , lix_(x_, 0.0f, 480.0f, 3.0f)
    , zoom_(1.0f)
    , qi_(zoom_, 1.0f, 2.0f, 1.0f, 3.0f)
    {        
        cam_.rotation(-90.0f); // rotate camera to go landscape
        cam_.position( vector_2d(-240.0f, -160.0f) ); // set camera 0,0 to left,bottom of physical screen
        
        connect_update();
        connect_render();        
    }
    
private:
    resource_manager            res_man_;
    boost::shared_ptr<texture>  texture_;
    sprite                      spr_;
    camera                      cam_;
    
    float y_;
    linear_interpolator liy_;

    float x_;
    linear_interpolator lix_;

    float zoom_;
    quadratic_interpolator qi_;    
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
        
        liy_.update(clock.delta_time);
        lix_.update(clock.delta_time);
        qi_.update(clock.delta_time);
        
        cam_.zoom(zoom_);
    }
    
    virtual void render(const clock_info& clock) 
    {          
        cam_.apply();
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(1.0, 1.0, 1.0, 1.0);
        
        glPushMatrix();

        glTranslatef(x_, y_, 0);
            
        spr_.draw_at_point(point(0, 0));

        glPopMatrix();        
        glDisable(GL_BLEND);
    }
};


#endif // __X2D_CUSTOM_OBJ_H__
