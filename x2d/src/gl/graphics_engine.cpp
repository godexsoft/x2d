//
//  graphics_engine.cpp
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "graphics_engine.h"
#include <math.h>
#include "log.h"

namespace x2d {
namespace graphics {

    void graphics_engine_bare::init(float x, float y, float width, float height)
    {
        // set up OpenGL projection matrix
        glMatrixMode(GL_PROJECTION);
        
        glViewport(0, 0, width, height);
        glScissor(0, 0, width, height);

#ifdef ES1_GLEXT_H_GUARD
        glOrthof(0, width, 0, height, -1, 100);
#elif defined (_OPENGL_H)
        glOrtho(0, width, 0, height, -1, 100);
#endif
        
        glMatrixMode(GL_MODELVIEW);
        
        // initialize OpenGL states
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    // multiply current color and texture color
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glLoadIdentity();
        glClearColor(0.4f,0.4f,0.4f,1.0f);
        
        // clear
        glClear(GL_COLOR_BUFFER_BIT);
        
        // no lighting/depth testing
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        
        LOG("DONE with OpenGL initialization");
    }
        
    void graphics_engine_bare::shutdown()
    {
#ifdef ES1_GLEXT_H_GUARD        
        shutdown_();
#endif
    }
    
#ifdef ES1_GLEXT_H_GUARD
    void graphics_engine_bare::create_frame_buffer() 
    {
        glGenFramebuffersOES(1, &view_frame_buf_);
        glGenRenderbuffersOES(1, &view_render_buf_);
        
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, view_frame_buf_);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, view_render_buf_);
        render_buf_storage_();
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, view_render_buf_);
        
        glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backing_width_);
        glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backing_height_);
        
        if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) 
        {
            LOG("failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
            throw std::exception();
        }
    }
#endif // gl es1
    
#ifdef ES1_GLEXT_H_GUARD
    void graphics_engine_bare::destroy_frame_buffer() 
    {
        glDeleteFramebuffersOES(1, &view_frame_buf_);
        view_frame_buf_ = 0;
        glDeleteRenderbuffersOES(1, &view_render_buf_);
        view_render_buf_ = 0;
    }
#elif defined(_OPENGL_H)
    void graphics_engine_bare::destroy_frame_buffer() 
    {
    }
#endif // gl es1
    
    void graphics_engine_bare::set_current_context() 
    {
        set_cur_ctx_();
    }
    
#ifdef ES1_GLEXT_H_GUARD
    void graphics_engine_bare::start_frame() 
    {
        set_current_context();
        
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, view_frame_buf_);
        glViewport(0, 0, backing_width_, backing_height_);
        glClear(GL_COLOR_BUFFER_BIT);
    }
#elif defined(_OPENGL_H)
    void graphics_engine_bare::start_frame() 
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
#endif // gl es1
  
    void graphics_engine_bare::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
#ifdef ES1_GLEXT_H_GUARD
    void graphics_engine_bare::present_frame() 
    {
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, view_render_buf_);
        present_render_buffer_();
    }
#elif defined(_OPENGL_H)
    void graphics_engine_bare::present_frame() 
    {
        glFlush();
    }    
#endif // gl es1

} // namespace graphics
} // namespace x2d
