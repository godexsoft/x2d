//
//  configuration.cpp
//  x2d
//
//  Created by Alex Kremer on 1/30/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"
#include "sprite.h"

namespace x2d {
namespace config {
        
    configuration::configuration(resource_manager& res_man, const std::string& cfg_path)
    : res_man_(res_man)
    {
        config_["txtr"] = boost::shared_ptr<texture_cfg>( new texture_cfg("res/graphics/scenary.png") );
        config_["txtr.cafe"] = boost::shared_ptr<sprite_cfg>( new sprite_cfg("res/graphics/scenary.png", point(0, 0), size(158, 140)) );
    }
    
    template <>
    resource_ptr<sprite> configuration::get<sprite>(const std::string& key)
    {
        sprite_cfg cfg = *static_cast<sprite_cfg*>(&(*config_[key]));
        return resource_ptr<sprite>( new sprite( res_man_.get<texture>(cfg.texture_), cfg.origin_, cfg.size_) );
    }

    
} // namespace config    
} // namespace x2d