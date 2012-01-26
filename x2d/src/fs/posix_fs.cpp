//
//  posix_fs.cpp
//  x2d
//
//  Created by Alex Kremer on 1/26/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "filesystem.h"

namespace x2d { 
namespace filesystem {
	
	template<>
	fs_obj_impl<path>::fs_obj_ptr fs_obj_impl<path>::next() 
    {  		
		if( this->dir_handle_ ) 
        {
			struct dirent * entry = readdir(this->dir_handle_);
			if(entry) 
            {
				return directory_iterator::open_path(path_ / entry->d_name);
			}
		}
		
		return fs_obj_ptr(); // empty
	}
	
	template<>
	directory_iterator::fs_obj_ptr directory_iterator::open_path( const directory_iterator::path_type& p )
    {		
		std::string pth = p.string();
		
		DIR *d = opendir(pth.c_str());
		if(d) 
        { 
			return directory_iterator::fs_obj_ptr( new fs_obj_impl<directory_iterator::path_type>(p, d) );
		}
		
		FILE *f = fopen(pth.c_str(), "r");
		if(f) 
        {        
			return directory_iterator::fs_obj_ptr( new fs_obj_impl<directory_iterator::path_type>(p, f) );
		} 
		
		return directory_iterator::fs_obj_ptr(); // empty
	}
	
	template<>
	directory_iterator::fs_obj_ptr basic_directory_iterator<path>::next_object( directory_iterator::fs_obj_ptr fso ) 
    {
		if(fso->type() == type_dir) 
        {
			return fso->next();
		} 
		
		return directory_iterator::fs_obj_ptr(); // That's it
	}
		
} // namespace filesystem
} // namespace x2d
