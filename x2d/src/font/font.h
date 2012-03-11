//
//  font.h
//  x2d
//
//  Created by Alex Kremer on 2/11/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_FONT_H__
#define __X2D_FONT_H__

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "exceptions.h"
#include "sprite.h"
#include "math_util.h"
#include "log.h"

namespace x2d {
namespace bitmapfont {

    /**
     * @brief Minimal glyph wrapper. Wraps a sprite.
     */
    class glyph 
    {
    public:
        friend class font;
        
        glyph(const size& sz, const boost::shared_ptr<sprite>& s)
        : size_(sz)
        , spr_(s)
        {
        }
        
    private:
        size                        size_;
        boost::shared_ptr<sprite>   spr_;
    };
    
    /**
     * @brief Simple bitmap font
     */
    class font
    {
    public:
        font(const std::string& characters, std::vector<int> widths, 
             int height, const size& spacing, boost::shared_ptr<texture> txtr);
        
        size calculate_size(const std::string& txt);
        void print(const std::string& txt, alignment align=LEFT_ALIGN);
        
    private:
        boost::shared_ptr<texture>     texture_;
        size                           spacing_;
        std::map<char, glyph>          glyphs_;  // TODO: utf8 support!
    };
    
} // bitmapfont
} // x2d
using namespace x2d::bitmapfont;

#endif // __X2D_FONT_H__
