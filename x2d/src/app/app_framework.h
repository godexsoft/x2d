//
//  app_framework.h
//  x2d
//
//  Created by Alex Kremer on 8/9/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_APP_FRAMEWORK_H__
#define __X2D_APP_FRAMEWORK_H__

#include "log.h"
#include "platform.h"

#include "configuration.h"
#include "liverpool_manager.h"
#include "resource_manager.h"
#include "kernel.h"

namespace x2d {
namespace base {
    
    class scene;

} // namespace base
    
    class app_framework;
} // namespace x2d

/**
 * This must be defined by the client
 */
int x2d_main();

#ifdef __APPLE__
void ios_x2d_main(x2d::app_framework&);
#elif defined(ANDROID)
void android_x2d_main(x2d::app_framework&);
#endif

namespace x2d {
    template<typename T=app_framework>
    int run_app()
    {
        try {
            T app;
            
            // call init first
            app.init();
                        
            // run the loop
#ifdef ANDROID
            android_x2d_main(app);
#elif defined(__APPLE__)
            ios_x2d_main(app);
#endif
        }
        catch(sys_exception& ex)
        {
            LOG("Game exception caught: %s", ex.what());
        }
        
        return 0;
    }
    
    class app_framework
    {
    public:
        app_framework()
        : res_manager_(lvp_manager_)
        , config_(kernel_, res_manager_)
        {
            LOG("App framework started..");
        }
        
        inline void step()
        {
            kernel_.step();
        }
        
        // you can override this to setup your liverpool stuff for example
        virtual void init();
        
        // this is the main game entrypoint.
        // here you should create your objects and other usefull stuff
        // but never loop in this function. return asap.
        // by default it just switches to the main scene.
        virtual void main()
        {
            // load the main scene by default
            switch_to( get_config().create_scene("scenes.main") );
        }
        
        liverpool_manager& get_lvp_manager()
        {
            return lvp_manager_;
        }

        resource_manager& get_res_manager()
        {
            return res_manager_;
        }
        
        kernel& get_kernel()
        {
            return kernel_;
        }

        configuration& get_config()
        {
            return config_;
        }

        const device_capabilities& get_capabilities() const
        {
            return caps_;
        }
        
        void switch_to(const boost::shared_ptr<scene>& s);
        
    private:
        kernel              kernel_;
        liverpool_manager   lvp_manager_;
        resource_manager    res_manager_;
        configuration       config_;
        
        device_capabilities caps_;
        
        boost::shared_ptr<scene>    cur_scene_;
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_APP_FRAMEWORK_H__
