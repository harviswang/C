LOCAL_PATH:= $(call my-dir)

####################################
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES += $(LOCAL_PATH) \
                    system/core/include
LOCAL_SRC_FILES += unwind_backtrace.c unwind_backtrace_test.c
LOCAL_LDLIBS +=
LOCAL_CFLAGS += -Wall -Werror -g -O0
LOCAL_MODULE := unwind_backtrace_test 
LOCAL_SHARED_LIBRARIES := libc libcutils libdl libcorkscrew

include $(BUILD_EXECUTABLE)
####################################
