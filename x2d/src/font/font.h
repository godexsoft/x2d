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

        glyph(int idx, short x, short y, short width, short height,
                short xoff, short yoff, short xadv, short page_id,
                const std::string& letter)
        : id_(idx)
        , x_(x)
        , y_(y)
        , width_(width)
        , height_(height)
        , x_offset_(xoff)
        , y_offset_(yoff)
        , x_advance_(xadv)
        , page_id_(page_id)
        , letter_(letter)
        {
        }

    private:

        void init(const boost::shared_ptr<texture>& txtr);

        int id_;
        short x_;
        short y_;
        short width_;
        short height_;
        short x_offset_;
        short y_offset_;
        short x_advance_;
        short page_id_;
        std::string letter_;

        // the sprite will be autocreated on font instantiation
        boost::shared_ptr<sprite> spr_;
    };

    typedef std::map<int, std::map<int, int> > kern_map;
    
    /**
     * @brief Simple bitmap font
     */
    class font
    {
    public:
        font(short sz, short baseline, short line_height,
             short scale_width, short scale_height, short stretch_height,
             const glm::vec4& padding, const size& spacing);

        void add_page(const boost::shared_ptr<texture>& txtr);
        void set_glyphs(const std::vector<glyph>& glyphs);
        void set_kernings(const kern_map& kerns);

        int get_kerning_amount(int first, int second);
        
        size calculate_size(const std::string& txt);

        void print(const std::string& txt, alignment align = LEFT_ALIGN);

    private:
        short size_;
        short baseline_;
        short line_height_;
        short scale_width_;
        short scale_height_;
        short stretch_height_;

        glm::vec4 padding_;
        size spacing_;

        std::vector<boost::shared_ptr<texture> > pages_;
        std::map<int, glyph> glyphs_;
        kern_map kernings_;
    };

} // bitmapfont
} // x2d
using namespace x2d::bitmapfont;

#endif // __X2D_FONT_H__
