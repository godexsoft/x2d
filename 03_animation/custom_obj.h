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

#include "interpolator.h"
#include "sprite.h"
#include "configuration.h"
#include "animation.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, configuration& conf, const std::string& key)
    : base_object(k)
    {        
        connect_update();
        connect_render(0.0f);     
        
        anim_ = conf.get_object<animation>("animations.player");        
        anim_->start();
    }
    
private:
    boost::shared_ptr<animation> anim_;
        
protected:
    
    virtual void update(const clock_info& clock) 
    { 
        anim_->update(clock);
    }
    
    virtual void render(const clock_info& clock) 
    {  
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1, 1, 1, 1);
        
        glPushMatrix();
        anim_->draw_at_point(point(0, 0));
        glPopMatrix();        

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
    }
};


#endif // __X2D_CUSTOM_OBJ_H__
