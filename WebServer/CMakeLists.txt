cmake_minimum_required(VERSION 3.29)
project(WebServer C)

# Set C++ standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

file(GLOB_RECURSE SANDBOX_SRC "src/*.c" "src/*.h")

add_executable(${PROJECT_NAME} ${SANDBOX_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE HTTP-lib)
target_compile_definitions(${PROJECT_NAME} PRIVATE ROOTDIR="${CMAKE_SOURCE_DIR}")

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
