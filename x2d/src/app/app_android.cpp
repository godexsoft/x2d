//
//  app_android.cpp
//  x2d
//
//  Created by Alex Kremer on 9/9/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "app_framework.h"
#include "log.h"

#include <jni.h>
#include <errno.h>

#include <android_native_app_glue.h>
#include <android/window.h>
#include <android/sensor.h>
#include <android/asset_manager.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

// this is set in android_main()
extern struct android_app* g_android_app;

struct saved_state
{
};

/**
 * Android app shared stuff
 */
struct engine
{
    glm::mat4 input_transform;
    
    ASensorManager*     sensor_manager;
    const ASensor*      accel_sensor;
    ASensorEventQueue*  sensor_event_queue;
    
    bool        animating;
    EGLDisplay  display;
    EGLSurface  surface;
    EGLContext  context;
    int32_t     width;
    int32_t     height;
    struct saved_state state;
    
    app_framework* app;
};

static void android_set_cur_ctx(struct engine* engine) {
	if (eglMakeCurrent(engine->display, engine->surface,
                       engine->surface, engine->context) == EGL_FALSE)
	{
		LOG("Can't make current context");
	}
}

static void android_present_rb(struct engine* engine) {
	eglSwapBuffers(engine->display, engine->surface);
}

static void android_set_rb_storage(struct engine* engine) {
    
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void android_term_display(struct engine* engine)
{
    if (engine->display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        
        if (engine->context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(engine->display, engine->context);
        }
        
        if (engine->surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(engine->display, engine->surface);
        }
        
        eglTerminate(engine->display);
    }
    
    engine->animating = false;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Initialize an EGL context for the current display.
 */
static int android_init_display(struct engine* engine)
{
    // initialize OpenGL ES and EGL
	LOG("Init display!!!");
    
    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };
    
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;
    EGLDisplay display;
    
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    
    eglInitialize(display, 0, 0);
    
    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    
    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    
    ANativeWindow_setBuffersGeometry(g_android_app->window, 0, 0, format);
    
    surface = eglCreateWindowSurface(display, config, g_android_app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);
    
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        return -1;
    }
    
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
    
    LOG("DISPLAY SIZE: %d x %d", w, h);
    
    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    
    // generate transformation matrix
    engine->input_transform = glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, engine->height, 0.0f) );
	engine->input_transform = glm::scale(engine->input_transform, glm::vec3(1.0f, -1.0f, 1.0f));
    
    // Initialize GL state.
    // setup callbacks for platform stuff
    graphics_engine::instance().set_cur_ctx_cb(
        boost::bind(&android_set_cur_ctx, engine) );
    graphics_engine::instance().set_present_render_buffer_cb(
        boost::bind(&android_present_rb, engine) );
    graphics_engine::instance().set_render_buf_storage_cb(
        boost::bind(&android_set_rb_storage, engine) );
    graphics_engine::instance().set_shutdown_cb(
        boost::bind(&android_term_display, engine) );
    
    // init opengl
    graphics_engine::instance().init();
    
    engine->animating = true;
    return 0;
}

/**
 * Process the next input event.
 */
static int32_t android_handle_input(struct android_app* app, AInputEvent* event)
{
    LOG("Input from android..");
    
    struct engine* engine = (struct engine*)g_android_app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        
    	int historySize = AMotionEvent_getHistorySize(event);
    	int endX, endY;
    	endX = endY = 0;
        
    	if(historySize>0)
    	{
    		endX = AMotionEvent_getHistoricalX(event, 0, historySize-1);
    		endY = AMotionEvent_getHistoricalY(event, 0, historySize-1);
    	}
        
        std::vector<touch> tchs;
        tchs.push_back( touch( point(AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0)),
                              point(endX, endY), 1, AMotionEvent_getEventTime(event)) );
        
        engine->app->get_kernel().on_touches_began(
            input_manager::transform(tchs, engine->input_transform) );
        
        return 1;
    }
    
    return 0;
}

static void android_handle_cmd(struct android_app* app, int32_t cmd)
{
    LOG("Handle android cmd..");
    
    struct engine* engine = (struct engine*)g_android_app->userData;
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            LOG("Save state requested..");
            
            // The system has asked us to save our current state.  Do so.
            g_android_app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)g_android_app->savedState) = engine->state;
            g_android_app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            LOG("Init window requested..");
            
            // The window is being shown, get it ready.
            if (g_android_app->window != NULL)
            {
                android_init_display(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            LOG("Terminate window");
            
            // The window is being hidden or closed, clean it up.
            android_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            LOG("Gain focus");
            
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accel_sensor != NULL)
            {
                ASensorEventQueue_enableSensor(engine->sensor_event_queue,
                                               engine->accel_sensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensor_event_queue,
                                               engine->accel_sensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            LOG("Lost focus");
            
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accel_sensor != NULL)
            {
                ASensorEventQueue_disableSensor(engine->sensor_event_queue,
                                                engine->accel_sensor);
            }
            
            // Also stop animating.
            engine->animating = false;
            break;
    }
}

void android_x2d_main(app_framework& app)
{
    LOG("Launching android application...");
    struct engine engine;
    
    g_android_app->userData = &engine;
    g_android_app->onAppCmd = android_handle_cmd;
    g_android_app->onInputEvent = android_handle_input;
    
    engine.animating = false;
    engine.app = &app;
    
    LOG("Going to make full screen..");
    
    // make fullscreen
    ANativeActivity_setWindowFlags(g_android_app->activity, AWINDOW_FLAG_FULLSCREEN,
                                   AWINDOW_FLAG_FORCE_NOT_FULLSCREEN);
    
    // accel
    engine.sensor_manager = ASensorManager_getInstance();
    engine.accel_sensor = ASensorManager_getDefaultSensor(
        engine.sensor_manager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensor_event_queue = ASensorManager_createEventQueue(
        engine.sensor_manager,
            g_android_app->looper, LOOPER_ID_USER, NULL, NULL);
    
    // run user code allowing them to initialize some objects
    // just before starting the main looper.
    app.main();
    
    LOG("Start loop");
    while (1)
    {
        int ident;
        int events;
        struct android_poll_source* source;
        
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                                      (void**)&source)) >= 0) {
            
            if (source != NULL)
            {
                source->process(g_android_app, source);
            }
            
            if (ident == LOOPER_ID_USER)
            {
                if (engine.accel_sensor != NULL)
                {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(
                             engine.sensor_event_queue,
                                &event, 1) > 0)
                    {
//                        LOG("accelerometer: x=%f y=%f z=%f",
//                               event.acceleration.x, event.acceleration.y,
//                               event.acceleration.z);
                    }
                }
            }
            
            if (g_android_app->destroyRequested != 0)
            {
                android_term_display(&engine);
                return;
            }
        }
        
        if (engine.animating)
        {
            app.step();
        }
    }
}