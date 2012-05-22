//
//  title.h
//  invaders
//
//  Created by Alex Kremer on 5/21/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __TITLE_H__
#define __TITLE_H__

#include "configuration.h"
#include <vector>

class game;

class title 
: public base_object
{
public:
    title(kernel& k, configuration& conf, game& g);
    
protected:
    void touch_input_began(space s, const std::vector<touch>& touches);

private:
    
    configuration   &config_;    
    game            &game_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __TITLE_H__
