LOCAL_PATH:= $(call my-dir)

####################################
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES += system/core/include/
LOCAL_SRC_FILES += CallStack.cpp
LOCAL_LDLIBS +=   
LOCAL_CFLAGS += -O2 -g -W -Wall
LOCAL_MODULE := CallStack.exe
LOCAL_SHARED_LIBRARIES := libc libutils
LOCAL_MODULE_PATH := $(LOCAL_PATH)

include $(BUILD_EXECUTABLE)
####################################
