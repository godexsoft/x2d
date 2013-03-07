//
//  value_parsers.cpp
//  x2d
//
//  Created by Alex Kremer on 2/2/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"
#include "space.h"
#include "exceptions.h"

#include <sstream>

namespace x2d {
namespace config {
   
    template<>
    std::string value_parser<std::string>::parse(const std::string& str)
    {
        return str; // :)
    }
    
    template<>
    bool value_parser<bool>::parse(const std::string& str)
    {        
        return str=="true" || str=="1" || str=="yes" || str=="y";
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
    short value_parser<short>::parse(const std::string& str)
    {
        std::stringstream ss;
        
        if(str.substr(0, 2) == "0x")
        {
            ss << std::hex << str.substr(2);
        }
        else
        {
            ss << str;
        }
        
        short i;
        ss >> i;
        
        return i;
    }
    
    template<>
    std::vector<std::string> value_parser<std::vector<std::string> >::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        std::vector<std::string> tmp;
        std::string s;
        
        while(ss >> s)
        {
            tmp.push_back(s);
        }
        
        return tmp;
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
        ss >> r.origin.x >> r.origin.y >> r.area.width >> r.area.height;
        
        return r;
    }
    
    template<>
    glm::vec4 value_parser<glm::vec4>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        float x, y, z, a;
        ss >> x >> y;
        
        if(!(ss >> z))
        {
            z = 0.0f;
            a = 0.0f;
        }
        else if(!(ss >> a))
        {
            a = 0.0f;
        }
        
        return glm::vec4(x, y, z, a);
    }
    
    template<>
    glm::vec3 value_parser<glm::vec3>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        float x, y, z;
        ss >> x >> y;

        if(!(ss >> z))
        {
            z = 0.0f;
        }
        
        return glm::vec3(x, y, z);
    }
    
    template<>
    glm::vec2 value_parser<glm::vec2>::parse(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        
        float x, y;
        ss >> x >> y;

        return glm::vec2(x, y);
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
    
    template<>
    space value_parser<space>::parse(const std::string& str)
    {
        if(str == "world")
        {
            return WORLD_SPACE;
        }
        else if(str == "camera")
        {
            return CAMERA_SPACE;
        }
        else if(str == "screen")
        {
            return SCREEN_SPACE;
        }
        else
        {
            throw parse_exception("Space type must be one of 'screen', 'camera' or 'world'. Got '" + str + "' instead.");
        }
    }
    
    template<>
    parent_space value_parser<parent_space>::parse(const std::string& str)
    {
        if(str == "none")
        {
            return PARENT_SPACE_NONE;
        }
        else if(str == "both")
        {
            return PARENT_SPACE_BOTH;
        }
        else if(str == "position")
        {
            return PARENT_SPACE_POSITION;
        }
        else if(str == "box")
        {
            return PARENT_SPACE_BOX;
        }
        else
        {
            throw parse_exception("ParentSpace type must be one of 'none', 'both', 'position' or 'box'. Got '" + str + "' instead.");
        }
    }
    
    template<>
    alignment value_parser<alignment>::parse(const std::string& str)
    {
        if(str == "left")
        {
            return LEFT_ALIGN;
        }
        else if(str == "right")
        {
            return RIGHT_ALIGN;
        }
        else if(str == "center")
        {
            return CENTER_ALIGN;
        }
        else
        {
            throw parse_exception("Align type must be one of 'left', 'center' or 'right'. Got '" + str + "' instead.");
        }
    }
    
} // namespace config
} // namespace x2d