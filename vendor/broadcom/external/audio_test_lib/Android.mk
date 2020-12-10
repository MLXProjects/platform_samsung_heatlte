LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

ALSALIB_HEAD_PATH := $(LOCAL_PATH)/../alsa-lib/include
ALSALIB_CONTROL_PATH := $(LOCAL_PATH)/../alsa-lib/src/control
LOCAL_C_INCLUDES := $(ALSALIB_HEAD_PATH)
LOCAL_C_INCLUDES += $(ALSALIB_CONTROL_PATH)
LOCAL_C_INCLUDES += $(TOP)/kernel/include/sound
LOCAL_C_INCLUDES += $(TOP)/vendor/broadcom/include
LOCAL_C_INCLUDES += $(TOP)/vendor/broadcom/amhal

LOCAL_CFLAGS := -D_POSIX_SOURCE
LOCAL_CFLAGS += -Werror

LOCAL_SRC_FILES := audio_test_lib_wrap.c audio_test_lib.c audio_test_lib_cdc_d2153.c
LOCAL_MODULE := libaudio_test
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := libasound liblog libcutils libutils

include $(BUILD_SHARED_LIBRARY)
