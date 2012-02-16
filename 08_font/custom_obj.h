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
#include "sound.h"
#include "platform.h"

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
    , rot_(0.0f)
    , lirot_(rot_, 0.0f, 360.0f, 2.0f)
    , sfx_(sound_engine::instance().get_sfx(rm, "res/sound/swoosh.caf"))
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
    
    float rot_;
    linear_interpolator lirot_;
    
    boost::shared_ptr<sfx_obj<X2D_SND_DRIVER> > sfx_;
protected:

    void update(const clock_info& clock)
    {
        lir_.update(clock.delta_time);
        lig_.update(clock.delta_time);
        lib_.update(clock.delta_time);
        
        lirot_.update(clock.delta_time);
    }
    
    void render(const clock_info& clock)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(r_, g_, b_, 1.0);

        glPushMatrix();
        std::string txt("\"quotes\"! Welcome to x2d :-)");        
        size sz = font_->calculate_size(txt);        

        glRotatef(rot_, 0, 0, 1);
        glTranslatef(-sz.width/2, 0, 0);
        
        font_->print(txt);

        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 1);

        glPopMatrix();
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches)
    {
        // sound_engine::instance().play( platform::filesystem::path_for_resource("bg.mp3") );        
        // sound_engine::instance().play( res_man_, "res/sound/loop.caf" );        
     
        sfx_->reset();
        sfx_->play();
    }
};

#endif // __X2D_CUSTOM_OBJ_H__
