//
//  liverpool.h
//  Liverpool (now part of x2d)
//
//  Created by Alex Kremer on 9/14/10.
//  Copyright 2010-2012 godexsoft. All rights reserved.
//

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "filesystem.h"
#include "log.h"

#pragma once
#ifndef __X2D_LIVERPOOL_H__
#define __X2D_LIVERPOOL_H__

namespace x2d {
namespace liverpool {

	/**
	 * @brief Represents a single entry - a directory or a file.
	 */
	class entry 
    {
	public:
		typedef enum 
        {
			type_undef = 0,
			type_dir,
			type_file
		} entry_type;
		
	private:
		entry_type   type_;	    // Type - dir or file			
		unsigned int offset_;	// Offset starting from global data pointer			
		unsigned int size_;		// Size of file in bytes
		unsigned int depth_;	// The depth of this item relative to the root ('/')
		std::string  path_;			
		
	public:
        /**
         * @param[in] path  Path to file
         * @param[in] ofst  Offset inside liverpool zip archive
         * @param[in] sz    Size of the data block representing the entry
         */
		entry(const std::string& path, int ofst, int sz)
		: type_( *path.rbegin()=='/'? type_dir : type_file )
		, offset_(ofst)
		, size_(sz)  
		, depth_(std::count(path.begin(), path.end(), '/')-(type_==type_dir?1:0))
		, path_(path)
		{ 
			// Add a front slash to the path (the root folder)
			path_.insert(path_.begin(), '/');
			
			// Remove last slash in path for directories
			if(type_ == type_dir) 
            {
				path_.erase(path_.rfind('/'));
			}
		};	
		
		inline const entry_type& type() const { return type_; };
		inline bool is_dir() const { return type_ == type_dir; };
		inline unsigned int offset() const { return offset_; };
		inline unsigned int size() const { return size_; };
		inline unsigned int depth() const { return depth_; };
		inline const std::string path() const { return path_; };
	};	
	
	/**
	 * @brief Main interface to zip filesystem
	 */
	class liverpool {
	private:
		typedef	std::vector<entry>	entry_vec;
		
		entry_vec       entries_;				
        ifdstream       fs_stream_;
		
        void init();
        inline const ifdstream open_fs(const path& p);
        
        /**
         * Read bytes from the stream and return them as integer
         */
		unsigned int read_int(int bytes);
        
		void read_local_file_header();
		entry read_central_dir_record();
		
	public:
		
        /**
         * Open zip using file descriptor stream
         */
		liverpool(const ifdstream& fds);
        
        /**
         * Open zip using a path
         */
        liverpool(const path& p);
		
		~liverpool() 
        {
			if(fs_stream_.is_open()) 
            {
				fs_stream_.close();
			}
		}
				
		/**
         * @brief Access as a filesystem
         */
		template<int _OFFSET>
		class filter : public std::unary_function<entry, bool> 
        {
		private:
			std::string& path_;
			unsigned int depth_;
			
		public:
            /**
             * @param[in] path  Path to file
             */
			filter(std::string& path) 
			: path_(path)
			{
				// This is a directory?
				if(*path_.rbegin()=='/') 
                {
					path_.erase(path_.rfind('/'));
				}
				
				depth_ = std::count(path_.begin(), path_.end(), '/')-_OFFSET;	
			}
			
			bool operator ()(const entry& e) 
            {
				return e.depth() == depth_ && e.path().find(path_) == 0;
			}
		};

		typedef filter<1> file_filter;
		typedef filter<0> dir_filter;
		
		/**
		 * @brief Iterator class to allow stl algo usage and iteration
		 */
		template<class _FT>
 		class basic_iterator 
        {
		public:
			basic_iterator(liverpool* l) 
			: l_(l)
			, path_("")	// Empty path - always nonexisting
			, current_(l_->entries_.end())
			{ 
            }

			basic_iterator(liverpool* l, std::string& path) 
			: l_(l)
			, path_(path)
			, current_( std::find_if(l_->entries_.begin(), 
									 l_->entries_.end(), 
									 _FT(path) ))
			{
            }
			
			void operator++ () 
            {
				current_ = std::find_if(current_+1, 
										l_->entries_.end(), 
										_FT(path_));
			}
			
			bool operator!= (const basic_iterator& it) 
            {
				return it.current_ != this->current_;
			}
			
			entry* operator->() 
            {
				return &*current_;
			}
			
			const entry operator* () 
            {
				return *current_;
			}
            
        private:
			liverpool * l_;
			std::string path_;
			entry_vec::iterator current_;
		};
		
		typedef basic_iterator<dir_filter> diterator;
		typedef basic_iterator<file_filter> iterator;
		
		friend class basic_iterator<dir_filter>;
		friend class basic_iterator<file_filter>;
		
		iterator begin(const std::string& path) 
        {
			return iterator(this, const_cast<std::string&>(path));
		}
		
		iterator end() 
        {
			return iterator(this);
		}
		
		diterator dir_begin(const std::string& path) 
        {
			return diterator(this, const_cast<std::string&>(path));
		}
		
		diterator dir_end() 
        {
			return diterator(this);
		}
				
		/**
         * Obtain a stream to file data
         * @param[in] path The file path inside the zip
         */
		ifdstream open_stream(const std::string& path) 
        {			
			// First we need to get the entry
			iterator it = begin(path);
			if(it != end()) 
            {				
				entry e = *it;
				if( !e.is_dir() ) 
                {					
					
                    return ifdstream(
                        fs_stream_.fd(), 
                            fs_stream_.offset() + e.offset(), 
                                e.size() );
					
				} else {
					LOG("Given path is a directory. Can't open a stream to a directory.");
					throw std::exception();
				}
				
			} else {
				LOG("Couldn't open file '%s' in lvp archieve.", path.c_str());
				throw std::exception();
			}
		}		
	};
	
    typedef boost::shared_ptr<x2d::liverpool::liverpool> liverpool_ptr;
    
} // namespace liverpool
} // namespace x2d
using namespace x2d::liverpool;

#endif // __X2D_LIVERPOOL_H__