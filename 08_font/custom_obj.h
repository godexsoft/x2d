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

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, const boost::shared_ptr<font>& f)
    : base_object(k)
    , font_(f)
    , r_(0.0f)
    , li_(r_, 0.0f, 1.0f, 0.5f)
    {    
        connect_update();
        connect_render(0.0f);
    }
    
private:
    boost::shared_ptr<font> font_;
    
    float r_;
    linear_interpolator li_;
    
protected:

    void update(const clock_info& clock)
    {
        li_.update(clock.delta_time);
    }
    
    void render(const clock_info& clock)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(r_, 0.0, 0.0, 1.0);

        glPushMatrix();
        std::string txt("*ROFL* \"quotes\" tested. Welcome to x2d engine.");        
        size sz = font_->calculate_size(txt);        
        
        glTranslatef(-sz.width/2, 0, 0);
        
        font_->print(txt);

        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 1);

        glPopMatrix();
    }
};

#endif // __X2D_CUSTOM_OBJ_H__
