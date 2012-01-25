//
//  log.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __LOG_UTIL_H__
#define __LOG_UTIL_H__

#ifdef ANDROID

#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG "x2d"
#endif // Log tag
#endif // Android

#ifdef DEBUG
#define LOG(a...) x2d_log(a)
#else
#define LOG(a...)
#endif

#ifdef __cplusplus
#include <string>
#include <stdarg.h>

namespace x2d {
namespace util {
    
    class log 
    {
    public:
        static void lst_log(const std::string& format, va_list lst);
    };
} // namespace util
} // namespace x2d

using namespace x2d::util;

#endif

/// Define a plain C interface
#ifdef __cplusplus
extern "C"
#endif
void x2d_log(const char* format, ...);

#endif // __LOG_UTIL_H__
