//
//  EAGLView.mm
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

//#if __APPLE__
//#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"
#import <OpenGLES/EAGLDrawable.h>

#include "graphics_engine.h"
#include "input_manager.h"
#include "objc_callback.h"
#include "log.h"
#include "app_framework.h"

// global pointer to the app :-/
extern app_framework* g_app;

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

- (id) initWithCapabilities:(const device_capabilities&)caps
{
    if ((self = [super initWithFrame:
                 CGRectMake(0, 0,
                            caps.display_size.width,
                            caps.display_size.height)]))
    {
        // generate transformation matrix
        input_transform = glm::mat4(1.0f);

        // input transformations on iphone
        input_transform = glm::translate( input_transform, glm::vec3(0.0f,
                caps.has_retina ?
                    caps.display_size.height*2
                    : caps.display_size.height, 0.0f) );
        input_transform = glm::scale(input_transform, glm::vec3(1.0f, -1.0f, 1.0f));

        // set retina if detected
        if(caps.has_retina)
        {
            self.contentScaleFactor = 2.0f;
            CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
            eaglLayer.contentsScale = 2;
            
            // scale input by two for retina
            input_transform = glm::scale(input_transform, glm::vec3(2.0f, 2.0f, 1.0f));
        }
        
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO],
                                            kEAGLDrawablePropertyRetainedBacking,
                                            kEAGLColorFormatRGBA8,
                                            kEAGLDrawablePropertyColorFormat, nil];
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
    
    g_app->get_kernel().on_touches_began( input_manager::transform(tchs, input_transform) );
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{
    std::vector<touch> tchs;
    
    for(UITouch* t in touches)
    {
        tchs.push_back( touch( [t locationInView:self], [t previousLocationInView:self], t.tapCount, t.timestamp) );
    }
    
    g_app->get_kernel().on_touches_moved( input_manager::transform(tchs, input_transform) );
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{
    std::vector<touch> tchs;
    
    for(UITouch* t in touches)
    {
        tchs.push_back( touch( [t locationInView:self], [t previousLocationInView:self], t.tapCount, t.timestamp) );
    }
    
    g_app->get_kernel().on_touches_ended( input_manager::transform(tchs, input_transform) );
}

- (void)accelerometer:(UIAccelerometer *)accelerometer
		didAccelerate:(UIAcceleration *)acceleration
{
    g_app->get_kernel().on_acceleration(acceleration.x, acceleration.y, acceleration.z);
}

@end

// #endif // iOS or Simulator
// #endif // __APPLE__
