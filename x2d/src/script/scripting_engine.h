//
//  scripting_engine.h
//  x2d
//
//  Created by Alex Kremer on 22/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_SCRIPTING_ENGINE_H__
#define __X2D_SCRIPTING_ENGINE_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include "script.h"
#include "log.h"

struct lua_State;

namespace x2d {

    class kernel;
    class object;
    
namespace scripting {

    class scripting_engine
    {
    public:
        scripting_engine(kernel& k);
        
        void execute(const boost::shared_ptr<script>& s);
        void execute(const boost::shared_ptr<script>& s, object* o);
        
    private:
        
        void bind_log();
        void bind_aux_types();
        void bind_event();
        void bind_object();
        
        kernel&     kernel_;
        lua_State*  lua_;
    };
    
} // namespace scripting
} // namespace x2d
using namespace x2d::scripting;

#endif // __X2D_SCRIPTING_ENGINE_H__
