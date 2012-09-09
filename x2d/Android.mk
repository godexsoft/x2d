# build all libs
include $(LOCAL_PATH)/x2d/libs/lua/Android.mk
include $(LOCAL_PATH)/x2d/libs/luabind/Android.mk
include $(LOCAL_PATH)/x2d/libs/box2d/Android.mk

include $(CLEAR_VARS)
LOCAL_MODULE := boost_signals
LOCAL_SRC_FILES := ../../../sources/boost/android/lib/libboost_signals.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := boost_system
LOCAL_SRC_FILES := ../../../sources/boost/android/lib/libboost_system.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := boost_random
LOCAL_SRC_FILES := ../../../sources/boost/android/lib/libboost_random.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/x2d/libs/luabind
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/libs/box2d
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/libs/lua/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/libs/glm
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/libs/glm/gtc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/libs/rapidxml

LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/app
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/anim
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/base
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/config
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/event
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/fs
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/font
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/gl
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/input
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/kernel
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/physics
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/script
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/sound
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/util
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/x2d/src/resource

LOCAL_C_INCLUDES += $(NDK_ROOT)/sources/boost

LOCAL_CPP_FEATURES := rtti exceptions

LOCAL_MODULE := x2d_core

LOCAL_CPPFLAGS := -DLUA_COMPAT_ALL=1 -fpermissive

LOCAL_STATIC_LIBRARIES := android_native_app_glue

LOCAL_SRC_FILES := \
     x2d/x2d/src/platform_android.cpp \
     x2d/x2d/src/anim/animation.cpp \
     x2d/x2d/src/app/app_framework.cpp \
     x2d/x2d/src/app/app_android.cpp \
     x2d/x2d/src/base/callable.cpp \
     x2d/x2d/src/base/camera.cpp \
     x2d/x2d/src/base/object.cpp \
     x2d/x2d/src/base/space.cpp \
     x2d/x2d/src/base/spawner.cpp \
     x2d/x2d/src/base/sprite.cpp \
     x2d/x2d/src/base/texture.cpp \
     x2d/x2d/src/config/configuration.cpp \
     x2d/x2d/src/config/parsers.cpp \
     x2d/x2d/src/config/plugin.cpp \
     x2d/x2d/src/config/random.cpp \
     x2d/x2d/src/config/value_holder.cpp \
     x2d/x2d/src/config/value_parsers.cpp \
     x2d/x2d/src/event/event_manager.cpp \
     x2d/x2d/src/event/listener.cpp \
     x2d/x2d/src/font/font.cpp \
     x2d/x2d/src/fs/liverpool.cpp \
     x2d/x2d/src/fs/liverpool_manager.cpp \
     x2d/x2d/src/fs/posix_fs.cpp \
     x2d/x2d/src/gl/graphics_engine.cpp \
     x2d/x2d/src/input/input_manager.cpp \
     x2d/x2d/src/input/object_input.cpp \
     x2d/x2d/src/kernel/clock.cpp \
     x2d/x2d/src/kernel/kernel.cpp \
     x2d/x2d/src/kernel/timer.cpp \
     x2d/x2d/src/physics/body.cpp \
     x2d/x2d/src/physics/body_part.cpp \
     x2d/x2d/src/physics/contact.cpp \
     x2d/x2d/src/physics/world.cpp \
     x2d/x2d/src/script/script.cpp \
     x2d/x2d/src/script/scripting_engine.cpp \
     x2d/x2d/src/sound/null_drivers.cpp \
     x2d/x2d/src/sound/sound.cpp \
     x2d/x2d/src/sound/sound_resource.cpp \
     x2d/x2d/src/util/log.cpp \
     x2d/x2d/src/util/math_util.cpp
		
include $(BUILD_STATIC_LIBRARY)
