//
//  exceptions.h
//  x2d
//
//  Created by Alex Kremer on 2/2/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_EXCEPTIONS_H__
#define __X2D_EXCEPTIONS_H__

#include <exception>

namespace x2d {
namespace base {

    /**
     * @brief Base exception class
     */
    class sys_exception
    : public std::exception
    {
    public:
        /**
         * @param[in] msg The message to store
         */
        sys_exception(const std::string& msg) throw()
        : msg_(msg)
        {            
        }
        
        virtual ~sys_exception() throw()
        {            
        }
        
        const char* what() const throw()
        {
            return msg_.c_str();
        }
        
    private:
        std::string     msg_;
    };
    
    /**
     * @brief Config system exception
     */
    class config_exception
    : public sys_exception
    {    
    public:
        config_exception(const std::string& msg)
        : sys_exception(msg)
        {            
        }
    };

    /**
     * @brief Parsing exception in configuration system
     */
    class parse_exception
    : public config_exception
    {        
    public:
        parse_exception(const std::string& msg)
        : config_exception("[x2d config parsing]: " + msg)
        {            
        }
    };

    /**
     * @brief Structure exception in configuration system
     */
    class structure_exception
    : public config_exception
    {    
    public:
        structure_exception(const std::string& msg)
        : config_exception("[x2d config structure]: " + msg)
        {            
        }
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_EXCEPTIONS_H__
