//
//  callable.cpp
//  x2d
//
//  Created by Alex Kremer on 25/8/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "callable.h"
#include "configuration.h"
#include "script.h"

namespace x2d {
    
    template<>
    void exec_script(configuration& c, const boost::shared_ptr<script>& s, object* o)
    {
        c.get_scripting_engine().execute(s, o);
    }
    
} // namespace x2d