#include <jni.h>
#include <android/log.h>
#include <Debugger.h>

#define  LOG_TAG    "wkigen_jni"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static const char* const kClassJni = "com/github/wkigen/RemoteDebugger/Debugger";


SocketDebug::Debugger socketDebugger;

void jswpSocketDebugOpen(){
    ALOG("socketDebugOpen");
    socketDebugger.openJDWP();
}

static const JNINativeMethod gMethods[] = {
        {"jswpSocketDebugOpen", "()V", (void*)jswpSocketDebugOpen}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved){
    ALOG("JNI_OnLoad");
    JNIEnv* env = NULL;
    if(vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) //从JavaVM获取JNIEnv，一般使用1.4的版本
        return JNI_ERR;
    jclass clazz = env->FindClass(kClassJni);
    if (clazz == NULL) {
        ALOG("cannot find class");
        return JNI_ERR;
    }
    if(env->RegisterNatives(clazz, gMethods, sizeof(gMethods)/sizeof(gMethods[0]))){
        ALOG("register native method failed!\n");
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

