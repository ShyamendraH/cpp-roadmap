cmake_minimum_required(VERSION 3.20)

project(cpp_systems_builder
    VERSION 0.1.0
    DESCRIPTION "C++ Systems Builder: srcscope, tinycore, forge, termkit, httpkit"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release, RelWithDebInfo)" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_compile_options(-Wall -Wextra -Wpedantic)

option(ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

include(CTest)

add_subdirectory(srcscope)
# add_subdirectory(tinycore)  # Week 5
# add_subdirectory(forge)     # Week 13
# add_subdirectory(termkit)   # Week 21
# add_subdirectory(httpkit)   # Week 29
