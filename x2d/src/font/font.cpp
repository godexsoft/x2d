//
//  font.cpp
//  x2d
//
//  Created by Alex Kremer on 2/11/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "font.h"

namespace x2d {
namespace bitmapfont {
        
    font::font(const std::string& characters, std::vector<int> widths, 
               int height, const size& spacing, boost::shared_ptr<texture> txtr)
    : texture_(txtr)
    , spacing_(spacing)
    {       
        if(characters.size() != widths.size())
        {
            throw sys_exception("Font character count does not match widths count.");
        }
        
        int cur_y_offset = 0;
        int cur_x_offset = 0;
        
        for(int i=0; i<characters.size(); ++i)
        {
            size  sz(widths.at(i), height);
            
            if(cur_x_offset + sz.width > texture_->area().width)
            {
                cur_x_offset = 0;
                cur_y_offset += height + spacing_.height;
            }
            
            point orig(cur_x_offset, cur_y_offset);
            
            glyphs_.insert( std::make_pair(characters.at(i), 
                glyph(sz, boost::shared_ptr<sprite>( new sprite( texture_, orig, sz ) ) ) ) );
            
            LOG("[%c] Sprite inside glyph: %f %f %f %f", characters.at(i), orig.x, orig.y, sz.width, sz.height);                
            cur_x_offset += sz.width + spacing_.width;
        }
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
        
        for(int i=0; i<txt.size(); ++i)
        {
            std::map<char, glyph>::iterator it = glyphs_.find(txt.at(i));            
            if(it != glyphs_.end())
            {
                glEnable(GL_BLEND);
                glPushMatrix();
                
                cursor_x += it->second.size_.width/2;
                
                glTranslatef(cursor_x, 0.0, 0.0);
                it->second.spr_->draw();
                
                glPopMatrix();                                
                cursor_x += it->second.size_.width/2;
                
                glDisable(GL_BLEND);
            }
        }            
    }
    
    size font::calculate_size(const std::string& txt)
    {
        int cursor_x = 0;
        int max_height = 0;
        
        for(int i=0; i<txt.size(); ++i)
        {
            std::map<char, glyph>::iterator it = glyphs_.find(txt.at(i));            
            if(it != glyphs_.end())
            {
                cursor_x += it->second.size_.width;
                max_height = fmaxf(max_height, it->second.size_.height);
            }
        }             
                
        return size(cursor_x, max_height);
    }
        
} // bitmapfont
} // x2d
