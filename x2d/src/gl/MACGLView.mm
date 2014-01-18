//
//  MACGLView.mm
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#if TARGET_OS_MAC && !TARGET_OS_IPHONE

#import <QuartzCore/QuartzCore.h>

#import "MACGLView.h"

#include "graphics_engine.h"
#include "objc_callback.h"
#include "log.h"
#include "app_framework.h"

// global pointer to the app :-/
extern app_framework* g_app;

@interface MACGLView (Callbacks)
- (void)presentRenderBuf;
- (void)setCurCtx;
- (void)renderBufStorage;
- (void)shutdown;
@end

@implementation MACGLView

- (id) initWithCapabilities:(const device_capabilities&)caps
{
    // context setup
    NSOpenGLPixelFormat *windowedPixelFormat;
    NSOpenGLPixelFormatAttribute attribs[] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFASingleRenderer,
        0 };
    
    windowedPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    
    if (!windowedPixelFormat)
    {
        LOG("Unable to create windowed pixel format.");
        throw std::exception();
    }
    
    self = [super initWithFrame:
            NSMakeRect(0, 0,
                caps.display_size.width,
                caps.display_size.height)
            pixelFormat:windowedPixelFormat];
    
    if (self == nil)
    {
        LOG("Unable to create a windowed OpenGL context.");
        throw std::exception();
    }
    
    [windowedPixelFormat release];
    context_ = [self openGLContext];
        
    if (!context_)
    {
        LOG("Couldn't init OpenGL.");
        throw std::exception();
    }
    
    // generate transformation matrix
    input_transform = glm::mat4(1.0f);
    
    // input transformations on iphone
    input_transform = glm::translate( input_transform, glm::vec3(0.0f,
            caps.has_retina ?
                caps.display_size.height*2
                : caps.display_size.height, 0.0f) );
    input_transform = glm::scale(input_transform, glm::vec3(1.0f, -1.0f, 1.0f));
    
    // set retina if detected
    //        if(caps.has_retina)
    //        {
    //            self.contentScaleFactor = 2.0f;
    //            CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    //            eaglLayer.contentsScale = 2;
    //
    //            // scale input by two for retina
    //            input_transform = glm::scale(input_transform, glm::vec3(2.0f, 2.0f, 1.0f));
    //        }
    
    [context_ makeCurrentContext];
                
    // setup callbacks for platform stuff
    graphics_engine::instance().set_cur_ctx_cb(
        objc_callback(@selector(setCurCtx), self) );
    graphics_engine::instance().set_present_render_buffer_cb(
        objc_callback(@selector(presentRenderBuf), self) );
    graphics_engine::instance().set_render_buf_storage_cb(
        objc_callback(@selector(renderBufStorage), self) );
    graphics_engine::instance().set_shutdown_cb(
        objc_callback(@selector(shutdown), self) );
        
    // init opengl
    graphics_engine::instance().init();
	
    return self;
}

- (void)layoutSubviews
{
    LOG("Layout subviews in EAGLView");
    graphics_engine::instance().set_current_context();
//    graphics_engine::instance().destroy_frame_buffer();
//    graphics_engine::instance().create_frame_buffer();
}

- (void)presentRenderBuf 
{
    [[self openGLContext] flushBuffer];
}

- (void)setCurCtx 
{
    [context_ setCurrentContext];
}

- (void)shutdown
{
    LOG("Shutdown EAGLView");
    [context_ release];
}

- (void)renderBufStorage
{
//    [context_ renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer *)self.layer];
}

@end

#endif // MacOSX
