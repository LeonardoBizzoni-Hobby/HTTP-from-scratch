cmake_minimum_required(VERSION 3.29)
project(HTTP-lib C)

# Set C++ standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

file(GLOB_RECURSE ENGINE_SRC "src/*.c" "src/*.h")
include_directories(src)

add_library(${PROJECT_NAME} STATIC ${ENGINE_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC src)

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
                      ARCHITE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
                      LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}")

# Configuration-specific settings
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:OptimizedDebug>:OPTDEBUG>
    $<$<CONFIG:Release>:RELEASE>)

# Configuration-specific optimizations
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:OptimizedDebug>:-O3>
    $<$<CONFIG:Release>:-O3>)
