// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

#define MAKE_FN_NAME_NATIVE_INIT(x, y) Java_com_ ## x ##_## y ##_DemoRenderer_nativeInit
#define FUNCTION_NAME_NATIVE_INIT(x, y) MAKE_FN_NAME_NATIVE_INIT(x, y)

#define MAKE_FN_NAME_NATIVE_RESIZE(x, y) Java_com_ ## x ##_## y ##_DemoRenderer_nativeResize
#define FUNCTION_NAME_NATIVE_RESIZE(x, y) MAKE_FN_NAME_NATIVE_RESIZE(x, y)

#define MAKE_FN_NAME_NATIVE_PAUSE(x, y) Java_com_ ## x ##_## y ##_DemoGLSurfaceView_nativePause
#define FUNCTION_NAME_NATIVE_PAUSE(x, y) MAKE_FN_NAME_NATIVE_PAUSE(x, y)

#define MAKE_FN_NAME_NATIVE_RESUME(x, y) Java_com_ ## x ##_## y ##_DemoGLSurfaceView_nativeResume
#define FUNCTION_NAME_NATIVE_RESUME(x, y) MAKE_FN_NAME_NATIVE_RESUME(x, y)

#define MAKE_FN_NAME_NATIVE_RENDER(x, y) Java_com_ ## x ##_## y ##_DemoRenderer_nativeRender
#define FUNCTION_NAME_NATIVE_RENDER(x, y) MAKE_FN_NAME_NATIVE_RENDER(x, y)

// Test
/*
#pragma message(VAR_NAME_VALUE(FUNCTION_NAME_NATIVE_INIT(COMPANY, APPNAME)))
#pragma message(VAR_NAME_VALUE(FUNCTION_NAME_NATIVE_RESIZE(COMPANY, APPNAME)))
#pragma message(VAR_NAME_VALUE(FUNCTION_NAME_NATIVE_PAUSE(COMPANY, APPNAME)))
#pragma message(VAR_NAME_VALUE(FUNCTION_NAME_NATIVE_RESUME(COMPANY, APPNAME)))
#pragma message(VAR_NAME_VALUE(FUNCTION_NAME_NATIVE_RENDER(COMPANY, APPNAME)))
*/

#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <unistd.h>
#include <amju.h>
#include <AmjuGL-OpenGLES.2.h>

using namespace Amju;

int   gAppAlive   = 1;

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

static long
_getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}

extern "C" void 
FUNCTION_NAME_NATIVE_INIT(COMPANY, APPNAME)
(JNIEnv* env)
{
    // Android: wait for debugger, or any crash at startup will be too soon
    // Thanks: http://stackoverflow.com/questions/17705109/how-to-debug-c-c-code-ndk-in-eclipse
    usleep(5000 * 1000);

    __android_log_print(ANDROID_LOG_INFO, "jc", "In native init!");

    AmjuGL::SetImpl(new AmjuGLOpenGLES2);
    AmjuGL::Init();

    TheEventPoller::Instance()->SetImpl(new EventPollerImpl);

    StartUpBeforeCreateWindow();
    StartUpAfterCreateWindow();

    gAppAlive  = 1;
}

extern "C" void 
FUNCTION_NAME_NATIVE_RESIZE(COMPANY, APPNAME)
(JNIEnv* env, jobject thiz, jint w, jint h)
{
    sWindowWidth  = w;
    sWindowHeight = h;
}

void _pause()
{
  /* we paused the animation, so store the current
   * time in sTimeStopped for future nativeRender calls */
    sDemoStopped = 1;
    sTimeStopped = _getTime();

  // j.c. TODO Call pause in Game
    __android_log_print(ANDROID_LOG_INFO, "jc try this", "pause");
}

void _resume()
{
  /* we resumed the animation, so adjust the time offset
   * to take care of the pause interval. */
    sDemoStopped = 0;
    sTimeOffset -= _getTime() - sTimeStopped;

  // j.c. No "unpause"
    __android_log_print(ANDROID_LOG_INFO, "jc try this", "resume");
}

extern "C" void 
FUNCTION_NAME_NATIVE_PAUSE(COMPANY, APPNAME)
(JNIEnv* env)
{
    _pause();
}

extern "C" void 
FUNCTION_NAME_NATIVE_RESUME(COMPANY, APPNAME)
(JNIEnv* env)
{
    // TODO Can we get rid of this??
    _resume();
}

extern "C" void 
FUNCTION_NAME_NATIVE_RENDER(COMPANY, APPNAME)
(JNIEnv* env)
{
    long   curTime;

    /* NOTE: if sDemoStopped is TRUE, then we re-render the same frame
     *       on each iteration.
     */
    if (sDemoStopped) {
        curTime = sTimeStopped + sTimeOffset;
    } else {
        curTime = _getTime() + sTimeOffset;
        if (sTimeOffsetInit == 0) {
            sTimeOffsetInit = 1;
            sTimeOffset     = -curTime;
            curTime         = 0;
        }
    }

    Screen::SetSize(sWindowWidth, sWindowHeight);
    TheGame::Instance()->RunOneLoop();
}
