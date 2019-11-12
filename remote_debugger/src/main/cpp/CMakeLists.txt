
cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ${WARNING_FLAGS}  -std=gnu11 -pthread -marm ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -std=c++11 -pthread ")

if (${ANDROID_ABI} STREQUAL "x86")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -std=c++11 -pthread  -D__X86__")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -std=c++11 -pthread -D__ARM__ ")
endif ()

add_library(
        remotesocket
        SHARED
        main.cpp
)

add_subdirectory(flinker)
include_directories(flinker)

add_subdirectory(debugger)
include_directories(debugger)


find_library(
        log-lib
        log)


target_link_libraries(
        remotesocket debugger
        ${log-lib})