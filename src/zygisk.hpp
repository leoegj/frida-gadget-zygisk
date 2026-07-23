#ifndef ZYGISK_API_HPP
#define ZYGISK_API_HPP

#include <jni.h>
#include <stdint.h>

namespace zygisk {

struct AppSpecializeArgs {
    jint uid;
    jint pid;
    jint *gid_arr;
    jint *runtime_flags;
    jint &mount_mode;
    jint &mount_state;
    jint &permissive;
    jint *seinfo;
    jstring *nice_name;
    jstring *app_data_dir;
    jstring *seccomp_policy;
    bool &is_child_zygote;
    bool &is_top_app;
    jintArray &bg_category;
};

struct ServerSpecializeArgs {
    jint uid;
    jint pid;
    jint *gid_arr;
    jint *runtime_flags;
    jint &mount_mode;
    jint &permissive;
    jstring *nice_name;
};

class ModuleBase {
public:
    virtual ~ModuleBase() = default;
    virtual void onLoad(const AppSpecializeArgs &args) {}
    virtual void preAppSpecialize(AppSpecializeArgs *args) {}
    virtual void postAppSpecialize(const AppSpecializeArgs *args) {}
    virtual void preServerSpecialize(ServerSpecializeArgs *args) {}
    virtual void postServerSpecialize(const ServerSpecializeArgs *args) {}
};

#define REGISTER_ZYGISK_MODULE(module) \
    extern "C" __attribute__((visibility("default"))) __attribute__((used)) \
    void* zygisk_module_entry() { return new module(); }

} // namespace zygisk

#endif // ZYGISK_API_HPP
