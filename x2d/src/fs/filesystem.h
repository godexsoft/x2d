//
//  filesystem.h
//  x2d
//
//  Created by Alex Kremer on 1/25/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_FILESYSTEM_H__
#define __X2D_FILESYSTEM_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include "log.h"

// Emulate boost::filesystem a little :)
namespace x2d {
namespace filesystem {
    
    /**
     * @brief A class to represent paths
     */
    template<class _CharType, _CharType _SEP>
    class basic_path 
    {
    public:
        typedef std::basic_string<_CharType> _StringType;
        typedef basic_path<_CharType, _SEP> path_type;

        /**
         * Construction from string
         * @param[in] p Path
         */
        basic_path(const _StringType& p) 
        : path_(p)
        { 
        }
        
        /**
         * Construction from bytes
         * @param[in] bytes Byte array
         */
        basic_path(const _CharType bytes[]) 
        : path_(bytes)
        { 
        }		
        
        /**
         * Empty path construction
         */
        basic_path() 
        : path_()
        { 
        }
        
        /**
         * Concatination operator
         */
        friend const path_type operator /(const path_type& p, const path_type& pp) 
        { 
            if(p.path_.empty())
            {
                return pp;
            }

            if(pp.path_.empty())
            {
                return p;
            }
            
            if(*p.path_.rbegin() == _SEP && *pp.path_.begin() == _SEP)
            {
                return path_type(p.path_.substr(0, p.path_.size()-1) + pp.path_);
            }
            
            if(*p.path_.rbegin() == _SEP || *pp.path_.begin() == _SEP)
            {
                return path_type(p.path_ + pp.path_);
            }
            
            return path_type(p.path_ + _SEP + pp.path_);				
        }
        
        const bool operator <(const path_type& p) const
        {
            return string() < p.string();
        }
        
        operator const std::string () const
        {
            return string();
        }
        
        /**
         * Get the last path component.
         * @return Last path component as string
         */
        const _StringType last_path_component() const 
        {
            size_t p = path_.rfind(_SEP);
            return path_.substr(p?p+1:p);
        }

        /**
         * Get path without last path component.
         * @return path without last path component
         */
        const path_type remove_last_path_component() const 
        {
            size_t p = path_.rfind(_SEP);
            
            if(p == _StringType::npos)
            {
                return _StringType(); // the whole string was the last path component
            }
            
            return path_type( path_.substr(0,p) );
        }
        
        /**
         * Convert back to string
         * @return String representation
         */
        inline const _StringType string() const 
        { 
            return path_; 
        }
        
    private:        
        _StringType	path_;	// String representation of the path
    };
    
    // File type
    typedef enum {
        type_undef = 0,
        type_file,
        type_dir,
    } file_type;    
    
    /**
     * @brief A basetype for fs_obj_impl
     */
    template<class _PathType>
    class fs_obj_base 
    {			
    public:			
        virtual const file_type type() const = 0;
        virtual const _PathType path() const = 0;
        virtual boost::shared_ptr<fs_obj_base<_PathType> > next() = 0;
    };			
    
    /**
     * @brief Directory iterator implementation
     */
    template<class _PathType>
    class basic_directory_iterator 
    : public std::iterator<std::input_iterator_tag, _PathType&, void, _PathType&, void>
    {			
        
    public:
        typedef _PathType path_type;
        typedef boost::shared_ptr<fs_obj_base<_PathType> > fs_obj_ptr;			
        
        // System access		
        static fs_obj_ptr open_path( const path_type& p );					
        static fs_obj_ptr next_object( fs_obj_ptr fso );									
        
        basic_directory_iterator( const _PathType& p ) 			
        : initial_fs_obj_( open_path(p) )  // Try to open the path
        , fs_obj_(next_object(initial_fs_obj_))
        , base_path_( p )
        { 
        }
        
        // Basically the END of iterations
        basic_directory_iterator()
        : initial_fs_obj_() // Null dir
        , fs_obj_() 
        , base_path_()
        { 
        }
        
        // Iterator ++
        void operator ++() 
        {			
            // Try to lookup and if null it will exit
            fs_obj_ = next_object(initial_fs_obj_);
        }
        
        bool operator !=(const basic_directory_iterator<_PathType>& it)
        {
            return this->fs_obj_ != it.fs_obj_;
        }			
        
        const basic_directory_iterator<_PathType>* const operator->()
        {
            return this;
        }		
        
        // Get status
        const file_type status() const 
        { 
            return this->fs_obj_->type(); 
        }
        
        const typename _PathType::_StringType leaf() const 
        {
            return this->fs_obj_->path().last_path_component();
        }
        
        const typename _PathType::_StringType path() const 
        {
            return base_path_.string();
        }
        
    private:
        
        file_type  ftype_;
        fs_obj_ptr initial_fs_obj_, fs_obj_;
        const _PathType base_path_;
    };
    
    inline bool is_directory(file_type t)
    {
        return t == type_dir;
    }
    
    // An input file stream which can be constructed from a file descriptor
    template<class _FDType>
    class ifd_basic_stream;		
    
} // namespace filesystem    
} // namespace x2d
using namespace x2d::filesystem;

// Include platform specific impl
#ifdef __APPLE__
#include "posix_fs.h"
#endif

#endif // __X2D_FILESYSTEM_H__
