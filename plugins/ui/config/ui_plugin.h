//
//  ui_plugin.h
//  x2d
//
//  Created by Alex Kremer on 23/02/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_UI_PLUGIN_H__
#define __X2D_UI_PLUGIN_H__

#include "configuration.h"
#include "plugin.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <map>

namespace x2d {
    
// forward declare all widgets   
namespace ui {
namespace widget {
    
    class button;
    class label;
    
} // namespace widget
} // namespace ui
using namespace x2d::ui::widget;
    
namespace plugin {
namespace ui {
    
    /**
     * @brief Support for '<label>' configuration node.
     */ 
    class label_cfg
    : public cfg_base
    {
    public:  
        /**
         * @param[in] cfg     Configuration object
         * @param[in] text    Text for the label
         * @param[in] font    Font to use
         */
        label_cfg(configuration& cfg, const std::string& text, const std::string& font)
        : config_(cfg)
        , text_(text)
        , font_(font)
        {            
        }
        
        /**
         * Create if required and return a shared version of the label.
         */
        boost::shared_ptr<label> get();
        
    private:
        configuration&             config_;
        std::string                text_;
        std::string                font_;
        boost::weak_ptr<label>     inst_;
    };

    
    
    /**
     * @brief Support for '<button>' configuration node.
     */ 
    class button_cfg
    : public cfg_base
    {
    public:  
        /**
         * @param[in] cfg     Configuration object
         */
        button_cfg(configuration& cfg)
        : config_(cfg)
        {            
        }
        
        /**
         * Create if required and return a shared version of the button.
         */
        boost::shared_ptr<button> get();
        
    private:
        configuration&             config_;
        boost::weak_ptr<button>    inst_;
    };
    
    
    /**
     * @brief The UI plugin configuration module
     */    
    class ui_plugin
    : public x2d::config::plugin
    , public boost::enable_shared_from_this<ui_plugin>
    {
    public:
        ui_plugin(configuration& conf)
        : plugin(conf)
        , config_(conf)
        {      
        }
        
    protected:
        void load_extensions(std::map<std::string, boost::function<void(xml_node*,const config_key&)> >& parsers)
        {
            LOG("Loading UI plugin extensions...");

            parsers["label"]        = boost::bind(&ui_plugin::parse_label, shared_from_this(), _1, _2);
            parsers["button"]       = boost::bind(&ui_plugin::parse_button, shared_from_this(), _1, _2);
        }
        
    private:
        void parse_label(xml_node* node, const config_key& key);
        void parse_button(xml_node* node, const config_key& key);
        
        configuration&       config_;
    };
    
} // namespace ui
} // namespace plugin
} // namespace x2d
using namespace x2d::plugin::ui;

#endif // __X2D_UI_PLUGIN_H__
