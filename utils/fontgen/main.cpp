//
//  main.cpp
//  fontgen - x2d
//
//  Created by Alex Kremer on 03/11/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "glm.hpp"

namespace po = boost::program_options;
using namespace std;

class generator
{
public:
    generator(const string& fnt, const vector<string>& txt_lst, int sz, int pd)
    : font_file_(fnt)
    , txt_files_(txt_lst)
    , char_size_(sz, sz)
    , padding_(pd * 2.0f)
    {
    }
    
    ~generator()
    {
        if(font_face_)
        {
            FT_Done_Face(font_face_);
        }
        
        if(font_library_)
        {
            FT_Done_FreeType(font_library_);
        }
    }
    
    int run();
    
private:
    
    void load_font();
    void process_character(int c);
    void parse_text(const string& file);
    
    map<int, bool>  gen_;
    string          font_file_;
    vector<string>  txt_files_;
    
    string          font_name_;
    glm::vec2       char_size_;
    glm::vec2       char_spacing_;
    float           font_scale_;
    float           padding_;    
    
    FT_Library      font_library_;
    FT_Face         font_face_;
};

int generator::run()
{
    cout << "[+] Running generator...\n";
    
    cout << "    Font size:          " << char_size_.y << "\n";
    cout << "    Padding:            " << padding_ << "\n";
    cout << "    Font file:          " << font_file_ << "\n";

    for(vector<string>::iterator it = txt_files_.begin(); it != txt_files_.end(); ++it)
    {
        cout << "    Text file to parse: " << *it << "\n";
    }
    
    // open font
    load_font();
    
    // parse all text files to generate a map with glyphs
    for(vector<string>::iterator it = txt_files_.begin(); it != txt_files_.end(); ++it)
    {
        parse_text(*it);
    }
    
    return 0;
}

void generator::load_font()
{
    if(!FT_Init_FreeType( &font_library_ ))
    {
        if(!FT_New_Face(font_library_, font_file_.c_str(), 0, &font_face_))
        {
            if(!FT_Select_Charmap(font_face_, ft_encoding_unicode))
            {
                char_spacing_.x = 2.0f * glm::ceil(char_size_.x * (1.0f / 32.0f));
                char_spacing_.y = 2.0f * glm::ceil(char_size_.y * (1.0f / 32.0f));

                cout << "[?] Spacing: x=" << char_spacing_.x << " / y=" << char_spacing_.y << "\n";
               
                font_scale_ = char_size_.y / (font_face_->bbox.yMax - font_face_->bbox.yMin);
                
                cout << "[?] Setting pixel sizes: " << char_size_.x - 2 << " x " << (FT_UInt)char_size_.y - 2 << "\n";
                
                FT_Set_Pixel_Sizes(font_face_, (FT_UInt)char_size_.x - 2, (FT_UInt)char_size_.y - 2);
                cout << "[+] Font loaded.\n";
            }
        }
    }
}

void generator::process_character(int c)
{
    if(!gen_.count(c))
    {
        // TODO: process
        gen_.insert(make_pair(c, true));
    }
}

void generator::parse_text(const string& file)
{
    cout << "[+] Parsing text file: " << file << "...\n";
    ifstream ifs(file.c_str(), ifstream::in);
    
    if(ifs.is_open())
    {
        while(!ifs.eof())
        {
            process_character(ifs.get());
        }
        
        cout << "[+] Done.\n";
    }
    else
    {
        cout << "[-] Could'n open text file " << file << ".\n";
    }
}

int main(int argc, const char * argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("output,o", po::value<std::string>(), "output name")
    ("size,s", po::value<int>()->default_value(16), "height of characters")
    ("padding,p", po::value<int>()->default_value(2), "character padding")
    ("font,f", po::value<string>(), "input font file (.ttf)")
    ("text,t", po::value<vector<string> >()->composing(), "input text file")
    ("monospace,m", po::value<bool>(), "monospaced font")
    ("advance,a", po::value<int>(), "glyph advance value for non-monospace fonts")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    string fnt;
    vector<string> txt_files;
    int size;
    int padding;
    
    if (vm.count("help"))
    {
        cout << desc << "\n";
        return 1;
    }
    
    // check font file
    if (vm.count("font"))
    {
        fnt = vm["font"].as<string>();        
    }
    else
    {
        cout << "[-] Input font is not passed.\n";
        return 127;
    }
    
    // set size and padding
    size = vm["size"].as<int>();
    padding = vm["padding"].as<int>();
    
    // check input text files
    if (vm.count("text"))
    {
        txt_files = vm["text"].as<vector<string> >();
    }
    else
    {
        cout << "[-] Text files not specified. abort.\n";
        return 127;
    }
    
    // create a generator
    generator gen(fnt, txt_files, size, padding);
    return gen.run();
}
