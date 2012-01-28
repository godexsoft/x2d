//
//  liverpool_manager.cpp
//  x2d
//
//  Created by Alex Kremer on 1/27/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "liverpool_manager.h"
#include "log.h"

namespace x2d {
namespace liverpool {

    void liverpool_manager::mount(const path& path, const std::string& mount_point)
    {
        LOG("Mounting '%s' to '%s'", path.string().c_str(), mount_point.c_str());
        mount_points_.insert( 
            std::make_pair( mount_point, 
                liverpool_ptr( new x2d::liverpool::liverpool(path) ) ) );
    }
    
    void liverpool_manager::unmount(const std::string& mount_point)
    {
        LOG("Unmounting '%s'", mount_point.c_str());
//        mount_point_.
    }
    
    const liverpool_ptr liverpool_manager::get(const std::string& mount_point)
    {
        LOG("Getting '%s'", mount_point.c_str());
        if( mount_points_.find(mount_point) == mount_points_.end() )
        {
            LOG("Requested mount point does not exist. Forgot to mount?");
            throw std::exception();
        }
        
        return mount_points_[mount_point];
    }
        
} // namespace liverpool
} // namespace x2d
