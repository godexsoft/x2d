//
//  platform.mm
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "platform.h"
#include <Foundation/Foundation.h>
#include <sys/time.h>

namespace platform {
namespace time {
    
    double current_time()
    {
        struct timeval start;        
        gettimeofday(&start, NULL);
        return double(start.tv_sec) + start.tv_usec * 1E-6;
    }
    
} // namespace time

namespace filesystem {
    
    const std::string path_for_resource(const std::string& resource)
    {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        
        std::string tmp = [[[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:resource.c_str()] ofType:@""] UTF8String];
        
        [pool drain];
        return tmp;
    }
    
    const std::string path_for_document(const std::string& name)
    {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        
        NSArray* doc_paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* root = [doc_paths objectAtIndex:0];
        std::string tmp = [[root stringByAppendingPathComponent:[NSString stringWithUTF8String:name.c_str()]] UTF8String];
        
        [pool drain];
        return tmp;
    }

} // namespace filesystem    
    
namespace thread {

    counter interlocked_increment(counter* c) 
    {
        return __sync_add_and_fetch(c, 1);
    }
    
    counter interlocked_decrement(counter* c) 
    {
        return __sync_sub_and_fetch(c, 1);
    }
    
} // namespace thread
} // namespace platform

