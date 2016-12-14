LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS := -lz

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE    := mutilshotagent

LOCAL_SRC_FILES :=  mutilshot.cpp  \
                    RawToJpg.cpp  \
                    Socket.cpp \
		    ServerSocket.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

LOCAL_CFLAGS += -DDEBUG
LOCAL_CFLAGS += -DUSE_ONLINE

LOCAL_CFLAGS += -DLOGE=ALOGE
LOCAL_CFLAGS += -DLOGV=ALOGV
LOCAL_CFLAGS += -DLOGW=ALOGW 
LOCAL_CFLAGS += -DLOGD=ALOGD
LOCAL_CFLAGS += -DLOGI=ALOGI
LOCAL_CFLAGS += -DLOGE_IF=ALOGE_IF
LOCAL_CFLAGS += -DLOGV_IF=ALOGV_IF
LOCAL_CFLAGS += -DLOGW_IF=ALOGW_IF
LOCAL_CFLAGS += -DLOGD_IF=ALOGD_IF
LOCAL_CFLAGS += -DLIBUTILS_NATIVE=1 $(TOOL_CFLAGS) -fpermissive
LOCAL_STATIC_LIBRARIES += libcv libcvhighgui libcxcore  libcvml 
LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \
	libskia \
	libui \
	libgui \
        liblog 

LOCAL_C_INCLUDES += \
	external/skia/include/core \
	external/skia/include/effects \
	external/skia/include/images \
	external/skia/src/ports \
	external/skia/include/utils \
	external/zlib

include $(BUILD_EXECUTABLE)
