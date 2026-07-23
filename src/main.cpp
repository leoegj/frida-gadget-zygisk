#include <dlfcn.h>
#include <jni.h>
#include <string.h>
#include <android/log.h>

#include "zygisk.hpp"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "FridaGadget", __VA_ARGS__)

static JavaVM* get_jvm() {
    static JavaVM* jvm = nullptr;
    if (jvm) return jvm;
    typedef jint (*GetVMs_t)(JavaVM**, jsize, jsize*);
    GetVMs_t func = (GetVMs_t)dlsym(RTLD_DEFAULT, "JNI_GetCreatedJavaVMs");
    if (!func) {
        void* handle = dlopen("libnativehelper.so", RTLD_NOW);
        if (handle) func = (GetVMs_t)dlsym(handle, "JNI_GetCreatedJavaVMs");
    }
    if (func) {
        jsize count = 0;
        func(&jvm, 1, &count);
        if (count == 0) jvm = nullptr;
    }
    return jvm;
}

static void inject_gadget() {
    void* handle = dlopen("/data/local/tmp/libfrida-gadget.so", RTLD_NOW | RTLD_GLOBAL);
    if (handle) {
        LOGD("frida-gadget loaded successfully");
    } else {
        LOGD("frida-gadget load failed: %s", dlerror());
    }
}

class FridaGadgetModule : public zygisk::ModuleBase {
public:
    void preAppSpecialize(zygisk::AppSpecializeArgs *args) override {
        LOGD("preAppSpecialize called");
        
        if (!args || !args->nice_name) {
            LOGD("args or nice_name is null");
            return;
        }

        JavaVM* jvm = get_jvm();
        if (!jvm) {
            LOGD("get_jvm failed");
            return;
        }

        JNIEnv* env = nullptr;
        jvm->AttachCurrentThread(&env, nullptr);
        if (!env) {
            LOGD("AttachCurrentThread failed");
            return;
        }

        const char* name = env->GetStringUTFChars(*args->nice_name, nullptr);
        if (name) {
            LOGD("process name: %s", name);
            if (strstr(name, "com.taobao.taobao") != nullptr) {
                if (strchr(name, ':') == nullptr) {
                    LOGD("taobao main process detected, injecting gadget...");
                    inject_gadget();
                }
            }
            env->ReleaseStringUTFChars(*args->nice_name, name);
        }
    }

    void postAppSpecialize(const zygisk::AppSpecializeArgs *args) override {}
    void preServerSpecialize(zygisk::ServerSpecializeArgs *args) override {}
    void postServerSpecialize(const zygisk::ServerSpecializeArgs *args) override {}
};

REGISTER_ZYGISK_MODULE(FridaGadgetModule)
