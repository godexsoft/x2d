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
        
        config_[key] = boost::shared_ptr< value_cfg<float> >( new value_cfg<float>( 
            boost::shared_ptr< random_cfg<float> >( new random_cfg<float>(min, max) ) ) );
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
        
        config_[key] = boost::shared_ptr< value_cfg<int> >( new value_cfg<int>(
            boost::shared_ptr< random_cfg<int> >( new random_cfg<int>(min, max) ) ) );
    }

    template<>
    void configuration::parse_random<vector_2d>(xml_node* node, const config_key& key)
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
        
        vector_2d min = value_parser<vector_2d>::parse(from->value());
        vector_2d max = value_parser<vector_2d>::parse(to->value());
        
        config_[key] = boost::shared_ptr< value_cfg<vector_2d> >( new value_cfg<vector_2d>(
            boost::shared_ptr< random_cfg<vector_2d> >( new random_cfg<vector_2d>(min, max) ) ) );
    }

} // namespace config
} // namespace x2d