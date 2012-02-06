//
//  posix_fs.h
//  x2d
//
//  Created by Alex Kremer on 1/26/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_FS_POSIX_H__
#define __X2D_FS_POSIX_H__

#include "log.h"

#include <dirent.h> // Filesystem access
#include <fstream>

namespace x2d {
namespace filesystem {
    
    typedef FILE fd_type;
    
    typedef basic_path<char, '/'> path;		
    typedef basic_directory_iterator<path> directory_iterator;
    
    /* 
     * Tell the compiler we are going to implement this
     */
    template<>
    directory_iterator::fs_obj_ptr directory_iterator::open_path( const directory_iterator::path_type& p );
    
    
    /**
     * @brief Wrapper for filesystem object
     */
    template<class _PathType>
    class fs_obj_impl : public fs_obj_base<_PathType> 
    {
    public:
        typedef _PathType path_type;
        typedef boost::shared_ptr<fs_obj_base<_PathType> > fs_obj_ptr;

        fs_obj_impl()
        : path_()
        , dir_handle_(NULL)
        , file_handle_(NULL)
        , ft_(type_undef)
        { 
        }

        /**
         * Construction from Dir handle.
         * @param[in] p Path
         * @param[in] d dir handle
         */
        fs_obj_impl(const path_type& p, DIR* d)
        : path_(p)
        , dir_handle_(d)
        , file_handle_(NULL)
        , ft_(type_dir)
        { 
        }

        /**
         * Construction from File handle.
         * @param[in] p Path
         * @param[in] f File handle
         */
        fs_obj_impl(const path_type& p, FILE* f)
        : path_(p)
        , dir_handle_(NULL)
        , file_handle_(f)
        , ft_(type_file)
        { 
        }
        
        ~fs_obj_impl() {
            if(dir_handle_) 
            {
                closedir(dir_handle_);   
            }
            if(file_handle_)
            {
                fclose(file_handle_);  
            }
        }
        
        inline const file_type type() const 
        {
            return ft_; 
        }

        inline const path_type path() const
        { 
            return path_; 
        }
        
        fs_obj_ptr next();
        
        bool operator !=( const fs_obj_impl& fso ) 
        {
            return (fso.dir_handle_ != this->dir_handle_ ||
                    fso.file_handle_ != this->file_handle_);				   
        }
        
    private:
        const path_type		path_;
        DIR*				dir_handle_;
        FILE*				file_handle_;
        file_type			ft_;
    };
    
    /**
     * @brief Implementation of fdstream in terms of FILE structure
     */
    template<>		
    class ifd_basic_stream<fd_type> 
    {        
    public:
        ifd_basic_stream(fd_type* fp, long offset=0, long size=0) 
        : fp_(fp)
        , getter_(0)
        , start_(offset)
        , size_(size)
        {
            seekg(getter_, std::ios_base::beg);				
        }
        
        long tellg() const 
        { 
            return getter_; 
        }
        
        void seekg(long off, std::ios_base::seekdir way=std::ios_base::cur) 
        { 
            switch (way) 
            {
                case std::ios_base::beg:
                    getter_ = off;
                    fseek(fp_, start_+getter_, SEEK_SET);
                    break;
                case std::ios_base::end:
                    getter_ = size_+off;
                    fseek(fp_, (start_+size_)+off, SEEK_SET);
                    break;
                    
                default:	// Cur
                    getter_ += off;
                    fseek(fp_, off, SEEK_CUR);
                    break;
            }
            
            if(getter_ < 0 || getter_ >= size_) 
            {
                close();
            }
        }	
        
        inline bool is_open() const 
        { 
            return fp_!=NULL; 
        }
        
        inline bool good() const 
        { 
            return fp_!=NULL; 
        }
        
        void close() 
        { 
            fclose(fp_); 
            fp_=NULL; 
        }
        
        inline const int peek() const 
        { 
            int c = fgetc(fp_); 
            ungetc(c, fp_); 
            return c; 
        }
        
        inline const int read(char *buf, long len) 
        { 
            if(getter_ + len >= size_) 
            {
                len = size_ - getter_;
            }
            
            int bytes = fread(buf, 1, len, fp_); 
            getter_ += bytes;				
            
            return bytes;
        }
        
        // Share the real file pointer
        inline fd_type* fd() 
        { 
            return fp_; 
        }
        
        inline const long offset() const 
        { 
            return start_; 
        }
        
        inline const long size() const 
        { 
            return size_; 
        }        
        
    private:
        fd_type    *fp_;
        long        getter_;
        long        start_;
        long        size_;
    };
    
    typedef ifd_basic_stream<fd_type> ifdstream;
    
} // namespace filesystem
} // namespace x2d

#endif // __X2D_FS_POSIX_H__