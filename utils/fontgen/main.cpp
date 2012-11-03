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

namespace po = boost::program_options;
using namespace std;

class generator
{
public:
    generator(const string& fnt, const vector<string>& txt_lst)
    : font_file_(fnt)
    , txt_files_(txt_lst)
    {
    }
    
    int run();
    
private:
    
    void process_character(int c);
    void parse_text(const string& file);
    
    map<int, bool>  gen_;
    string          font_file_;
    vector<string>  txt_files_;
};

int generator::run()
{
    cout << "[+] Running generator...\n";
    
    // parse all text files to generate a map with glyphs
    for(vector<string>::iterator it = txt_files_.begin(); it != txt_files_.end(); ++it)
    {
        parse_text(*it);
    }
    
    return 0;
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
    ("size,s", po::value<int>(), "height of characters")
    ("padding,p", po::value<int>(), "character padding")
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
    
    if (vm.count("help"))
    {
        cout << desc << "\n";
        return 1;
    }
    
    // check font file
    if (vm.count("font"))
    {
        fnt = vm["font"].as<string>();        
        cout << "Input font file: " << fnt << ".\n";
    }
    else
    {
        cout << "[-] Input font is not passed.\n";
        return 127;
    }
    
    // check input text files
    if (vm.count("text"))
    {
        txt_files = vm["text"].as<vector<string> >();
        for(vector<string>::iterator it = txt_files.begin(); it != txt_files.end(); ++it)
        {
            cout << "[+] Text file to parse: " << *it << "\n";
        }
    }
    else
    {
        cout << "[-] Text files not specified. abort.\n";
        return 127;
    }
    
    // create a generator
    generator gen(fnt, txt_files);
    return gen.run();
}
