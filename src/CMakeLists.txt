cmake_minimum_required(VERSION 3.10)
project(frida-gadget-zygisk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译 Zygisk 模块 (.so)
add_library(zygisk_gadget SHARED main.cpp)

# 链接 Android 的 log 库
target_link_libraries(zygisk_gadget log)

# 设置输出目录为项目根目录下的 zygisk/
set_target_properties(zygisk_gadget PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/zygisk/${ANDROID_ABI}"
    PREFIX ""
)
