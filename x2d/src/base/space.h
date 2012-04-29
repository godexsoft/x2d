//
//  space.h
//  x2d
//
//  Created by Alex Kremer on 2/5/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_SPACE_H__
#define __X2D_SPACE_H__

#include "log.h"

namespace x2d {
namespace base {

    /**
     * @brief Space. World or Screen
     */
    enum space
    {       
        WORLD_SPACE  = 1,
        SCREEN_SPACE = 2,
        CAMERA_SPACE = 3        
    };
    
    enum parent_space
    {
        PARENT_SPACE_POSITION = 1,
        PARENT_SPACE_BOX = 2,
        PARENT_SPACE_BOTH = 3,
        PARENT_SPACE_NONE = 4
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_SPACE_H__
