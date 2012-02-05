//
//  object.h
//  x2d
//
//  Created by Alex Kremer on 2/5/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_OBJECT_H__
#define __X2D_OBJECT_H__

#include "log.h"
#include "base_object.h"

namespace x2d 
{
    class object 
    : public base_object
    {        
    public:
        object(kernel& k)
        : base_object(k)
        {        
        }
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_OBJECT_H__
