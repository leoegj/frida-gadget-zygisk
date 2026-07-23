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

# === 清理 zygisksu 排除列表，确保我们的 zygisk 模块能注入淘宝 ===
# post-fs-data.sh 执行顺序按模块名排序，frida_gadget_inject < zygisksu
# 所以我们运行在 zygisksu daemon 启动之前

if [ -f /data/adb/zygisksu/denylist ]; then
    # 清理 denylist（移除 taobao）
    grep -v "com.taobao.taobao" /data/adb/zygisksu/denylist > /data/adb/zygisksu/denylist.tmp 2>/dev/null
    if [ -s /data/adb/zygisksu/denylist.tmp ]; then
        cat /data/adb/zygisksu/denylist.tmp > /data/adb/zygisksu/denylist
    else
        echo -n > /data/adb/zygisksu/denylist
    fi
    rm -f /data/adb/zygisksu/denylist.tmp
fi

# 设置 enforce 为 1 (just_umount: 排除列表中的 App 仍会注入模块)
echo -n 1 > /data/adb/zygisksu/denylist_enforce
chmod 666 /data/adb/zygisksu/denylist_enforce
