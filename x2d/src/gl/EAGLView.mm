//
//  EAGLView.mm
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"
#import <OpenGLES/EAGLDrawable.h>

#include "graphics_engine.h"
#include "objc_callback.h"
#include "log.h"

@interface EAGLView (Callbacks)
- (void)presentRenderBuf;
- (void)setCurCtx;
- (void)renderBufStorage;
- (void)shutdown;
@end

@implementation EAGLView

+ (Class)layerClass 
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame 
{    
    if ((self = [super initWithFrame:frame])) 
    {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		self.multipleTouchEnabled = YES;
		self.userInteractionEnabled = YES;
        
        context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context_ || ![EAGLContext setCurrentContext:context_]) {
            LOG("Couldn't init OpenGLES.");
            throw std::exception();
        }
        
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
	}
	
    return self;
}

- (void)layoutSubviews 
{
    LOG("Layout subviews in EAGLView");
    graphics_engine::instance().set_current_context();
    graphics_engine::instance().destroy_frame_buffer();
    graphics_engine::instance().create_frame_buffer();
}

- (void)presentRenderBuf 
{
    [context_ presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void)setCurCtx 
{
    [EAGLContext setCurrentContext:context_];
}

- (void)shutdown
{
    LOG("Shutdown EAGLView");
    if ([EAGLContext currentContext] == context_) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [context_ release];
}

- (void)renderBufStorage
{
    [context_ renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer *)self.layer];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
    LOG("Touch began");
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{
    LOG("Touch moved");
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{
    LOG("Touch ended");
}

@end
