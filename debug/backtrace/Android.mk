LOCAL_PATH:= $(call my-dir)

####################################
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES += system/core/include/
LOCAL_SRC_FILES += backtrace.c
LOCAL_LDLIBS +=  -lpthread 
LOCAL_CFLAGS += -rdynamic
LOCAL_MODULE := backtrace 
LOCAL_SHARED_LIBRARIES := libc

include $(BUILD_EXECUTABLE)
####################################

