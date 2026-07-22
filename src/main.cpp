#include <dlfcn.h>
#include <jni.h>
#include <string.h>
#include <android/log.h>

#include "zygisk.hpp"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "FridaGadget", __VA_ARGS__)

static JavaVM* g_jvm = nullptr;

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
    void onLoad(const zygisk::AppSpecializeArgs* args) override {
        // 保存 JavaVM 引用
        if (args && args->app_data_dir) {
            // 通过 env 获取 JavaVM
        }
    }

    void preAppSpecialize(const zygisk::AppSpecializeArgs* args) override {
        // 检查是否是淘宝进程
        if (!args || !args->nice_name) return;

        JNIEnv* env = nullptr;
        if (g_jvm) {
            g_jvm->AttachCurrentThread(&env, nullptr);
        }
        if (!env) return;

        const char* name = env->GetStringUTFChars(*args->nice_name, nullptr);
        if (name) {
            if (strstr(name, "com.taobao.taobao") != nullptr) {
                // 只在主进程注入，不注入子进程（如 :channel）
                if (strchr(name, ':') == nullptr) {
                    LOGD("taobao main process detected, injecting gadget...");
                    inject_gadget();
                }
            }
            env->ReleaseStringUTFChars(*args->nice_name, name);
        }
    }

    void postAppSpecialize(const zygisk::AppSpecializeArgs* args) override {}
    void preServerSpecialize(const zygisk::ServerSpecializeArgs* args) override {}
    void postServerSpecialize(const zygisk::ServerSpecializeArgs* args) override {}

    void onDestroy() override {
        if (g_jvm) {
            g_jvm->DetachCurrentThread();
        }
    }
};

// Zygisk 模块入口
REGISTER_ZYGISK_MODULE(FridaGadgetModule)
