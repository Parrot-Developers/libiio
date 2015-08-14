LOCAL_PATH := $(call my-dir)

###############################################################################
# libiio
###############################################################################

include $(CLEAR_VARS)

LOCAL_MODULE := libiio
LOCAL_DESCRIPTION := Library for Linux Industrial I/O (IIO) devices
LOCAL_CATEGORY_PATH := libs/libiio

LOCAL_LIBRARIES := libxml2

LOCAL_SRC_FILES := \
	channel.c \
	device.c \
	context.c \
	buffer.c \
	utilities.c \
	plugins.c \
	local.c \
	network.c \
	xml.c

LOCAL_CFLAGS := -DHAVE_IPV6=1 \
	-DHAVE_PTHREAD=0 \
	-DLIBIIO_EXPORTS=1 \
	-DLIBIIO_VERSION_GIT=\"dc05765\" \
	-DLIBIIO_VERSION_MAJOR=0 \
	-DLIBIIO_VERSION_MINOR=5 \
	-DLOCAL_BACKEND=1 \
	-DNETWORK_BACKEND=1 \
	-DWITH_NETWORK_GET_BUFFER=1\
	 -D_GNU_SOURCE=1 \
	-D_POSIX_C_SOURCE=200809L \
	-Diio_EXPORTS \
	-fvisibility=hidden

LOCAL_LDLIBS := -ldl

include $(BUILD_LIBRARY)

###############################################################################
# libiio-plugins-private
###############################################################################

include $(CLEAR_VARS)

LOCAL_MODULE := libiio-plugins-private
LOCAL_DESCRIPTION := Export of libiio headers for plugin implementors
LOCAL_CATEGORY_PATH := libs/libiio

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_CUSTOM)
