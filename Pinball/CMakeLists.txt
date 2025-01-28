cmake_minimum_required(VERSION 3.7)
set(PROJECT pinball)
project(${PROJECT})

# Add global definitions
add_definitions("-Wall")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Define the assets directory (adjust the path as needed)
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/Game/Assets)

# Create an 'assets' folder in the build output directory
set(ASSETS_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Assets)

add_executable(${PROJECT} ${SOURCE_FILES})

# Using C++14
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED 14)

# Copy all files from the assets directory to the output directory
add_custom_command(TARGET ${PROJECT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR} ${ASSETS_OUTPUT_DIR}
        COMMENT "Copying assets to the output directory"
)

message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}
CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

# gcc 5 and clang 3.4 at least to support c++14
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4)
    message(FATAL_ERROR "Require at least clang++-3.4")
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1)
    message(FATAL_ERROR "Require at least g++-5.1")
endif ()

# Find SDL2
INCLUDE(FindPkgConfig REQUIRED)
PKG_CHECK_MODULES(SDL2 REQUIRED sdl2)
PKG_CHECK_MODULES(SDL2_IMAGE REQUIRED SDL2_image)
PKG_CHECK_MODULES(SDL2mixer REQUIRED SDL2_mixer)
PKG_CHECK_MODULES(Box2d REQUIRED Box2d)

# Platform-specific configurations
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DGL_SILENCE_DEPRECATION)
    target_link_libraries(${PROJECT} "-framework Metal -framework Foundation")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_compile_definitions(_USE_MATH_DEFINES)
    add_compile_definitions(NOMINMAX)
    target_compile_definitions(${PROJECT} PRIVATE /wd4251)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

message("SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}
SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_IMAGE_INCLUDE_DIRS}
SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_IMAGE_INCLUDE_DIRS}
SDL2mixer_INCLUDE_DIRS: ${SDL2mixer_INCLUDE_DIRS}
Box2d_INCLUDE_DIRS: ${Box2d_INCLUDE_DIRS}
")
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2mixer_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include ${Box2d_INCLUDE_DIRS})

message("SDL2_LINK_LIBRARIES: ${SDL2_LINK_LIBRARIES}
SDL2_IMAGE_LINK_LIBRARIES: ${SDL2_IMAGE_LINK_LIBRARIES}
SDL2mixer_LINK_LIBRARIES: ${SDL2mixer_LINK_LIBRARIES}
Box2d_LINK_LIBRARIES: ${Box2d_LINK_LIBRARIES}
")
target_link_libraries(${PROJECT} ${SDL2_LINK_LIBRARIES} ${SDL2_IMAGE_LINK_LIBRARIES} ${SDL2mixer_LINK_LIBRARIES} ${Box2d_LINK_LIBRARIES})

# Install the target and resources
install(TARGETS ${PROJECT} DESTINATION ..)
install(DIRECTORY ${ASSETS_OUTPUT_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/assets)
