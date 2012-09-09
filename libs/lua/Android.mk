include $(CLEAR_VARS)
LOCAL_CFLAGS := -DLUA_COMPAT_ALL=1
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/x2d/libs/lua/src
LOCAL_MODULE    	:= lua

LOCAL_SRC_FILES := \
   	x2d/libs/lua/src/ldblib.c \
	x2d/libs/lua/src/lgc.c \
	x2d/libs/lua/src/lmem.c \
	x2d/libs/lua/src/lparser.c \
	x2d/libs/lua/src/lapi.c \
	x2d/libs/lua/src/ldebug.c \
	x2d/libs/lua/src/ltablib.c \
	x2d/libs/lua/src/lundump.c \
	x2d/libs/lua/src/linit.c \
	x2d/libs/lua/src/loadlib.c \
	x2d/libs/lua/src/lstate.c \
	x2d/libs/lua/src/ltm.c \
	x2d/libs/lua/src/lauxlib.c \
	x2d/libs/lua/src/ldo.c \
	x2d/libs/lua/src/liolib.c \
	x2d/libs/lua/src/lobject.c \
	x2d/libs/lua/src/lvm.c \
	x2d/libs/lua/src/llex.c \
	x2d/libs/lua/src/lstring.c \
	x2d/libs/lua/src/lbaselib.c \
	x2d/libs/lua/src/ldump.c \
	x2d/libs/lua/src/lopcodes.c \
	x2d/libs/lua/src/lzio.c \
	x2d/libs/lua/src/lcode.c \
	x2d/libs/lua/src/lfunc.c \
	x2d/libs/lua/src/lstrlib.c \
	x2d/libs/lua/src/lmathlib.c \
	x2d/libs/lua/src/loslib.c \
	x2d/libs/lua/src/ltable.c \
	x2d/libs/lua/src/print.c
	
include $(BUILD_STATIC_LIBRARY)
