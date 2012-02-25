//
//  label.cpp
//  x2d
//
//  Created by Alex Kremer on 23/02/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "label.h"
#include "configuration.h"

namespace x2d {
namespace ui {
namespace widget {

    label::label(configuration& conf, 
        const std::string& text, const boost::shared_ptr<font>& fnt)
    : object(conf.get_kernel(), conf)
    , text_(text)
    , font_(fnt)
    {     
        connect_render(position_.z);
    }
    
    void label::update(const clock_info& clock)
    {
        object::update(clock);
    }
    
    void label::render(const clock_info& clock)
    {
        glPushMatrix();
        glTranslatef(position_.x, position_.y, 0.0f);
        
        if(rotation_ != 0.0f)
        {
            glRotatef(rotation_, 0, 0, 1);
        }
        
        if(scale_ != 0.0f)
        {
            glScalef(scale_, scale_, 1.0f);
        }
                
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        
        font_->print(text_);

        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);

        glPopMatrix();        
    }
    
} // namespace widget
} // namespace ui
} // namespace x2d
