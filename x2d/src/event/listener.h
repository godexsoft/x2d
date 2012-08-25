//
//  listener.h
//  x2d
//
//  Created by Alex Kremer on 25/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_EVENT_LISTENER_H__
#define __X2D_EVENT_LISTENER_H__

#include "log.h"
#include "script.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals.hpp>
#include <boost/signals/connection.hpp>

namespace x2d {
    
    class kernel;

namespace config {
    
    class configuration;
    
} // namespace config
using namespace x2d::config;

namespace event {

    class listener
    {
    public:
        listener(kernel& k, configuration& c, const std::string& ev);
        ~listener();
        
        void set_callback(const boost::function<void()>& cb);
        void set_script(const boost::shared_ptr<script>& scr);
        
    private:
        void script_executor();
        
        kernel&          kernel_;
        configuration&   config_;
        
        std::string                 event_;
        boost::function<void()>     callback_;
        boost::shared_ptr<script>   script_;
        
        boost::signals::connection con_;
    };
    
} // namespace event
} // namespace x2d
using namespace x2d::event;

#endif // __X2D_EVENT_LISTENER_H__
