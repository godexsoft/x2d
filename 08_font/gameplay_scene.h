//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __SCENE_H__
#define __SCENE_H__

#include "app_framework.h"

class scene
: public base_object
{
public:
    scene(app_framework& app)
    : base_object(app.get_kernel())
    , app_(app)
    {
        app_.get_config().create_object("str");
        app_.get_config().create_object("str2");
        app_.get_config().create_object("str3");
    }
    
private:
    app_framework&  app_;
};


#endif // __SCENE_H__
