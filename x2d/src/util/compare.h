//
//  compare.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __PTR_CMP_H__
#define __PTR_CMP_H__

namespace x2d {
namespace util {
    
    template<typename T>
    struct ptr_cmp
    {
        bool operator()(const T* a, const T* b) const 
        {
            return *a<*b;
        }
    };
    
} // namespace util
} // namespace x2d
using namespace x2d::util;

#endif // __PTR_CMP_H__