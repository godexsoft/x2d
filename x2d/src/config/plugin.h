//
//  plugin.h
//  x2d
//
//  Created by Alex Kremer on 23/02/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_PLUGIN_H__
#define __X2D_PLUGIN_H__

#include <map>
#include <boost/function.hpp>
#include "configuration.h"

namespace x2d {
namespace config {
     
    class configuration;
    
    /**
     * @brief Base for all plugins
     */
    class plugin
    { 
        friend class configuration;
        
    public:
        plugin(configuration& conf)
        : config_(conf)
        {            
        }
        
    protected:
        /**
         * Override this method to load plugin's extensions (parsers).
         */
        virtual void load_extensions(std::map<std::string, boost::function<void(xml_node*,const config_key&)> >& parsers) = 0;
        
        /**
         * Sets a configuration entry
         */
        void add_to_config(const config_key& key, const boost::shared_ptr<cfg_base>& obj);
   
    private:
        configuration& config_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_PLUGIN_H__
