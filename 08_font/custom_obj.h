//
//  custom_obj.h
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CUSTOM_OBJ_H__
#define __X2D_CUSTOM_OBJ_H__

#include "base_object.h"
#include "font.h"
#include "interpolator.h"
#include "platform.h"
#include "resource_manager.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, resource_manager& rm, const boost::shared_ptr<font>& f)
    : base_object(k)
    , res_man_(rm)
    , font_(f)
    , r_(0.0f)
    , lir_(r_, 0.0f, 1.0f, 0.6f)
    , g_(0.0f)
    , lig_(g_, 0.0f, 1.0f, 0.9f)
    , b_(0.0f)
    , lib_(b_, 0.0f, 1.0f, 1.2f)
    {    
        connect_update();
        connect_render(0.0f);
        connect_touch_input(SCREEN_SPACE);
    }
    
private:
    resource_manager&       res_man_;
    boost::shared_ptr<font> font_;
    
    float r_;
    linear_interpolator lir_;

    float g_;
    linear_interpolator lig_;    
    
    float b_;
    linear_interpolator lib_;
    
protected:

    void update(const clock_info& clock)
    {
        lir_.update(clock.delta_time);
        lig_.update(clock.delta_time);
        lib_.update(clock.delta_time);
    }
    
    void render(const clock_info& clock)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(r_, g_, b_, 1.0);

        glPushMatrix();
        std::string txt("i can haz right align");        
        
        glTranslatef(240, -100, 0);
        font_->print(txt, RIGHT_ALIGN);

        glPopMatrix();
        glPushMatrix();
        txt = "i can haz left align";

        glTranslatef(-240, 0, 0);
        font_->print(txt, LEFT_ALIGN);

        glPopMatrix();
        glPushMatrix();
        txt = "i can haz center align";
        
        glTranslatef(0, 100, 0);
        font_->print(txt, CENTER_ALIGN);

        
        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 1);

        glPopMatrix();
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches)
    {
    }
};

#endif // __X2D_CUSTOM_OBJ_H__
