#include <dlfcn.h>
#include <jni.h>
#include <string.h>
#include <android/log.h>

#include "zygisk.hpp"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "FridaGadget", __VA_ARGS__)

static JNIEnv* g_env = nullptr;

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
    void onLoad(zygisk::Api *api, JNIEnv *env) override {
        g_env = env;
        LOGD("onLoad called, JNI env=%p", env);
    }

    void preAppSpecialize(zygisk::AppSpecializeArgs *args) override {
        LOGD("preAppSpecialize called");
        
        if (!g_env) {
            LOGD("g_env is null");
            return;
        }
        
        const char* name = g_env->GetStringUTFChars(args->nice_name, nullptr);
        if (name) {
            LOGD("process name: %s", name);
            if (strstr(name, "com.taobao.taobao") != nullptr) {
                if (strchr(name, ':') == nullptr) {
                    LOGD("taobao main process detected, injecting gadget...");
                    inject_gadget();
                }
            }
            g_env->ReleaseStringUTFChars(args->nice_name, name);
        }
    }

    void postAppSpecialize(const zygisk::AppSpecializeArgs *args) override {}

    void preServerSpecialize(zygisk::ServerSpecializeArgs *args) override {}
    void postServerSpecialize(const zygisk::ServerSpecializeArgs *args) override {}
};

// Zygisk 模块入口
REGISTER_ZYGISK_MODULE(FridaGadgetModule)
