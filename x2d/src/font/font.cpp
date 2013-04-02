//
//  font.cpp
//  x2d
//
//  Created by Alex Kremer on 2/11/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "font.h"
#include "utf8.h"

namespace x2d {
namespace bitmapfont {
    
    void glyph::init(const boost::shared_ptr<texture>& txtr)
    {
        spr_ = boost::shared_ptr<sprite>( new sprite( txtr, point(x_, y_), size(width_, height_) ) );
    }
    
    font::font(short sz, short baseline, short line_height,
                 short scale_width, short scale_height, short stretch_height,
                 const glm::vec4& padding, const size& spacing)
    : size_(sz)
    , baseline_(baseline)
    , line_height_(line_height)
    , scale_width_(scale_width)
    , scale_height_(scale_height)
    , stretch_height_(stretch_height)
    , padding_(padding)
    , spacing_(spacing)
    {
    }

    void font::add_page(const boost::shared_ptr<texture>& txtr)
    {
        pages_.push_back(txtr);
    }

    void font::set_glyphs(const std::vector<glyph>& glyphs)
    {
        for(std::vector<glyph>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it)
        {
            glyph g(*it);
            g.init(pages_.at( g.page_id_ ));

            glyphs_.insert( std::make_pair((*it).id_, g) );
        }
    }
    
    void font::set_kernings(const kern_map& kerns)
    {
        kernings_ = kerns;
    }
    
    int font::get_kerning_amount(int first, int second)
    {
        kern_map::iterator it = kernings_.find(first);
        if (it != kernings_.end())
        {
            std::map<int, int>::iterator jt = it->second.find(second);
            if (jt != it->second.end())
            {
                return jt->second;
            }
        }
        
        return 0;
    }
    
    void font::print(const std::string& txt, alignment align)
    {
        if(align == RIGHT_ALIGN)
        {
            size s = calculate_size(txt);
            glTranslatef(-s.width, 0, 0);
        }
        else if(align == CENTER_ALIGN)
        {
            size s = calculate_size(txt);
            glTranslatef(-s.width/2, 0, 0);
        }
        
        int cursor_x = 0;
        int len = utf8::distance(txt.begin(), txt.end());
        
        std::string::const_iterator cit = txt.begin();
        
        for (int i = 0; i < len; ++i)
        {
            int c = utf8::next(cit, txt.end());
            
            std::map<int, glyph>::iterator it = glyphs_.find(c);
            if(it != glyphs_.end())
            {
                glEnable(GL_BLEND);
                glPushMatrix();
                
                cursor_x += it->second.x_advance_/2.0f;
                
                if (len > 1 && i < len - 1)
                {
                    cursor_x += get_kerning_amount(c, utf8::peek_next(cit, txt.end()));
                }
                
                float y = (line_height_ - it->second.height_) / 2.0f;
                y -= it->second.y_offset_;
                float x = cursor_x + it->second.x_offset_;
                                
                glTranslatef(x, y, 0.0);
                it->second.spr_->draw();
                
                glPopMatrix();                                
                cursor_x += it->second.x_advance_/2.0f;
                
                glDisable(GL_BLEND);
            }
        }
    }
    
    size font::calculate_size(const std::string& txt)
    {
        float width = 0.0f;
        float total_width = 0.0f;
        int len = utf8::distance(txt.begin(), txt.end());
        
        std::string::const_iterator cit = txt.begin();
        
        for (int i = 0; i < len; ++i)
        {
            int c = utf8::next(cit, txt.end());
            
            std::map<int, glyph>::iterator it = glyphs_.find(c);
            if(it != glyphs_.end())
            {
                width += it->second.x_advance_;
                if (len > 1 && i < len - 1)
                {
                    width += get_kerning_amount(c, utf8::peek_next(cit, txt.end()));
                }
            }
        }
        
        total_width = fmaxf(total_width, width);
        return CGSizeMake(total_width, line_height_);
    }
        
} // bitmapfont
} // x2d
