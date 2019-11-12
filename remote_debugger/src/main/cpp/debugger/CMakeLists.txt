cmake_minimum_required(VERSION 3.6)

set(UTILS_SOURCE
        Debugger.cpp
        JdwpPacket.cpp
        JdwpPacket.cpp
        )

add_library(debugger
        STATIC
        ${UTILS_SOURCE}
        ${TRAMPOLINE}
        )


include_directories(flinker)

target_link_libraries(debugger flinker ${log-lib})