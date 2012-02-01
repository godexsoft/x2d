//
//  random.cpp
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"

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
            LOG("Parsing exception: Random must have 'from' defined.");
            throw std::exception();
        }
        
        xml_attr* to = node->first_attribute("to");
        if(!to) 
        {
            LOG("Parsing exception: Random must have 'to' defined.");
            throw std::exception();
        }
        
        std::stringstream ss;
        float min, max;
        
        ss << from->value() << " " << to->value();
        ss >> min >> max;
        
        config_[key] = boost::shared_ptr< value_cfg<float> >( new value_cfg<float>(res_man_, 
            boost::shared_ptr< random_cfg<float> >( new random_cfg<float>(res_man_, min, max) ) ) );
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
            LOG("Parsing exception: Random must have 'from' defined.");
            throw std::exception();
        }
        
        xml_attr* to = node->first_attribute("to");
        if(!to) 
        {
            LOG("Parsing exception: Random must have 'to' defined.");
            throw std::exception();
        }
        
        std::stringstream ss;
        int min, max;
        
        ss << from->value() << " " << to->value();
        ss >> min >> max;
        
        config_[key] = boost::shared_ptr< value_cfg<int> >( new value_cfg<int>(res_man_, 
            boost::shared_ptr< random_cfg<int> >( new random_cfg<int>(res_man_, min, max) ) ) );
    }

} // namespace config
} // namespace x2d