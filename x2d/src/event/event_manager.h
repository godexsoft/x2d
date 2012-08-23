//
//  event_manager.h
//  x2d
//
//  Created by Alex Kremer on 23/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_EVENT_MANAGER_H__
#define __X2D_EVENT_MANAGER_H__

#include <boost/any.hpp>
#include <boost/signals.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include <string>
#include "log.h"

namespace x2d {    
namespace event {

    class event_manager
    {
    public:
        typedef boost::signal<void()>  listener;
        
        const boost::signals::connection connect_listener(std::string ev,
            const boost::function<void()>& l);
        
//        void send(std::string& ev, const boost::any& payload);
        void send(std::string& ev);
        
    private:
        boost::ptr_map<std::string, listener>    listeners_;
    };
    
} // namespace event
} // namespace x2d
using namespace x2d::event;

#endif // __X2D_EVENT_MANAGER_H__
