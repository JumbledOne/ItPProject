cmake_minimum_required(VERSION 3.20)
project(MY_PROJECT LANGUAGES C CXX)

# Keep the source list automatically up to date as files are added below src.
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
)

if(NOT APP_SOURCES)
    message(FATAL_ERROR "No C or C++ source files were found under ${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

add_executable(app ${APP_SOURCES})

# Add src and every folder below it (any depth) as include directories, so
# headers can be included as <account.h> regardless of which subfolder they are in.
file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hxx"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")
foreach(header ${APP_HEADERS})
    get_filename_component(header_dir "${header}" DIRECTORY)
    list(APPEND APP_INCLUDE_DIRS "${header_dir}")
endforeach()
list(REMOVE_DUPLICATES APP_INCLUDE_DIRS)

target_include_directories(app PRIVATE ${APP_INCLUDE_DIRS})
target_compile_features(app PRIVATE cxx_std_17)

# Put the executable directly in the repository's build directory for every
# configuration, including multi-config generators such as Visual Studio.
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/build"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/build"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/build"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/build"
)
