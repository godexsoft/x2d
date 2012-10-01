//
//  resource_manager.cpp
//  x2d
//
//  Created by Alex Kremer on 9/30/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "resource_manager.h"
#include "log.h"

namespace x2d {
namespace resource {

    //
    // Texture resources will attempt to load from resolution-perfect folder first.
    //
    template<>
    boost::shared_ptr<texture> resource_manager::get<texture>(const std::string& key)
    {
        LOG("TEXTURE VERSION");
        
        // get first path component. That is the name of the virtual disk
        size_t pos = key.find('/');
        if(pos == std::string::npos)
        {
            LOG("Requested key does not contain a correct path: '%s'", key.c_str());
            throw std::exception();
        }
        
        std::string disk = key.substr(0, pos);
        liverpool_ptr l = lvp_man_.get(disk);
        
        try
        {
            std::stringstream ss;
            
            // check if it's possible to get a resolution-perfect version
            ss << "/__"
            << static_cast<int>(caps_.has_retina? caps_.display_size.width*2
                                : caps_.display_size.width)
            << "x"
            << static_cast<int>(caps_.has_retina? caps_.display_size.height*2
                                : caps_.display_size.height);
            
            std::string rpv = ss.str() + key.substr(pos);
            LOG("Resolution-perfect version check: '%s'", rpv.c_str());
            
            return boost::shared_ptr<texture>(
                new texture( l->open_stream(rpv) ) );
        }
        catch(std::exception& e)
        {
            // not found. return default
            return boost::shared_ptr<texture>(
                new texture( l->open_stream(key.substr(pos)) ) );
        }
    }
    
} // namespace resource
} // namespace x2d
