//
//  liverpool_manager.h
//  x2d
//
//  Created by Alex Kremer on 1/27/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_LIVERPOOL_MANAGER_H__
#define __X2D_LIVERPOOL_MANAGER_H__

#include "liverpool.h"
#include <map>
#include <string>

namespace x2d {
namespace liverpool {
    
    class liverpool_manager
    {
    public:
        /**
         * Mount a liverpool zip file to a named mount point.
         * If already mounted, the paths will be overriden and/or added
         * from the new archive.
         * 
         * @param[in] path The path to a liverpool zip file
         * @param[in] mount_point The name for this mount point
         */
        void mount(const path& path, const std::string& mount_point);
        
        /**
         * Unmount a named mount point.
         * All information about the mountpoint will be lost.
         * 
         * @param[in] mount_point The name of the mount point to unmount
         */
        void unmount(const std::string& mount_point);

        /**
         * Retrieve a reference to an open liverpool filesystem.
         *
         * @param[in] mount_point The "virtual disk" to retrieve
         */
        const liverpool_ptr get(const std::string& mount_point);
        
    private:
        typedef std::map<std::string, liverpool_ptr> mnt_map;

        mnt_map     mount_points_;
        
    };

} // namespace liverpool
} // namespace x2d
using namespace x2d::liverpool;

#endif // __X2D_LIVERPOOL_MANAGER_H__
