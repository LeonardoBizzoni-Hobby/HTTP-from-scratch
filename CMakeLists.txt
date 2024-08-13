cmake_minimum_required(VERSION 3.29)
project(http CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHITE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set output directories
set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

file(GLOB SRC "src/*.cpp" "src/*.hpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SRC})

target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_SRC})
target_compile_definitions(${PROJECT_NAME} PRIVATE ROOTDIR="${CMAKE_SOURCE_DIR}")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
                      ARCHITE_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
                      LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

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





if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Workaround for std::expected not available in clang
  add_compile_options(
    -stdlib=libstdc++ -D__cpp_concepts=202002 -Wno-builtin-macro-redefined
  )
endif()
