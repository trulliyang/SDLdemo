LOCAL_PATH := $(call my-dir)

###########################
#
# SDL shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := SDL2

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/src/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/dummy/*.c) \
	$(LOCAL_PATH)/src/atomic/SDL_atomic.c.arm \
	$(LOCAL_PATH)/src/atomic/SDL_spinlock.c.arm \
	$(wildcard $(LOCAL_PATH)/src/core/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/cpuinfo/*.c) \
	$(wildcard $(LOCAL_PATH)/src/dynapi/*.c) \
	$(wildcard $(LOCAL_PATH)/src/events/*.c) \
	$(wildcard $(LOCAL_PATH)/src/file/*.c) \
	$(wildcard $(LOCAL_PATH)/src/haptic/*.c) \
	$(wildcard $(LOCAL_PATH)/src/haptic/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/android/*.c) \
	$(LOCAL_PATH)/src/joystick/steam/SDL_steamcontroller.c \
	$(wildcard $(LOCAL_PATH)/src/loadso/dlopen/*.c) \
	$(wildcard $(LOCAL_PATH)/src/power/*.c) \
	$(wildcard $(LOCAL_PATH)/src/power/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/filesystem/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/render/*.c) \
	$(wildcard $(LOCAL_PATH)/src/render/*/*.c) \
	$(wildcard $(LOCAL_PATH)/src/stdlib/*.c) \
	$(wildcard $(LOCAL_PATH)/src/thread/*.c) \
	$(wildcard $(LOCAL_PATH)/src/thread/pthread/*.c) \
	$(wildcard $(LOCAL_PATH)/src/timer/*.c) \
	$(wildcard $(LOCAL_PATH)/src/timer/unix/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/test/*.c))

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)

###########################
#
# SDL static library
#
###########################

LOCAL_MODULE := SDL2_static

LOCAL_MODULE_FILENAME := libSDL2

LOCAL_LDLIBS := 
LOCAL_EXPORT_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_STATIC_LIBRARY)

###########################
#
# SDL main static library
#
###########################

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_MODULE := SDL2_main

LOCAL_MODULE_FILENAME := libSDL2main

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := main

#SDL_PATH := src/include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src/include

# Add your application source files here...
LOCAL_SRC_FILES := main.c

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)



# sdl module
# include $(CLEAR_VARS)

#LOCAL_MODULE := sdl_module
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/include


# Add your application source files here...
#LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/sdl/*.c)

#LOCAL_SHARED_LIBRARIES := SDL2 mlt

#LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
#include $(BUILD_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := prebuilt-ffmpeg
#LOCAL_SRC_FILES := $(LOCAL_PATH)/ffmpeg/lib/libavformat.so
#include $(PREBUILT_SHARED_LIBRARY)


#include $(CLEAR_VARS)
#LOCAL_MODULE := prebuilt-ffmpeg1
#LOCAL_SRC_FILES := $(LOCAL_PATH)/ffmpeg/lib/libavcodec.so
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := prebuilt-ffmpeg2
#LOCAL_SRC_FILES := $(LOCAL_PATH)/ffmpeg/lib/libavfilter.so
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := prebuilt-ffmpeg3
#LOCAL_SRC_FILES := $(LOCAL_PATH)/ffmpeg/lib/libavutil.so
#include $(PREBUILT_SHARED_LIBRARY)


#include $(CLEAR_VARS)
#LOCAL_MODULE := prebuilt-ffmpeg4
#LOCAL_SRC_FILES := $(LOCAL_PATH)/ffmpeg/lib/libswresample.so
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := prebuilt-ffmpeg5
#LOCAL_SRC_FILES := $(LOCAL_PATH)/ffmpeg/lib/libswscale.so
#include $(PREBUILT_SHARED_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE := prebuilt-ijkffmpeg
LOCAL_SRC_FILES := $(LOCAL_PATH)/ffmpeg/lib/libijkffmpeg.so
include $(PREBUILT_SHARED_LIBRARY)

#  avformat
#include $(CLEAR_VARS)
#LOCAL_MODULE := avformat_module
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/include
# Add your application source files here...
#LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/avformat/*.c)
#LOCAL_SHARED_LIBRARIES := mlt prebuilt-ffmpeg prebuilt-ffmpeg1 prebuilt-ffmpeg2 prebuilt-ffmpeg3 prebuilt-ffmpeg4 prebuilt-ffmpeg5
#LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
#include $(BUILD_SHARED_LIBRARY)

# movit
include $(CLEAR_VARS)

LOCAL_MODULE := movit
LOCAL_C_INCLUDES := $(LOCAL_PATH)/movit \
                    $(LOCAL_PATH)/include \

# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/movit/*cpp) \

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -lGLESv3 -lEGL
LOCAL_SHARED_LIBRARIES := SDL2

include $(BUILD_STATIC_LIBRARY)

# mlt
include $(CLEAR_VARS)

LOCAL_MODULE := mlt
LOCAL_C_INCLUDES := $(LOCAL_PATH)/framework/ \
                    $(LOCAL_PATH)/mlt++ \
                    $(LOCAL_PATH) \
                    $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/ffmpeg/include \
                    $(LOCAL_PATH)/movit

# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/framework/*.c) \
                   $(wildcard $(LOCAL_PATH)/mlt++/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/sdl/*.c)     \
                   $(wildcard $(LOCAL_PATH)/avformat/*.c) \
                   $(wildcard $(LOCAL_PATH)/opengl/*.cpp) \
                   $(wildcard $(LOCAL_PATH)/opengl/*.c)

LOCAL_SHARED_LIBRARIES :=movit SDL2 prebuilt-ijkffmpeg
#prebuilt-ffmpeg prebuilt-ffmpeg1 prebuilt-ffmpeg2 prebuilt-ffmpeg3 prebuilt-ffmpeg4 prebuilt-ffmpeg5

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -lGLESv3 -lEGL
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)

# melt
include $(CLEAR_VARS)

LOCAL_MODULE := melt
LOCAL_C_INCLUDES := $(LOCAL_PATH)/framework/ \
                    $(LOCAL_PATH)/mlt++ \

#LOCAL_C_INCLUDES += $(LOCAL_PATH)/3rd/libleaktracer/include/


# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/melt/*.cpp)
#LOCAL_SRC_FILES += 3rd/libleaktracer/src/AllocationHandlers.cpp \
#	               3rd/libleaktracer/src/MemoryTrace.cpp

LOCAL_SHARED_LIBRARIES :=  SDL2

LOCAL_SHARED_LIBRARIES := mlt movit

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -lGLESv3 -lEGL
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := memLeakTrace
LOCAL_C_INCLUDES := $(LOCAL_PATH)/3rd/libleaktracer/include
# Add your application source files here...
LOCAL_SRC_FILES := 3rd/libleaktracer/src/AllocationHandlers.cpp \
	               3rd/libleaktracer/src/MemoryTrace.cpp

LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)


