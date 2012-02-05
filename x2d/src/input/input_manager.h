//
//  input_manager.h
//  x2d
//
//  Created by Alex Kremer on 2/3/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_INPUT_MANAGER_H__
#define __X2D_INPUT_MANAGER_H__

#include "touch.h"
#include "math_util.h"
#include "log.h"

#include <vector>

namespace x2d {

    // forward declare kernel to make friends
    class kernel;
    
namespace input {
    
    struct transform_touches 
    : public std::unary_function<const touch, const touch> 
    {
        affine_matrix& m_;
        transform_touches(affine_matrix& m) 
        : m_(m) 
        {
        }
        
        const touch operator()(const touch& t) 
        {
            return touch( m_.apply(t.location()), m_.apply(t.prev_location()), t.tap_count(), t.timestamp() );
        }
    };
    
    class input_manager
    {
        friend class kernel;
                
    public:
        input_manager(kernel& k, bool wt, bool wa)        
        : kernel_(k)
        , want_touch_(wt)
        , want_accel_(wa)
        {            
        }
        
        static const std::vector<touch> transform(const std::vector<touch>& touches, affine_matrix& mat) 
        {            
            std::vector<touch> tr;
            tr.reserve(touches.size());
            
            std::transform(touches.begin(), touches.end(), std::back_inserter(tr), transform_touches(mat));
            
            return tr;
        }
        
    private:
        void on_touches_began(const std::vector<touch>& touches);    
        void on_touches_moved(const std::vector<touch>& touches);
        void on_touches_ended(const std::vector<touch>& touches);
        
        kernel&     kernel_;
        bool        want_touch_;
        bool        want_accel_;
    };
    
} // namespace input
} // namespace x2d
using namespace x2d::input;

#endif // __X2D_INPUT_MANAGER_H__
