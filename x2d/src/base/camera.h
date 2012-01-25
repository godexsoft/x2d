//
//  camera.h
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CAMERA_H__
#define __X2D_CAMERA_H__

#include "math_util.h"

namespace x2d {
namespace base {

    class camera
    {
    private:
        float       rotation_;
        float       zoom_;
        
        vector_2d   position_;
    };

} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_CAMERA_H__
