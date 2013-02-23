//
//  value.cpp
//  x2d
//
//  Created by Alex Kremer on 2/23/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#include "value.h"
#include "math_util.h"

namespace x2d {

    template<>
    int value<int>::get() const
    {
        if(is_rand_)
        {
            return int((double)from_ + floor( (double)value_ * dist_(x2d::GENERATOR) + chunk_ ));
        }
        else
        {
            return value_;
        }
    }

    template<>
    long value<long>::get() const
    {
        if(is_rand_)
        {
            return long((double)from_ + floor( (double)value_ * dist_(x2d::GENERATOR) + chunk_ ));
        }
        else
        {
            return value_;
        }
    }

    template<>
    short value<short>::get() const
    {
        if(is_rand_)
        {
            return short((double)from_ + floor( (double)value_ * dist_(x2d::GENERATOR) + chunk_ ));
        }
        else
        {
            return value_;
        }
    }
    
} // namespace x2d
