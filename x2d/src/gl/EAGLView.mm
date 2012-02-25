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
#include "input_manager.h"
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

- (id) initWithFrame:(const CGRect&)frame kernel:(kernel*)k
{    
    if ((self = [super initWithFrame:frame])) 
    {
        // save kernel pointer
        _k = k;
        
        // generate transformation matrix
        input_transform = glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, frame.size.height, 0.0f) );
        input_transform = glm::scale(input_transform, glm::vec3(1.0f, -1.0f, 1.0f));
        
        // support retina
        // self.contentScaleFactor = 2.0f;
        
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
        
        // init accelerometer
        UIAccelerometer *accel = [UIAccelerometer sharedAccelerometer];
        accel.delegate = self;
        accel.updateInterval = 1.0f/60.0f;
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
    std::vector<touch> tchs;
    
    for(UITouch* t in touches)
    {
        tchs.push_back( touch( [t locationInView:self], [t previousLocationInView:self], t.tapCount, t.timestamp) );
    }
    
    _k->on_touches_began( input_manager::transform(tchs, input_transform) );
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{
    std::vector<touch> tchs;
    
    for(UITouch* t in touches)
    {
        tchs.push_back( touch( [t locationInView:self], [t previousLocationInView:self], t.tapCount, t.timestamp) );
    }
    
    _k->on_touches_moved( input_manager::transform(tchs, input_transform) );
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{
    std::vector<touch> tchs;
    
    for(UITouch* t in touches)
    {
        tchs.push_back( touch( [t locationInView:self], [t previousLocationInView:self], t.tapCount, t.timestamp) );
    }
    
    _k->on_touches_ended( input_manager::transform(tchs, input_transform) );
}

- (void)accelerometer:(UIAccelerometer *)accelerometer
		didAccelerate:(UIAcceleration *)acceleration
{
    _k->on_acceleration(acceleration.x, acceleration.y, acceleration.z);
}

@end
