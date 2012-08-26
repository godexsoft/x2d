//
//  callable.h
//  x2d
//
//  Created by Alex Kremer on 25/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CALLABLE_H__
#define __X2D_CALLABLE_H__

#include "log.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace x2d {
namespace config {
    class configuration;
}
using namespace x2d::config;

namespace scripting {
    class script;
}
using namespace x2d::scripting;
    
    template<typename T>
    void exec_script(configuration& c, const boost::shared_ptr<script>& s, T o);
    
    /**
     * @brief Generic wrapper on top of either a boost function or a x2d script
     */
    template<typename T>
    class callable
    {
    public:
        callable(configuration& c)
        : config_(c)
        {            
        }
        
        void set_callback(const boost::function<void(T)>& cb)
        {
            callback_ = cb;
            script_ = boost::shared_ptr<script>();
        }
        
        void set_script(const boost::shared_ptr<script>& scr)
        {
            script_ = scr;
            callback_ = boost::function<void(T)>();
        }
        
        void operator ()(T o)
        {
            if(script_)
            {
                exec_script(config_, script_, o);
            }
            else if(callback_)
            {
                callback_(o);
            }
            else
            {
                LOG("No script or callback defined. callable didn't run.");
            }
        }
        
    private:        
        configuration&   config_;
        
        boost::function<void(T)>     callback_;
        boost::shared_ptr<script>    script_;
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_CALLABLE_H__
