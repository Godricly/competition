cmake_minimum_required (VERSION 3.5)

project(image-noise-generator CXX)

set(ING_VERSION_MAJOR 0)
set(ING_VERSION_MINOR 1)
set(ING_VERSION_PATCH 0)
set(ING_VERSION "${ING_VERSION_MAJOR}.${ING_VERSION_MINOR}.${ING_VERSION_PATCH}")

# ONLY one switch is ON !!
option(USING_CUDA "using cuda to calculate." OFF)

if(USING_CUDA)
    set(USING_CUDA_FLAG "-DUSE_CUDNN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USING_CUDA_FLAG}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

## switcher for debug
option(GDB_DEBUGING "debug." OFF)
if(GDB_DEBUGING) 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

message("CMAKE CXX FLAGS: " ${CMAKE_CXX_FLAGS})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

aux_source_directory(src/core DIR_SRCS)
message("DIR_SRCS: " ${DIR_SRCS})

aux_source_directory(src MAIN_SRC)
message("MAIN_SRC: " ${MAIN_SRC})

add_executable(ing.bin ${DIR_SRCS} ${MAIN_SRC})

set_target_properties(ing.bin PROPERTIES OUTPUT_NAME ing)

if(BUILD_LIBRARY)
    add_library(ing_dynamic SHARED ${DIR_SRCS})
    add_library(ing_static STATIC ${DIR_SRCS})
endif()
