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
    jint *mount_mode;
    jint *mount_state;
    jint *permissive;
    jint *seinfo;
    jstring *process;
    jstring *nice_name;
    jstring *app_data_dir;
    jstring *seccomp_policy;
    bool *is_child_zygote;
    bool *is_top_app;
    jintArray *bg_category;
    jint *fds_to_ignore;
    jint *storage_ce_uuid;
    jint *storage_ce_userid;
    jint *storage_ce_flags;
    jint *storage_ce_mnt_flags;
};

struct ServerSpecializeArgs {
    jint uid;
    jint pid;
    jint *gid_arr;
    jint *runtime_flags;
    jint *mount_mode;
    jint *permissive;
    jstring *process;
    jstring *nice_name;
    jstring *seccomp_policy;
};

class ModuleBase {
public:
    virtual ~ModuleBase() = default;
    virtual void onLoad(const zygisk::AppSpecializeArgs* args) {}
    virtual void preAppSpecialize(const zygisk::AppSpecializeArgs* args) {}
    virtual void postAppSpecialize(const zygisk::AppSpecializeArgs* args) {}
    virtual void preServerSpecialize(const zygisk::ServerSpecializeArgs* args) {}
    virtual void postServerSpecialize(const zygisk::ServerSpecializeArgs* args) {}
    virtual void onDestroy() {}
};

#define REGISTER_ZYGISK_MODULE(module) \
    extern "C" __attribute__((visibility("default"))) __attribute__((used)) \
    void* zygisk_module_entry() { return new module(); }

} // namespace zygisk

#endif // ZYGISK_API_HPP
