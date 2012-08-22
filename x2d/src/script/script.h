//
//  script.h
//  x2d
//
//  Created by Alex Kremer on 22/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_SCRIPT_H__
#define __X2D_SCRIPT_H__

#include <string>
#include "log.h"

namespace x2d {    
namespace scripting {

    class scripting_engine;
    
    class script
    {
        friend class scripting_engine;
        
    public:
        script(const std::string& data)
        : data_(data)
        {
            LOG("Created script object...");
        }
        
        ~script()
        {
            LOG("Destroyed script object...");
        }
        
    private:
        std::string data_;
    };
    
} // namespace scripting
} // namespace x2d
using namespace x2d::scripting;

#endif // __X2D_SCRIPT_H__
