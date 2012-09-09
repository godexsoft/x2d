include $(CLEAR_VARS)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/x2d/libs/luabind

LOCAL_C_INCLUDES := $(LOCAL_PATH)/x2d/libs/luabind
LOCAL_C_INCLUDES += $(LOCAL_PATH)/x2d/libs/lua/src
LOCAL_C_INCLUDES += $(NDK_ROOT)/sources/boost

LOCAL_CPP_FEATURES := rtti exceptions

LOCAL_MODULE 	 := luabind

LOCAL_SRC_FILES := \
   	x2d/libs/luabind/src/class.cpp \
	x2d/libs/luabind/src/class_info.cpp \
	x2d/libs/luabind/src/class_registry.cpp \
	x2d/libs/luabind/src/class_rep.cpp \
	x2d/libs/luabind/src/create_class.cpp \
	x2d/libs/luabind/src/error.cpp \
	x2d/libs/luabind/src/exception_handler.cpp \
	x2d/libs/luabind/src/function.cpp \
	x2d/libs/luabind/src/inheritance.cpp \
	x2d/libs/luabind/src/link_compatibility.cpp \
	x2d/libs/luabind/src/object_rep.cpp \
	x2d/libs/luabind/src/open.cpp \
	x2d/libs/luabind/src/pcall.cpp \
	x2d/libs/luabind/src/scope.cpp \
	x2d/libs/luabind/src/stack_content_by_name.cpp \
	x2d/libs/luabind/src/weak_ref.cpp \
	x2d/libs/luabind/src/wrapper_base.cpp
		
include $(BUILD_STATIC_LIBRARY)
