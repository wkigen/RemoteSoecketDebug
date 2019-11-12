cmake_minimum_required(VERSION 3.6)

set(UTILS_SOURCE
        fake_dlfcn.c
        system_utils.c
        )

add_library(flinker
        STATIC
        ${UTILS_SOURCE}
        ${TRAMPOLINE}
        )

target_link_libraries(flinker)