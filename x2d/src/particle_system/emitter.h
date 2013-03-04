//
//  emitter.h
//  x2d
//
//  Created by Alex Kremer on 3/4/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_EMITTER_H__
#define __X2D_EMITTER_H__

#include "kernel.h"
#include "log.h"
#include "value.h"

namespace x2d { 
    
    class object;
    
namespace config {
    
    class configuration;
    
} // namespace config
using namespace x2d::config;
    
namespace particle_system {

    class emitter
    {
        friend class x2d::object;
        
    public:
        emitter(kernel& k, configuration& conf);
        
        virtual ~emitter()
        {            
        }
        
    protected:
        void update(const clock_info& clock) 
        {
        }

    private:
        kernel&         kernel_;
        configuration&  config_;
    };
    
} // namespace particle_system
} // namespace x2d
using namespace x2d::particle_system;

#endif // __X2D_SPAWNER_H__
