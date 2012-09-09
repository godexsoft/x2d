//
//  app_framework.cpp
//  x2d
//
//  Created by Alex Kremer on 8/9/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "app_framework.h"

// global app used by iOS runner :-/
// this is dirty but I have no idea how
// to make it work with the delegate on iOS otherwise.
app_framework* g_app;

namespace x2d {

    void app_framework::init()
    {
        LOG("Calling default init method of app framework...");
        
        // by default we expect a resources.zip file in the bundle.
        // we will mount only that single resource disc.
        get_lvp_manager().mount("resources.zip", "res");
        
        // and then we parse the main.xml config
        get_config().parse_file("res/main.xml");
    }
    
} // namespace x2d

#ifdef __APPLE__
// for iOS we just forward to x2d_main which must be defined by the client
int main(int argc, char* argv[])
{
    return x2d_main();
}
#endif

#ifdef ANDROID
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
AAssetManager* g_assetManager;
struct android_app* g_android_app;

// for android we are using app glue so main is already implemented by them
extern "C"
void android_main(struct android_app* state) {
    // Make sure glue isn't stripped.
    app_dummy();
    
    // dirty but needed for liverpool :-/
    g_assetManager = state->activity->assetManager;
    
    // dirty too
    g_android_app = state;
    
    // call x2d main
    x2d_main();
}
#endif