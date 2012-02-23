//
//  ui_plugin.cpp
//  x2d
//
//  Created by Alex Kremer on 23/02/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "ui_plugin.h"

#include "label.h"

namespace x2d {
namespace plugin {
namespace ui {
    
    void ui_plugin::parse_label(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        // text:     text for the label
        
        std::string text = configuration::get_mandatory_attr<std::string>(config_, node, key, "text", 
            parse_exception("UI::Label type must have 'text' defined.")).get(config_);

        std::string font = configuration::get_mandatory_attr<std::string>(config_, node, key, "font", 
            parse_exception("UI::Label type must have 'font' defined.")).get(config_);
        
        boost::shared_ptr<label_cfg> lbl = 
            boost::shared_ptr<label_cfg>( new label_cfg(config_, text, font) );
        
        add_to_config(key, lbl);
    }

    void ui_plugin::parse_button(xml_node* node, const config_key& key)
    {
        // must have:
        // n:        name of the element
        
        boost::shared_ptr<button_cfg> btn = 
            boost::shared_ptr<button_cfg>( new button_cfg(config_) );
        
        add_to_config(key, btn);
    }
    
    
    
    boost::shared_ptr<label> label_cfg::get()
    {
        if( boost::shared_ptr<label> p = inst_.lock() )
        {            
            // already exists outside of cfg
            return p;
        }
        else
        {
            boost::shared_ptr<label> l = 
                boost::shared_ptr<label>(
                    new label( config_, text_, config_.get_object<font>(font_) ) );
            inst_ = l;
            return l;
        }
    }
    
    
} // namespace ui
} // namespace plugin
    
namespace config {
    
    template <>
    boost::shared_ptr<label> configuration::get_object<label>(const config_key& key)
    {
        return static_cast<label_cfg*>( &(*config_[key]) )->get();
    }
    
} // namespace config
} // namespace x2d
