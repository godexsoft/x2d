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

#include <ft2build.h>
#include FT_FREETYPE_H

#include "glm.hpp"
#include <png.h>

#define CHAR_CR ((uint32_t)'\r')
#define CHAR_LF ((uint32_t)'\n')

namespace po = boost::program_options;
using namespace std;

struct pixel
{
    pixel()
    : red(0)
    , green(0)
    , blue(0)
    , alpha(255)
    {
    }
    
    pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : red(r)
    , green(g)
    , blue(b)
    , alpha(a)
    {
    }
    
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

class bitmap
{
public:
    bitmap(uint32_t w, uint32_t h)
    : width_(w)
    , height_(h)
    , pixels_(w*h)
    {
    }
    
    void set_pixel(uint32_t x, uint32_t y, const pixel& p);
    pixel get_pixel(uint32_t x, uint32_t y) const;

    const uint32_t get_width() const { return width_; }
    const uint32_t get_height() const { return height_; }
    
private:
    vector<pixel>       pixels_;
    uint32_t            width_;
    uint32_t            height_;
};

void bitmap::set_pixel(uint32_t x, uint32_t y, const pixel& p)
{
    assert(x < width_ && y < height_);
    pixels_[y*width_ + x] = p;
}

pixel bitmap::get_pixel(uint32_t x, uint32_t y) const
{
    return pixels_[y*width_ + x];
}

struct glyph_info
{
    uint32_t index;
    uint32_t code_point;
    
    glyph_info(uint32_t i, uint32_t cp)
    : index(i)
    , code_point(cp)
    {
    }
};

class generator
{
public:
    generator(const string& fnt, const vector<string>& txt_lst, int sz, int pd, string output)
    : font_file_(fnt)
    , txt_files_(txt_lst)
    , char_size_(sz, sz)
    , padding_(pd * 2.0f)
    , font_name_(output)
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
    void save_image(const bitmap& bm);
    void save_config();
    
    map<uint32_t, glyph_info>    gen_;
    string                       font_file_;
    vector<string>               txt_files_;
    
    string          font_name_;
    glm::vec2       char_size_;
    glm::vec2       char_spacing_;
    float           font_scale_;
    float           padding_;
    
    FT_Library      font_library_;
    FT_Face         font_face_;
    
    stringstream    cfg_characters_;
    stringstream    cfg_widths_;
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
    
    uint8_t largest_width = 0;
    bool mono = false;
    bool static_advance = false;
    
    if(!mono)
    {
        for(map<uint32_t, glyph_info>::iterator it = gen_.begin(); it != gen_.end(); ++it)
        {
            glyph_info info = it->second;
            
            FT_Error err = FT_Load_Glyph(font_face_, info.index, FT_LOAD_RENDER);
            assert(!err);
            
            // load width of character
            uint8_t char_width = 0;
            
            if(static_advance)
            {
                char_width = glm::ceil(font_scale_ * font_face_->glyph->advance.x);
            }
            else
            {
                char_width = glm::max(font_face_->glyph->bitmap_left, 0) + font_face_->glyph->bitmap.width;
            }
            
            largest_width = glm::max(largest_width, char_width);
        }
        
        char_size_.x = largest_width;
        cout << "[debug] Updated font width to: " << char_size_.x << "\n";
    }
    
    float w = glm::floor( glm::sqrt( (float)gen_.size() ) );
    float h = glm::ceil( (float)gen_.size() / w );
    
    uint32_t width =  (w * (char_size_.x + padding_)) + (char_spacing_.x * glm::max( w - 1.0f, 0.0f ));
    uint32_t height = (h * (char_size_.y + padding_)) + (char_spacing_.y * glm::max( h - 1.0f, 0.0f ));
    
    if(!mono)
    {
        uint32_t x, y;
        x = y = 0;
        y += char_spacing_.y;
        
        for(map<uint32_t, glyph_info>::iterator it = gen_.begin(); it != gen_.end(); ++it)
        {
            glyph_info info = it->second;
            cfg_characters_ << (char)it->first;
            
            FT_Error err = FT_Load_Glyph(font_face_, info.index, FT_LOAD_RENDER);
            assert(!err);
            
            // load width of character
            uint8_t char_width = 0;
            
            if(static_advance)
            {
                char_width = glm::ceil(font_scale_ * font_face_->glyph->advance.x);
            }
            else
            {
                char_width = glm::max(font_face_->glyph->bitmap_left, 0) + font_face_->glyph->bitmap.width;
                
                if(char_width == 0)
                {
                    char_width = glm::ceil(font_scale_ * font_face_->glyph->advance.x);
                }
            }
            
            char_width = padding_ + ((char_width > 0) ? char_width : char_size_.x);
            
            // new line?
            if(x + char_width > width)
            {
                x = 0;
                y += char_size_.y + padding_ + char_spacing_.y;
            }
            
            x += char_width + char_spacing_.x;
        }
        
        height = y + char_size_.y + padding_ + char_spacing_.y;
    }
    
    cout << "[debug] width and height: " << width << "x" << height << "\n";
    
    uint32_t baseline = glm::ceil(font_scale_ * font_face_->ascender);
    cout << "[debug] baseline: " << baseline << "\n";
    
    bitmap bm(width, height);
    
    uint32_t idx = 0;
    uint32_t x = 0;
    uint32_t y = baseline;
    
    for(map<uint32_t, glyph_info>::iterator it = gen_.begin(); it != gen_.end(); ++it, ++idx)
    {
        glyph_info info = it->second;
        cfg_characters_ << (char)it->first;
        
        FT_Error err = FT_Load_Glyph(font_face_, info.index, FT_LOAD_RENDER);
        assert(!err);

        uint32_t char_width = 0;
        
        if(mono)
        {
            char_width = char_size_.x + padding_;
        }
        else
        {
            if(static_advance)
            {
                char_width = glm::ceil(font_scale_ * font_face_->glyph->advance.x);
            }
            else
            {
                char_width = glm::max(font_face_->glyph->bitmap_left, 0) + font_face_->glyph->bitmap.width;
                
                if(char_width == 0)
                {
                    char_width = glm::ceil(font_scale_ * font_face_->glyph->advance.x);
                }
            }
            
            char_width = padding_ + ((char_width > 0) ? char_width : char_size_.x);
            cfg_widths_ << char_width << " ";
        }
        
        // new line?
        if(x + char_width > width)
        {
            x = 0;
            y += char_size_.y + padding_ + char_spacing_.y;
        }
        
        if(font_face_->glyph->bitmap.buffer)
        {
            cout << "[debug] found bitmap data for glyph...\n";
            
            uint32_t adj_x = x + (0.5f * padding_) + glm::max(font_face_->glyph->bitmap_left, 0);
            uint32_t adj_y = y + (0.5f * padding_) - glm::min((uint32_t)font_face_->glyph->bitmap_top, baseline);

            uint8_t* src = (unsigned char *)(font_face_->glyph->bitmap.buffer);
            
            for(uint32_t i = 0; i < font_face_->glyph->bitmap.rows; ++i,
                src += glm::max(font_face_->glyph->bitmap.pitch, -font_face_->glyph->bitmap.pitch) - font_face_->glyph->bitmap.width)
            {
                for(uint32_t j = 0; j < font_face_->glyph->bitmap.width; j++, src++)
                {
                    bm.set_pixel(j + adj_x, i + adj_y, pixel(255, 255, 255, src[0]));
                }
            }
        }
        
        x += char_width + char_spacing_.x;        
    }
    
    // now we have a filled bitmap. need to save it into a file
    char_size_.x += padding_;
    char_size_.y += padding_;
    
    // time to save image and config
    cout << "[+] Writing files...\n";
    save_image(bm);
    save_config();
    
    return 0;
}

void generator::save_config()
{
    stringstream ss;
 
    ss << "<x2d version=\"1.0\">\n";
    
    ss << "    <texture n = \"" << font_name_ << "_sheet\" path = \""
       << "res/fonts/" << font_name_ << ".png\" />\n\n";
    
    ss << "    <font    n = \"" << font_name_ << "\"\n";
    ss << "        height = \"" << char_size_.y << "\"\n";
    ss << "       spacing = \"" << char_spacing_.x << " " << char_spacing_.y << "\"\n";
    ss << "       texture = \"" << font_name_ << "_sheet\"\n";
    ss << "    >\n";
    
    ss << "        <string n = \"characters\">\n";
    ss << "            <![CDATA[" << cfg_characters_.str() << "]]>\n";
    ss << "        </string>\n";
    ss << "        <string n = \"widths\">\n";
    ss << "            " << cfg_widths_.str() << "\n";
    ss << "        </string>\n";
    
    ss << "    </font>\n";
    ss << "</x2d>\n";
    
    ofstream out(string(font_name_ + ".xml").c_str());
    if(!out.is_open())
    {
        throw runtime_error("Couldn't open configuration output file. aborting.");
    }
    
    string str(ss.str());
    out.write(str.c_str(), str.size());
}

void generator::save_image(const bitmap& bm)
{
//    FILE* fp = fopen(string(font_name_ + ".png").c_str(), "wb");
    FILE* fp = fopen("/tmp/test.png", "wb");
    if(!fp)
    {
        throw runtime_error("Can't open png output file.");
    }
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte ** row_pointers = NULL;
    
    int status = -1;
    
    int pixel_size = 4;
    int depth = 8;
    
    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        throw runtime_error("Can't open png output structure.");
    }
    
    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL)
    {
        throw runtime_error("Can't create png info struct.");
    }
    
    png_set_IHDR (png_ptr, info_ptr, bm.get_width(), bm.get_height(), depth,
        PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    row_pointers = static_cast<png_byte**>(png_malloc (png_ptr, bm.get_height() * sizeof (png_byte *)));

    for (uint32_t y = 0; y < bm.get_height(); ++y)
    {
        png_byte *row =
            static_cast<png_byte*>(png_malloc (png_ptr, sizeof (uint8_t) * bm.get_width() * pixel_size));
        row_pointers[y] = row;
        for (uint32_t x = 0; x < bm.get_width(); ++x) {
            pixel p = bm.get_pixel(x, y);
            *row++ = p.red;
            *row++ = p.green;
            *row++ = p.blue;
            *row++ = p.alpha;
        }
    }
    
    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
    status = 0;
    
    for (size_t y = 0; y < bm.get_height(); y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    
    png_free (png_ptr, row_pointers);
    
    png_destroy_write_struct (&png_ptr, &info_ptr);
    fclose (fp);
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
    uint32_t char_code_point = c;

    if(!gen_.count(char_code_point))
    {
        if(char_code_point != CHAR_CR && char_code_point != CHAR_LF)
        {
            uint32_t glyph_index = FT_Get_Char_Index(font_face_, char_code_point);
            if(glyph_index)
            {
                cout << "[debug] Found glyph index " << glyph_index << " for character '" << (char)c << "'\n";
                
                // add to map
                gen_.insert( make_pair( char_code_point, glyph_info( glyph_index, char_code_point ) ) );
            }
        }
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
            // TODO: most likely this wont work for anything other than ascii. need some UTF-8 magic :)
            process_character(ifs.get());
        }
        
        cout << "[+] Done. Added " << gen_.size() << " characters.\n";
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
    ("output,o", po::value<std::string>()->default_value("x2d_font"), "output name")
    ("size,s", po::value<int>()->default_value(32), "height of characters")
    ("padding,p", po::value<int>()->default_value(0), "character padding")
    ("font,f", po::value<string>(), "input font file (.ttf)")
    ("text,t", po::value<vector<string> >()->composing(), "input text file")
    ("monospace,m", "monospaced font")
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
    generator gen(fnt, txt_files, size, padding, vm["output"].as<string>());
    return gen.run();
}
