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
#import "liverpool_manager.h"
#import "resource_manager.h"
#import "platform.h"
#import "texture.h"

#import <QuartzCore/QuartzCore.h>
#include <iostream>

namespace lp = x2d::liverpool;
namespace fs = x2d::filesystem;

class Lister : public std::unary_function<lp::entry, void> 
{
private:
    lp::liverpool_ptr fs;
	
public:
	Lister(const lp::liverpool_ptr& l) : fs(l) 
    {
    }
	
    void list(fs::path pth)
    {
        for(lp::liverpool::diterator it = fs->dir_begin(pth.string()); 
            it != fs->dir_end(); ++it) 
        {
            
            fs::path p(it->path());
            std::string lpc = p.last_path_component();
            
            if ( it->is_dir() ) 
            {                
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
        
    liverpool_manager lvp_man; // create as many as you like
    lvp_man.mount("resources.zip", "res");
    try 
    {
        lvp_man.mount("level1.zip", "lvl1");
    } 
    catch(...)
    {
        LOG("Failed to mount lvl1");
    }
  
//  create a resource manager which references (uses) the liverpool_manager we just created 
    resource_manager resource_man(lvp_man); // create as many as you like
    
    {
        resource_ptr<texture> txtr = resource_man.get<texture>("res/test/graphics/chupa.png");
//  now use txtr (for example in a scene)
    } // txtr ref counter decremented

    resource_ptr<texture> txtr = resource_man.get<texture>("lvl1/test/graphics/default.png");
    
//      resource_ptr<sprite> anim_frame = resource_man.get<sprite>("res/player_animation/anim_frame_1"); 
//  sprite has a weak ref to a texture object so it will require a resource_ptr<texture> each time it needs data
//  that means that if the resource_ptr had not been loaded into opengl then it will get loaded when sprite is used
//  otherwise it will just increment the ref counter of the texture resource.
//  the name to resource mapping is done using a simple map. the sprite object thus does not own the texture.
    
    Lister lister(lvp_man.get("res"));
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
