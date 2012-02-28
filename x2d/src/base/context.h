//
//  context.h
//  x2d
//
//  Created by Alex Kremer on 2/26/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CONTEXT_H__
#define __X2D_CONTEXT_H__

namespace x2d {

    class object;   
    
namespace base {
    
    class zone;
    
    /**
     * @brief Context for objects
     */
    class context
    {
        friend class object;    
        friend class zone;
        
    private:       
        void reg_object(object* o)
        {
            objects_.push_back(o);
        }
        
        void unreg_object(object* o)
        {
            objects_.erase( 
                std::remove( objects_.begin(), objects_.end(), o), 
                    objects_.end() );
        }
        
        std::vector<object*> objects_; // currently registered objects (active)
    };
    
} // namespace base
} // namespace x2d

#endif // __X2D_CONTEXT_H__
