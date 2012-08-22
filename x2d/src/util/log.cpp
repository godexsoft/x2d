//
//  log.cpp
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "log.h"
#include <stdio.h>
using namespace std;

namespace x2d {
namespace util {

    void logger::lst_log(const string& format, va_list lst)
    {
    #ifdef ANDROID
        char *buf = NULL;
        vasprintf(&buf, format.c_str(), lst);
        __android_log_print(ANDROID_LOG_INFO,LOG_TAG,"%s", buf);
        free(buf);
    #else	
        vprintf(string(format).append("\n").c_str(), lst);
    #endif
    }

    void logger::simple_log(const string& s)
    {
#ifdef ANDROID
        __android_log_print(ANDROID_LOG_INFO,LOG_TAG,"%s", s.c_str());
#else
        printf(string(s).append("\n").c_str());
#endif
    }

} // namespace util
} // namespace x2d

// Plain C version
extern "C" void x2d_log(const char* format, ...) 
{
	va_list			lst;
	va_start(lst, format);
	
	logger::lst_log(format, lst);

	va_end(lst);
}

extern "C" void x2d_log_simple(const char* s)
{
	logger::simple_log(s);
}
