//
//  AppDelegate.m
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import "AppDelegate.h"
#import "EAGLView.h"
#import "kernel.h"
#import "custom_obj.h"
#import "liverpool.h"
#import "platform.h"

#import <QuartzCore/QuartzCore.h>
#include <iostream>

namespace lp = x2d::liverpool;
namespace fs = x2d::filesystem;

class Lister : public std::unary_function<lp::entry, void> {
private:
    lp::liverpool& fs;
	
public:
	Lister(lp::liverpool& l) : fs(l) {};
	
    void list(fs::path pth)
    {
        for(lp::liverpool::diterator it = fs.dir_begin(pth.string()); it != fs.dir_end(); ++it) {
            
            fs::path p(it->path());
            std::string lpc = p.last_path_component();
            
            if ( it->is_dir() ) {
                
                LOG("[DIR] '%s'", lpc.c_str());
                
                // Skip hidden files and current/prev directory
                if(lpc.substr(0,1) != ".") 
                {                    
                    this->list( pth / lpc );
                }
            } else {
                
                // This is a file. Check it against all our supported types.
                LOG("----- '%s'", lpc.c_str());
            }
        }
    }
};

@implementation AppDelegate

@synthesize window = _window;
@synthesize gl_view = _gl_view;
@synthesize dl = _dl;

- (void)dealloc
{
    [_window release];
    [_gl_view release];
    [_dl release];
    [super dealloc];
}

- (void)start:(id)dummy
{
    NSLog(@"Starting.");
        
    // first read the resources zip
    FILE* fp = fopen( path_for_resource("resources.zip").c_str(), "rb" );
    assert(fp != NULL);
    
    LOG("file opened");
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    LOG("size = %d", size);
    
    lp::liverpool l( ifdstream(fp, 0, size) );
    Lister lister(l);
    lister.list("/");
    
    // register some object with our kernel
    obj = boost::shared_ptr<base_object>( new custom_obj(_k) );
    
    self.dl = [[CADisplayLink displayLinkWithTarget:self selector:@selector(step)] autorelease];
    self.dl.frameInterval = 1;
    [self.dl addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)step
{
    _k.step();
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:CGRectMake(0, 0, 320, 480)] autorelease];
    
    // create opengl view
    self.gl_view = [[[EAGLView alloc] initWithFrame:CGRectMake(0, 0, 320, 480)] autorelease];    
    self.window.multipleTouchEnabled = YES;
    self.window.userInteractionEnabled = YES;
    self.window.rootViewController = nil;
    
    [self.window addSubview:self.gl_view];
    [self.window makeKeyAndVisible];
    [self performSelectorOnMainThread:@selector(start:) withObject:nil waitUntilDone:NO];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    LOG("Will resign active");
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    LOG("Pause.");
    _k.pause();
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    LOG("Will enter foreground");
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    LOG("Resume.");
    _k.resume();
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    LOG("Shutdown graphics engine..");
    graphics_engine::instance().shutdown();
}

@end
