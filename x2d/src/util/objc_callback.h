//
//  objc_callback.h
//  x2d
//
//  Created by Alex Kremer on 1/22/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifdef __OBJC__

/**
 * This is to be used with Obj-C only
 */

#pragma once
#ifndef __X2D_CALLBACK_H__
#define __X2D_CALLBACK_H__

#include <boost/function.hpp>
#include <boost/bind.hpp>

static boost::function<void()> objc_callback(SEL sel, id obj)
{
    typedef void (*func)(id, SEL);
    func impl = (func)[obj methodForSelector:sel];
    return boost::bind(impl, obj, sel);    
}

template<typename T>
static boost::function<void(T)> objc_callback_1(SEL sel, id obj)
{
    typedef void (*func)(id, SEL, T);
    func impl = (func)[obj methodForSelector:sel];
    return boost::bind(impl, obj, sel, _1);    
}

template<typename T1, typename T2>
static boost::function<void(T1,T2)> objc_callback_2(SEL sel, id obj)
{
    typedef void (*func)(id, SEL, T1, T2);
    func impl = (func)[obj methodForSelector:sel];
    return boost::bind(impl, obj, sel, _1, _2);    
}

#endif // __X2D_CALLBACK_H__
#endif // __OBJC__