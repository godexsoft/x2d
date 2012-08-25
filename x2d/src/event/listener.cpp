//
//  listener.cpp
//  x2d
//
//  Created by Alex Kremer on 25/8/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "listener.h"
#include "kernel.h"
#include "configuration.h"

namespace x2d {
namespace event {
    
    listener::listener(kernel& k, configuration& c, const std::string& ev)
    : kernel_(k)
    , config_(c)
    , event_(ev)
    {        
    }
    
    listener::~listener()
    {
        if(con_.connected())
        {
            con_.disconnect();
        }
    }
    
    void listener::set_callback(const boost::function<void()>& cb)
    {
        if(con_.connected())
        {
            con_.disconnect();
        }
        
        callback_ = cb;
        con_ = kernel_.get_event_manager()->connect_listener(event_, callback_);
    }
    
    void listener::set_script(const boost::shared_ptr<script>& scr)
    {
        script_ = scr;
        set_callback(boost::bind(&listener::script_executor, this));
    }
    
    void listener::script_executor()
    {
        config_.get_scripting_engine().execute(script_);
    }
    
} // namespace event
} // namespace x2d