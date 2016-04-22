LOCAL_PATH:= $(call my-dir)

####################################
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES += system/core/include/
LOCAL_SRC_FILES += CallStack.cpp
LOCAL_LDLIBS +=  -lpthread 
LOCAL_CFLAGS += -rdynamic
LOCAL_MODULE := CallStack 
LOCAL_SHARED_LIBRARIES := libc libutils

include $(BUILD_EXECUTABLE)
####################################