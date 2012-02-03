//
//  value_parsers.cpp
//  x2d
//
//  Created by Alex Kremer on 2/2/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"
#include "exceptions.h"

#include <sstream>

namespace x2d {
namespace config {
   
    template<>
    bool value_parser<bool>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        bool b;
        ss >> b;
        
        return b;
    }
    
    template<>
    float value_parser<float>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        float f;
        ss >> f;
        
        return f;
    }

    template<>
    int value_parser<int>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        int i;
        ss >> i;
        
        return i;
    }
    
    template<>
    point value_parser<point>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        point p(0, 0);        
        ss >> p.x >> p.y;
        
        return p;
    }

    template<>
    size value_parser<size>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        size s(0, 0);        
        ss >> s.width >> s.height;
        
        return s;
    }

    template<>
    rect value_parser<rect>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        rect r(0, 0, 0, 0);        
        ss >> r.origin.x >> r.origin.y >> r.size.width >> r.size.height;
        
        return r;
    }
    
    template<>
    vector_2d value_parser<vector_2d>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        float x, y;
        ss >> x >> y;
        
        return vector_2d(x, y);
    }
    
    template<>
    color_info value_parser<color_info>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        float r, g, b, a;        
        ss >> r >> g >> b;
        
        if(ss >> a)
        {
            return color_info(r, g, b, a);
        }
        else
        {
            return color_info(r, g, b);
        }
    }
} // namespace config
} // namespace x2d