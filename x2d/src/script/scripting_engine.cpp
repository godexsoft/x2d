//
//  scripting_engine.cpp
//  x2d
//
//  Created by Alex Kremer on 22/8/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "scripting_engine.h"

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

#include <luabind/luabind.hpp>

static void empty_log(const char*) {}

namespace x2d {
namespace scripting {

    using namespace luabind;

    scripting_engine::scripting_engine()
    : lua_(lua_open())
    {
        open(lua_);
        
        // register default modules
        luaL_openlibs(lua_);
        
        // bind all global x2d stuff
        bind_log();
    }
    
    void scripting_engine::execute(const boost::shared_ptr<script>& s)
    {
        if(luaL_dostring(lua_, s->data_.c_str()) )
        {
            object error_msg(from_stack(lua_, -1));
            std::cout << "\n\nLUA ERROR: " << error_msg << "\n\n";
        }
    }
    
    void scripting_engine::bind_log()
    {
        module(lua_)
        [
            namespace_("x2d")
            [
#if defined(DEBUG)
                def("log", &x2d_log_simple)
#else
                def("log", &empty_log)
#endif
            ]
        ];
    }
    
} // namespace scripting
} // namespace x2d