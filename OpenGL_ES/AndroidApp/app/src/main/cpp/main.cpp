#include <GLES3/gl31.h>
// EGL
// https://www.khronos.org/egl
#include <EGL/egl.h>
// EGL Extension
#include <EGL/eglext.h>

#include <android_native_app_glue.h>

#define LOG_MSG_FLAG
//#undef LOG_MSG_FLAG
#define LOG_TAG "Native-Activity"
#include "log.h"

void android_main(struct android_app* app) {
    LOGD("[S]android_main(%p)", app);

    LOGD("[E]android_main()");
}
