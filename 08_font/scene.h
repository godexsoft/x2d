//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __SCENE_H__
#define __SCENE_H__

#include "clock.h"
#include "timer.h"
#include "interpolator.h"
#include "graphics_engine.h"
#include "texture.h"
#include "liverpool_manager.h"
#include "resource_manager.h"
#include "sprite.h"
#include "configuration.h"
#include "font.h"
#include "fps_counter.h"
#include "custom_obj.h"

#include <vector>

class scene 
{
public:
    scene(kernel& k, configuration& conf)
    : config_(conf)
    {
//        std::vector<int> widths;

//        widths.push_back(30);
//        widths.push_back(14);
//        widths.push_back(23);
//        widths.push_back(22);
//        widths.push_back(22);
//        widths.push_back(24);
//        widths.push_back(23);
//        widths.push_back(22);
//        widths.push_back(22);
//        widths.push_back(24);
//        widths.push_back(34);
//        widths.push_back(28);
//        widths.push_back(35);
//        widths.push_back(35);
//        widths.push_back(31);
//        widths.push_back(35);
//        widths.push_back(35);
//        widths.push_back(37);
//        widths.push_back(20);
//        widths.push_back(26);
//        widths.push_back(33);
//        widths.push_back(33);
//        widths.push_back(47);
//        widths.push_back(35);
//        widths.push_back(33);
//        widths.push_back(38);
//        widths.push_back(34);
//        widths.push_back(38);
//        widths.push_back(35);
//        widths.push_back(34);
//        widths.push_back(33);
//        widths.push_back(32);
//        widths.push_back(42);
//        widths.push_back(36);
//        widths.push_back(35);
//        widths.push_back(33);
//        widths.push_back(26);
//        widths.push_back(19);
//        widths.push_back(22);
//        widths.push_back(21);
//        widths.push_back(23);
//        widths.push_back(23);
//        widths.push_back(22);
//        widths.push_back(23);
//        widths.push_back(15);
//        widths.push_back(21);
//        widths.push_back(24);
//        widths.push_back(20);
//        widths.push_back(34);
//        widths.push_back(23);
//        widths.push_back(20);
//        widths.push_back(26);
//        widths.push_back(21);
//        widths.push_back(26);
//        widths.push_back(24);
//        widths.push_back(26);
//        widths.push_back(23);
//        widths.push_back(21);
//        widths.push_back(32);
//        widths.push_back(23);
//        widths.push_back(23);
//        widths.push_back(24);
//
//        font_ = boost::shared_ptr<font>( new font("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
//            widths, 32, size(2,2), config_.get_object<texture>("graphics.degrassi") ) );
//
        
//        widths.push_back(4);
//        widths.push_back(4);
//        widths.push_back(5);
//        widths.push_back(14);
//        widths.push_back(11);
//        widths.push_back(19);
//        widths.push_back(14);
//        widths.push_back(3);
//        widths.push_back(6);
//        widths.push_back(5);
//        widths.push_back(8);
//        widths.push_back(11);
//        widths.push_back(3);
//        widths.push_back(6);
//        widths.push_back(3);
//        widths.push_back(10);
//        widths.push_back(12);
//        widths.push_back(8);
//        widths.push_back(12);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(12);
//        widths.push_back(12);        
//        widths.push_back(12);
//        widths.push_back(12);
//        widths.push_back(12);
//        widths.push_back(3);
//        widths.push_back(3);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(10);
//        widths.push_back(16);
//        widths.push_back(14);
//        widths.push_back(12);
//        widths.push_back(11);
//        widths.push_back(12);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(12);
//        widths.push_back(12);
//        widths.push_back(4);
//        widths.push_back(6);
//        widths.push_back(12);
//        widths.push_back(11);
//        widths.push_back(16);
//        widths.push_back(12);
//        widths.push_back(12);
//        widths.push_back(12);
//        widths.push_back(12);
//        widths.push_back(12);
//        widths.push_back(11);
//        widths.push_back(10);
//        widths.push_back(12);
//        widths.push_back(14);
//        widths.push_back(17);
//        widths.push_back(13);
//        widths.push_back(13);
//        widths.push_back(11);
//        widths.push_back(5);
//        widths.push_back(9);
//        widths.push_back(5);
//        widths.push_back(8);
//        widths.push_back(12);
//        widths.push_back(5);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(9);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(6);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(3);
//        widths.push_back(6);
//        widths.push_back(10);
//        widths.push_back(4);
//        widths.push_back(15);
//        widths.push_back(10);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(6);
//        widths.push_back(10);
//        widths.push_back(6);
//        widths.push_back(11);
//        widths.push_back(12);
//        widths.push_back(15);
//        widths.push_back(10);
//        widths.push_back(11);
//        widths.push_back(11);
//        widths.push_back(6);
//        widths.push_back(7);
//        widths.push_back(6);
//        widths.push_back(14);
//
//        font_ = boost::shared_ptr<font>( new font(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~",
//            widths, 24, size(2,2), config_.get_object<texture>("graphics.sony_sketch") ) );
        
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_.get_object<font>("fonts.sony_sketch") ) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
private:
    configuration&  config_;
    boost::shared_ptr<font> font_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
