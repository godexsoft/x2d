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
#include <luabind/tag_function.hpp>

#include "kernel.h"
#include "event_manager.h"
#include "object.h"

static void empty_log(const char*) {}

namespace x2d {
namespace scripting {

    using namespace luabind;

    scripting_engine::scripting_engine(kernel& k)
    : kernel_(k)
    , lua_(lua_open())
    {
        open(lua_);
        
        // register default modules
        luaL_openlibs(lua_);
        
        // bind all global x2d stuff
        bind_log();
        bind_aux_types();
        bind_event();
        bind_object();
    }
    
    void scripting_engine::execute(const boost::shared_ptr<script>& s)
    {
        if(luaL_dostring(lua_, s->data_.c_str()) )
        {
            luabind::object error_msg(from_stack(lua_, -1));
            std::cout << "\n\nLUA ERROR: " << error_msg << "\n\n";
        }
    }

    void scripting_engine::execute(const boost::shared_ptr<script>& s, object* o)
    {
        // inject object as self
        luabind::globals(lua_)["self"] = o;
        
        execute(s);
        
        // remove object
        luabind::globals(lua_)["self"] = nil;
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

    
    struct send_event_wrapper
    {
        send_event_wrapper(const boost::shared_ptr<event_manager>& em)
        : em_(em)
        {}
        
        void operator()(std::string ev) const
        {
            em_->send(ev);
        }
        
        boost::shared_ptr<event_manager> em_;
    };
    
    struct listen_event_wrapper
    {
        listen_event_wrapper(const boost::shared_ptr<event_manager>& em)
        : em_(em)
        {}
        
        const boost::signals::connection operator()(std::string ev, luabind::object o) const
        {
            if(type(o) != LUA_TFUNCTION)
            {
                throw std::runtime_error("event.listen: must pass a function as second argument.");
            }
            
            return em_->connect_listener(ev, o);
        }
        
        boost::shared_ptr<event_manager> em_;
    };
    
    void scripting_engine::bind_event()
    {        
        module(lua_, "event")
        [
            def("send", luabind::tag_function<void(std::string)>(
                send_event_wrapper(kernel_.get_event_manager()))),
            def("listen",
                luabind::tag_function<const boost::signals::connection
                    (std::string, luabind::object)>(
                listen_event_wrapper(kernel_.get_event_manager())))
        ];
        
        module(lua_, "event")
        [
            class_<boost::signals::connection>("connection")
                .def(constructor<>())
                .def("disconnect", &boost::signals::connection::disconnect)
        ];
    }

    void scripting_engine::bind_aux_types()
    {
        module(lua_)
        [
            class_<color_info>("color_info")
                .def(constructor<float,float,float,float>())
        ];
        
        module(lua_)
        [
            class_<glm::vec2>("vec2")
                .def(constructor<float,float>()),
            class_<glm::vec3>("vec3")
                .def(constructor<float,float,float>())
        ];
    }
    
    void scripting_engine::bind_object()
    {
        module(lua_)
        [
            class_<object>("object")
                .def("bgcolor", &object::set_bgcolor)
                .def("rotation", (void(object::*)(float))&object::rotation)
                .def("scale", (void(object::*)(float))&object::scale)
                .def("position", (void(object::*)(const glm::vec2&))&object::position)
        ];
    }

    
} // namespace scripting
} // namespace x2d