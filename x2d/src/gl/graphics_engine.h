//
//  graphics_engine.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_GRAPHICS_ENGINE_H__
#define __X2D_GRAPHICS_ENGINE_H__

#include <boost/pool/detail/singleton.hpp>
#include <boost/function.hpp>

#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_3_0
#include <OpenGLES/ES1/glext.h>
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#endif

namespace x2d {
namespace graphics {
        
class graphics_engine_bare
{
public:
    /**
     * Initialize graphics subsystem
     */
    void init(float x, float y, float width, float height);
    void shutdown();
    
    void create_frame_buffer(); 
    void destroy_frame_buffer();
    void set_current_context();
    void start_frame();    
    void clear();
    void present_frame();
    
    void set_render_buf_storage_cb( const boost::function<void()>& f )
    {
        render_buf_storage_ = f;
    }
    
    void set_cur_ctx_cb( const boost::function<void()>& f )
    {
        set_cur_ctx_ = f;
    }
    
    void set_present_render_buffer_cb( const boost::function<void()>& f )
    {
        present_render_buffer_ = f;
    }

    void set_shutdown_cb( const boost::function<void()>& f )
    {
        shutdown_ = f;
    }    
    
private:
    GLint backing_width_;
    GLint backing_height_;
    
    GLuint view_render_buf_;
    GLuint view_frame_buf_;
    GLuint depth_render_buf_;
    
    // callbacks for platform dependant stuff
    boost::function<void()> render_buf_storage_;
    boost::function<void()> set_cur_ctx_;
    boost::function<void()> present_render_buffer_;
    boost::function<void()> shutdown_;
};

typedef boost::details::pool::singleton_default<graphics_engine_bare> graphics_engine;
    
} // namespace graphics
} // namespace x2d
using namespace x2d::graphics;

#endif // __X2D_GRAPHICS_ENGINE_H__
