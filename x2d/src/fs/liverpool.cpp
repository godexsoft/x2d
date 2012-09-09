//
//  liverpool.cpp
//  Liverpool (now part of x2d)
//
//  Created by Alex Kremer on 9/14/10.
//  Copyright 2010-2012 godexsoft. All rights reserved.
//

#include "liverpool.h"

#include <string>
#include <exception>
#include "platform.h"

#ifdef ANDROID
#include <android/asset_manager.h>
extern AAssetManager* g_assetManager;
#endif

using namespace std;

namespace x2d {
namespace liverpool {
	
    void liverpool::init()
    {
        if( !fs_stream_.good() ) 
        {
			throw exception();
		}
		
		// First find the zip end of central directory
		//		 0	4	End of central directory signature = 0x06054b50
		//		 4	2	Number of this disk
		//		 6	2	Disk where central directory starts
		//		 8	2	Number of central directory records on this disk
		//		10	2	Total number of central directory records
		//		12	4	Size of central directory (bytes)
		//		16	4	Offset of start of central directory, relative to start of archive
		//		20	2	ZIP file comment length (n)
		//		22	n	ZIP file comment
		
		fs_stream_.seekg(-22, ios_base::end); // Start where it should be if there is no comment
		
		bool found = false;
		int i = fs_stream_.tellg();
		
		while ( i > 0 && fs_stream_.good() ) 
        {
			if (fs_stream_.peek() == 0x50) 
            {
				if( read_int(4) == 0x06054b50 ) 
                {
					found = true;
					break;
				}
			}
            
			fs_stream_.seekg(-1);
			--i;
		}
        
		if(!found) 
        {
			LOG("Invalid zip..");
			throw exception();
		}
		
		// Now read the rest of the directory ending
		fs_stream_.seekg(6, ios_base::cur);
		
		// Get number of central directory records
		unsigned int total_dirs = read_int(2);
		
		// Get size of central directory
		// unsigned int sizeOfDirectory = read_int(4);
		fs_stream_.seekg(4, ios_base::cur);
		
		// and the offset
		unsigned int offset_of_dir = read_int(4);
		
		// Comment
		int comment_len = read_int(2);		
#ifdef DEBUG
		if(comment_len) 
        {
			char * buf = new char[comment_len+1];
			
			fs_stream_.read(buf, comment_len);
			buf[comment_len] = 0;					
			
			LOG("File comment: '%s'", buf);
			delete [] buf;
		}
#endif // DEBUG
		
		// Jump over to the central directory
		fs_stream_.seekg(offset_of_dir, ios_base::beg);
		
		for( ; total_dirs > 0; --total_dirs ) 
        {
			// Get the directory record
			entry e = read_central_dir_record();
			entries_.push_back( e );
		}	
    }
    
	liverpool::liverpool(const ifdstream& fds) 
	: fs_stream_(fds)
	{
        init();
	}
    
    liverpool::liverpool(const path& p)
    : fs_stream_(open_fs(p))
    {
        init();
    }
	
    // FIXME: not cross-platform. POSIX only. Move code to platform or FS?
    const ifdstream liverpool::open_fs(const path& p)
    {   
// TODO: make ifdstream a friend of the file class. implement a generic file wrapper
// the wrapper will share a pointer to FILE* (friend) with ifdstream.
// the FILE* is managed by the file class and therefore it closes the file handle in the destructor.
// so to use an ifdstream you either have to keep a copy of the file object or manage fp yourself
//      file f( path_for_resource( p.string() ) );
//      return f.ifd_stream();
        
#ifdef ANDROID
    	AAsset* asset = AAssetManager_open(g_assetManager, p.string().c_str(), AASSET_MODE_UNKNOWN);
    	if (asset == NULL) {
    	  LOG("COULD NOT OPEN ASSET");
    	}
    	// open asset as file descriptor
    	off_t start, length;
    	int fd = AAsset_openFileDescriptor(asset, &start, &length);
    	if(fd < 0)
    	{
    	  LOG("COULD NOT OPEN FILE DESCRIPTOR");
    	}

    	FILE* fp = fdopen( fd, "rb" );
    	if(!fp)
    	{
    		LOG("Could not open a file at '%s'", p.string().c_str());
    	    throw std::exception();
    	}

       	AAsset_close(asset);
    	return ifdstream(fp, start, length);


#else
        // first read the resources zip
        std::string pth = path_for_resource( p.string() );
        FILE* fp = fopen( pth.c_str(), "rb" );
        if(!fp)
        {
            LOG("Could not open a file at '%s'", pth.c_str());
            throw std::exception();
        }
        
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        return ifdstream(fp, 0, size);
#endif
    }
    
	//	 0	4	Central directory file header signature = 0x02014b50
	//	 4	2	Version made by
	//	 6	2	Version needed to extract (minimum)
	//	 8	2	General purpose bit flag
	//	10	2	Compression method
	//	12	2	File last modification time
	//	14	2	File last modification date
	//	16	4	CRC-32
	//	20	4	Compressed size
	//	24	4	Uncompressed size
	//	28	2	File name length (n)
	//	30	2	Extra field length (m)
	//	32	2	File comment length (k)
	//	34	2	Disk number where file starts
	//	36	2	Internal file attributes
	//	38	4	External file attributes
	//	42	4	Relative offset of local file header
	//	46	n	File name
	//	46+n	m	Extra field
	//	46+n+m	k	File comment	
	entry liverpool::read_central_dir_record() {
		// Read signature
		if( read_int(4) != 0x02014b50 ) 
        {
			LOG("OOOPS.. bad directory record!");
			throw exception();
		}
		
		// Skip 6 bytes - version and general purpose flag
		fs_stream_.seekg(6, ios_base::cur);
		
		// Get compression method - we can work with uncompressed only
		if(0 != read_int(2)) 
        {
			LOG("Compression failure..");
			throw exception();
		}
		
		// Skip till size
		fs_stream_.seekg(8, ios_base::cur);

		// Get size
		unsigned int compressed   = read_int(4);
		unsigned int uncompressed = read_int(4);
        
		if(compressed != uncompressed) 
        {
			LOG("File compressed/uncompressed size is not equal");
			throw exception();
		}
		
		// Get length and name
		unsigned int len         = read_int(2);
		unsigned int extra_len   = read_int(2);
		unsigned int comment_len = read_int(2);
		
		// Skip 8
		fs_stream_.seekg(8, ios_base::cur);
		
		// Get offset
		int offset = read_int(4);
		
		string file_name;
		
		if(len > 0) 
        {
			char * buf = new char[len+1];		
			
			fs_stream_.read(buf, len);
			buf[len] = 0;
						
			file_name = buf;				
			delete [] buf;
		}
		
		if(extra_len) 
        {
			fs_stream_.seekg(extra_len, ios_base::cur);
		}
		
		// Check comment
		if(comment_len > 0) 
        {
			char * buf = new char[comment_len+1];
			
			fs_stream_.read(buf, comment_len);
			buf[comment_len] = 0;
			
			LOG("Comment: '%s'", buf);
			delete [] buf;
		}
		
		// Now the offset we have here points to the local file header, not to the data
		// So we actually want to read it once and later just jump to the file data directly
		unsigned int position = fs_stream_.tellg();
		fs_stream_.seekg(offset, ios_base::beg);        // Jump to the local header
		
		read_local_file_header();
		offset = fs_stream_.tellg();                    // This is the data offset		
		fs_stream_.seekg(position, ios_base::beg);      // Restore old position
		
		return entry(file_name, offset, compressed);    // The size will be updated later
	}

	// Zip local file header
	// 	 0	4	Local file header signature = 0x04034b50
	//	 4	2	Version needed to extract (minimum)
	//	 6	2	General purpose bit flag
	//	 8	2	Compression method
	//	10	2	File last modification time
	//	12	2	File last modification date
	//	14	4	CRC-32
	//	18	4	Compressed size
	//	22	4	Uncompressed size
	//	26	2	File name length (n)
	//	28	2	Extra field length (m)
	//	30	n	File name
	//	30+n	m	Extra field	
	void liverpool::read_local_file_header()
    {
		// Read signature
		if( read_int(4) != 0x04034b50 ) 
        {
			LOG("OOOPS.. bad local file header");
			throw exception();
		}
		
		// Skip 4 bytes - version and general purpose flag
		fs_stream_.seekg(4, ios_base::cur);
		
		// Get compression method - we can work with uncompressed only
		if(0 != read_int(2)) 
        {
			LOG("Compression failure..");
			throw exception();
		}
		
		// Skip till len
		fs_stream_.seekg(16, ios_base::cur);		
		
		// Get length and name
		unsigned int len = read_int(2);
		unsigned int extra_len = read_int(2);
		
		// Skip both
		if(len) 
        {
			fs_stream_.seekg(len, ios_base::cur);
		}
		
		if(extra_len) 
        {
			fs_stream_.seekg(extra_len, ios_base::cur);
		}
	}
		
	unsigned int liverpool::read_int(int bytes) 
    {
		// Read data into the buffer
		char* data = new char[bytes];
		fs_stream_.read(data, bytes);
		
		unsigned int result = 0;
		unsigned char* p = reinterpret_cast<unsigned char*>(data);

		
		switch(bytes) {
			case 4:
				result |= (unsigned int)(p[bytes - 4]);
			case 3:
				result |= (unsigned int)(p[bytes - 3]) << 8;
			case 2:
				result |= (unsigned int)(p[bytes - 2]) << 16;
			case 1:
				result |= (unsigned int)(p[bytes - 1]) << 24;
			case 0:
				break;
		}

		delete [] data;
		return result >> ((4 - bytes)*8);		
	};
		
} // namespace liverpool
} // namespace x2d

