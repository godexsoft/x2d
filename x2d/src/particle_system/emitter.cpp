//
//  emitter.cpp
//  x2d
//
//  Created by Alex Kremer on 04/03/2013.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#include "emitter.h"
#include "configuration.h"

namespace x2d {    
namespace particle_system {

    emitter::emitter(kernel& k, configuration& conf, const emitter_settings& es)
    : kernel_(k)
    , config_(conf)
    , es_(es)
    {
    }
    
} // namespace particle_system
} // namespace x2d