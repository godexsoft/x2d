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
#include "platform.h"
#include "texture.h"

#include <sstream>

namespace x2d {
namespace resource {
    
    /**
     * @brief The resource manager.
     * Resource manager is used to manage resources such as textures, sounds and xml files.
     * It uses a liverpool manager to access the actual files.
     */
    class resource_manager
    {
    public:
        /**
         * @param[in] lvp_man   Liverpool manager to use
         */
        resource_manager(liverpool_manager& lvp_man)
        : lvp_man_(lvp_man)
        {            
        }
        
        /**
         * Get a resource of specified type.
         * @param[in] key   Key (path) to the resource
         * @return Shared pointer containing the resource
         */
        template<typename T>
        boost::shared_ptr<T> get(const std::string& key)
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
            
            // not found. return default
            return boost::shared_ptr<T>(
                new T( l->open_stream(key.substr(pos)) ) );
        }
        
    private:
        liverpool_manager&   lvp_man_;
        device_capabilities  caps_;
    };

    // forward declarations
    template<>
    boost::shared_ptr<texture> resource_manager::get<texture>(const std::string& key);
    
} // namespace resource    
} // namespace x2d
using namespace x2d::resource;

#endif // __X2D_RESOURCE_MANAGER_H__
