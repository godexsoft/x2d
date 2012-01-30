//
//  resource_manager.h
//  x2d
//
//  Created by Alex Kremer on 1/27/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_RESOURCE_MANAGER_H__
#define __X2D_RESOURCE_MANAGER_H__

#include "liverpool_manager.h"
#include "resource.h"

namespace x2d {
namespace resource {

    class resource_manager
    {
    public:
        resource_manager(liverpool_manager& lvp_man)
        : lvp_man_(lvp_man)
        {            
        }
        
        template<typename T>
        resource_ptr<T> get(const std::string& key)
        {
            // get first path component. That is the name of the virtual disk
            size_t pos = key.find('/');
            if(pos == std::string::npos)
            {
                LOG("Requested key does not contain a correct path: '%s'", key.c_str());
                throw std::exception();
            }
            
            std::string disk = key.substr(0, pos);
            liverpool_ptr l = lvp_man_.get(disk);
            
            return resource_ptr<T>( 
                new T( l->open_stream(key.substr(pos)) ) );
        }
        
    private:
        liverpool_manager&   lvp_man_;
//        std::map<std::string, base_resource_ptr>
    };
    
} // namespace resource    
} // namespace x2d
using namespace x2d::resource;

#endif // __X2D_RESOURCE_MANAGER_H__