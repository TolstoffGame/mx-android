LOCAL_PATH := $(call my-dir)
# Чтобы $(call import-module,...) искал модули внутри текущего каталога jni
NDK_MODULE_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)
LOCAL_MODULE := main
LOCAL_SRC_FILES := src/main.c src/eng.c src/assets.c
LOCAL_CFLAGS := -O2
LOCAL_LDLIBS := -llog -landroid -lGLESv2 -lEGL -lm
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image
include $(BUILD_SHARED_LIBRARY)

# Импортируем модули, которые мы положили как jni/SDL2 и jni/SDL2_image
$(call import-module,SDL2)
$(call import-module,SDL2_image)
