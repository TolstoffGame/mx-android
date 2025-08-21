LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := main
LOCAL_SRC_FILES := src/main.c src/eng.c src/assets.c
LOCAL_CFLAGS := -O2
LOCAL_LDLIBS := -llog -landroid -lGLESv2 -lEGL -lm
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image
include $(BUILD_SHARED_LIBRARY)

# SDL2 (папка SDL лежит в соседнем модуле android-project)
$(call import-module,SDL)
# SDL_image (мы только что склонировали в jni/SDL_image)
$(call import-module,SDL_image)
