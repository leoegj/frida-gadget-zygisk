# Zygisk Frida Gadget Inject
# 在淘宝进程启动时注入 libfrida-gadget.so

# 模块目录
MODDIR=${0%/*}

# 确保 gadget.so 已部署到 /data/local/tmp/
if [ ! -f /data/local/tmp/libfrida-gadget.so ]; then
    cp "$MODDIR/libs/libfrida-gadget.so" /data/local/tmp/libfrida-gadget.so
    chmod 755 /data/local/tmp/libfrida-gadget.so
    chcon u:object_r:system_file:s0 /data/local/tmp/libfrida-gadget.so 2>/dev/null
fi

if [ ! -f /data/local/tmp/frida-gadget.config ]; then
    cp "$MODDIR/libs/frida-gadget.config" /data/local/tmp/frida-gadget.config
    chmod 644 /data/local/tmp/frida-gadget.config
fi
