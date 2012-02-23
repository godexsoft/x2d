//
//  label.h
//  x2d
//
//  Created by Alex Kremer on 23/02/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_LABEL_H__
#define __X2D_LABEL_H__

#include "object.h"
#include "font.h"

namespace x2d {
namespace ui {
namespace widget {
        
    /**
     * @brief Implementation of a simple label
     */
    class label
    : public object
    {
    public:
        label(configuration& conf, 
              const std::string& text, const boost::shared_ptr<font>& fnt);
        
    protected:
        virtual void update(const clock_info& clock);        
        virtual void render(const clock_info& clock); 
        
    private:
        std::string             text_;
        boost::shared_ptr<font> font_;
    };
    
} // namespace widget
} // namespace ui
} // namespace x2d
using namespace x2d::ui::widget;

#endif // __X2D_LABEL_H__
