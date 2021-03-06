//
//  random.cpp
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"
#include "exceptions.h"

#include <sstream>

namespace x2d {
namespace config {
        
    template<>
    void configuration::parse_random<float>(xml_node* node, const config_key& key)
    {
        // must have:
        // from: minimal value to generate
        // to:   maximum value to generate
        
        xml_attr* from = node->first_attribute("from");
        if(!from) 
        {
            throw parse_exception("Random must have 'from' defined.");
        }
        
        xml_attr* to = node->first_attribute("to");
        if(!to) 
        {
            throw parse_exception("Random must have 'to' defined.");
        }
        
        std::stringstream ss;
        float min, max;
        
        ss << from->value() << " " << to->value();
        ss >> min >> max;
        
        config_[key] = boost::shared_ptr< value_cfg<float> >( new value_cfg<float>(min, max) );
    }
    
    
    template<>
    void configuration::parse_random<int>(xml_node* node, const config_key& key)
    {
        // must have:
        // from: minimal value to generate
        // to:   maximum value to generate
        
        xml_attr* from = node->first_attribute("from");
        if(!from) 
        {
            throw parse_exception("Random must have 'from' defined.");
        }
        
        xml_attr* to = node->first_attribute("to");
        if(!to) 
        {
            throw parse_exception("Random must have 'to' defined.");
        }
        
        std::stringstream ss;
        int min, max;
        
        ss << from->value() << " " << to->value();
        ss >> min >> max;
        
        config_[key] = boost::shared_ptr< value_cfg<int> >( new value_cfg<int>(min, max) );
    }

    template<>
    void configuration::parse_random<glm::vec3>(xml_node* node, const config_key& key)
    {
        // must have:
        // from: minimal value to generate
        // to:   maximum value to generate
        
        xml_attr* from = node->first_attribute("from");
        if(!from) 
        {
            throw parse_exception("Random must have 'from' defined.");
        }
        
        xml_attr* to = node->first_attribute("to");
        if(!to) 
        {
            throw parse_exception("Random must have 'to' defined.");
        }
        
        glm::vec3 min = value_parser<glm::vec3>::parse(from->value());
        glm::vec3 max = value_parser<glm::vec3>::parse(to->value());
        
        config_[key] = boost::shared_ptr< value_cfg<glm::vec3> >( new value_cfg<glm::vec3>(min, max) );
    }
    
    template<>
    void configuration::parse_random<glm::vec4>(xml_node* node, const config_key& key)
    {
        // must have:
        // from: minimal value to generate
        // to:   maximum value to generate
        
        xml_attr* from = node->first_attribute("from");
        if(!from)
        {
            throw parse_exception("Random must have 'from' defined.");
        }
        
        xml_attr* to = node->first_attribute("to");
        if(!to)
        {
            throw parse_exception("Random must have 'to' defined.");
        }
        
        glm::vec4 min = value_parser<glm::vec4>::parse(from->value());
        glm::vec4 max = value_parser<glm::vec4>::parse(to->value());
        
        config_[key] = boost::shared_ptr< value_cfg<glm::vec4> >( new value_cfg<glm::vec4>(min, max) );
    }
    
} // namespace config
} // namespace x2d