//
//  event_manager.cpp
//  x2d
//
//  Created by Alex Kremer on 23/8/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "event_manager.h"

namespace x2d {
namespace event {
    
    const boost::signals2::connection
    event_manager::connect_listener(
       std::string ev,
       const boost::function<void()>& l)
    {
        boost::ptr_map<std::string, listener>::iterator li = listeners_.find(ev);
        if(li != listeners_.end())
        {
            return (*li->second).connect(l);
        }
        else
        {
            listeners_.insert( ev, new listener() );
            return listeners_[ev].connect(l);
        }
    }
    
//    void event_manager::send(std::string& ev, const boost::any& payload)
    void event_manager::send(std::string& ev)
    {
        boost::ptr_map<std::string, listener>::iterator li = listeners_.find(ev);
        if(li != listeners_.end())
        {
            LOG("Sending event '%s' to listeners..", ev.c_str());
            (*li->second)();
        }
        else
        {
            LOG("Event '%s' not registered.", ev.c_str());
        }
    }
    
    
} // namespace event
} // namespace x2d