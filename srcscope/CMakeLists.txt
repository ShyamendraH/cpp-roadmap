# Testable logic lives in a library: main.cpp stays thin, and the
# tests link the same code without inheriting main().
add_library(srcscope_core
    src/counting.cpp)

# Headers live in src/ beside the sources. PUBLIC means the include
# path reaches everything that links srcscope_core.
target_include_directories(srcscope_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(srcscope
    src/main.cpp)

target_link_libraries(srcscope PRIVATE srcscope_core)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
