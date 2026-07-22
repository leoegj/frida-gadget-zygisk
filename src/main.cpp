#include <dlfcn.h>
#include <jni.h>
#include <string.h>
#include <android/log.h>

#include "zygisk.hpp"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "FridaGadget", __VA_ARGS__)

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
        // 默认不操作
    }

    void preAppSpecialize(const zygisk::AppSpecializeArgs* args) override {
        // 检查是否是淘宝进程
        if (args && args->nice_name) {
            const char* name = args->nice_name;
            if (strstr(name, "com.taobao.taobao") != nullptr) {
                // 只在主进程注入，不注入子进程（如 :channel）
                if (strchr(name, ':') == nullptr) {
                    LOGD("taobao main process detected, injecting gadget...");
                    inject_gadget();
                }
            }
        }
    }

    void postAppSpecialize(const zygisk::AppSpecializeArgs* args) override {}
    void preServerSpecialize(const zygisk::ServerSpecializeArgs* args) override {}
    void postServerSpecialize(const zygisk::ServerSpecializeArgs* args) override {}
};

REGISTER_ZYGISK_MODULE(FridaGadgetModule)
