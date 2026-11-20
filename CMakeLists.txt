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

target_include_directories(app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
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
