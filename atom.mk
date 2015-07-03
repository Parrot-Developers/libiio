LOCAL_PATH := $(call my-dir)

###############################################################################
# libiio
###############################################################################

include $(CLEAR_VARS)

LOCAL_MODULE := libiio
LOCAL_DESCRIPTION := Library for Linux Industrial I/O (IIO) devices
LOCAL_CATEGORY_PATH := libs/libiio

LOCAL_LIBRARIES := libxml2

LOCAL_EXPORT_LDLIBS = -liio

# cmake complains about the lack of pthread TODO get rid of this hack and find
# a real solution so that we can use iiod
LOCAL_CMAKE_CONFIGURE_ARGS := -DWITH_IIOD=FALSE

include $(BUILD_CMAKE)

