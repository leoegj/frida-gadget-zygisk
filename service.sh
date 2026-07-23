#!/system/bin/sh
# 兜底：启动后重设 zygisksu enforce = just_umount
for i in 1 2 3 4 5; do
    if /data/adb/ksu/bin/znctl status >/dev/null 2>&1; then
        /data/adb/ksu/bin/znctl enforce-denylist just_umount 2>/dev/null
        break
    fi
    sleep 2
done
