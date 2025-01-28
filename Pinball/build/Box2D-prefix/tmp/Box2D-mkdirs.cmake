# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/src/Box2D")
  file(MAKE_DIRECTORY "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/src/Box2D")
endif()
file(MAKE_DIRECTORY
  "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/src/Box2D-build"
  "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix"
  "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/tmp"
  "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/src/Box2D-stamp"
  "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/src"
  "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/src/Box2D-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/src/Box2D-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/cex/dev/repository/personal/sdl-pinball/Pinball/build/Box2D-prefix/src/Box2D-stamp${cfgdir}") # cfgdir has leading slash
endif()
