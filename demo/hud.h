//
//  hud.h
//  x2d
//
//  Created by Alex Kremer on 4/28/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//
//  HAPPY BIRTHDAY, EGOR!!! :-)
//

#pragma once
#ifndef __HUD_H__
#define __HUD_H__

#include "object.h"
#include "configuration.h"

class hud 
: public object
{
public:
    hud(kernel& k, configuration& conf, const object_traits& t);
    
    void update_fuel(float f);
    void update_indicators(bool orientation, bool velocity);
    
protected:
    virtual void update(const clock_info& clock);
    
private:
    configuration& config_;
    
    // the fuel bar 'filling'
    boost::shared_ptr<object> fuel_bar_filling_;
    
    // the indicators
    boost::shared_ptr<object> orientation_icon_;
    boost::shared_ptr<object> velocity_icon_;
    
    // get both indicator sprites first so that
    // x2d wont free the resource everytime we swap
    boost::shared_ptr<sprite> red_;
    boost::shared_ptr<sprite> green_;
};

#endif // __HUD_H__
