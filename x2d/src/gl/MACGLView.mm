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


static std::string get_key_name(int k)
{
    if(isalpha(k))
    {
        return std::string( 1, static_cast<char>(k) );
    }
    
    switch(k)
    {
        case NSRightArrowFunctionKey:
            return "right";
        case NSLeftArrowFunctionKey:
            return "left";
        case NSUpArrowFunctionKey:
            return "up";
        case NSDownArrowFunctionKey:
            return "down";
        case 32: // space
            return "space";
        case 13: // return
            return "return";
        case 44: // ,
            return ",";
        case 46: // .
            return ".";
        case 39: // '
            return "'";
            
        default:
            LOG("KEY %d NOT SUPPORTED YET!", k);
            return "space";
    }
}


@interface MACGLView (Callbacks)
- (void)presentRenderBuf;
- (void)setCurCtx;
- (void)renderBufStorage;
- (void)shutdown;
@end

@implementation MACGLView
{
    BOOL key_opt_;
    BOOL key_ctrl_;
    BOOL key_command_;
    BOOL key_shift_;
}

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
    
    // enable vsync
    [[self openGLContext] setValues:(GLint[]){1} forParameter:NSOpenGLCPSwapInterval];
    
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

- (void) keyDown:(NSEvent *)theEvent
{
    int key = [[theEvent charactersIgnoringModifiers] characterAtIndex:0];
    if(![theEvent isARepeat])
    {
        g_app->get_kernel().on_key_down( get_key_name(key) );
    }
}

- (void) keyUp:(NSEvent *)theEvent
{
    int key = [[theEvent charactersIgnoringModifiers] characterAtIndex:0];
    if(![theEvent isARepeat])
    {
        g_app->get_kernel().on_key_up( get_key_name(key) );
    }
}

- (void)flagsChanged:(NSEvent *)theEvent
{
    int flags = [theEvent modifierFlags];
    
    BOOL opt = (flags & NSAlternateKeyMask) ? YES : NO;
    BOOL ctrl = (flags & NSControlKeyMask) ? YES : NO;
    BOOL command = (flags & NSCommandKeyMask) ? YES : NO;
    BOOL shift = ( flags & NSShiftKeyMask ) ? YES : NO;

    if(opt && !key_opt_)
    {
        key_opt_ = YES; // opt is now pressed
        g_app->get_kernel().on_key_down("option");
    }
    else if(!opt && key_opt_)
    {
        key_opt_ = NO; // opt is now released
        g_app->get_kernel().on_key_up("option");
    }
    
    if(ctrl && !key_ctrl_)
    {
        key_ctrl_ = YES; // ctrl is now pressed
        g_app->get_kernel().on_key_down("control");
    }
    else if(!ctrl && key_ctrl_)
    {
        key_ctrl_ = NO; // ctrl is now released
        g_app->get_kernel().on_key_up("control");
    }
    
    if(command && !key_command_)
    {
        key_command_ = YES; // command is now pressed
        g_app->get_kernel().on_key_down("command");
    }
    else if(!command && key_command_)
    {
        key_command_ = NO; // command is now released
        g_app->get_kernel().on_key_up("command");
    }
    
    if(shift && !key_shift_)
    {
        key_shift_ = YES; // shift is now pressed
        g_app->get_kernel().on_key_down("shift");
    }
    else if(!shift && key_shift_)
    {
        key_shift_ = NO; // shift is now released
        g_app->get_kernel().on_key_up("shift");
    }
}

@end

#endif // MacOSX
