cmake_minimum_required ( VERSION 3.9 )

project ( v-rex-term C )

# Set the project source dir (just convention)
set( V-REX-TERM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( V-REX-TERM_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )

# version numbers in semver style
# matching the release tag version
set ( V-REX-TERM_VERSION_MAJOR "0" )
set ( V-REX-TERM_VERSION_MINOR "1" )
set ( V-REX-TERM_VERSION_PATCH "0" )
set ( V-REX-TERM_VERSION_PRERELEASE "-alpha.0" )
set ( V-REX-TERM_VERSION_STR ${V-REX-TERM_VERSION_MAJOR}.${V-REX-TERM_VERSION_MINOR}.${V-REX-TERM_VERSION_PATCH}${V-REX-TERM_VERSION_PRERELEASE})

# https://stackoverflow.com/a/47260387/9483968
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# https://stackoverflow.com/a/63037953
include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)

if (HAVE_LIB_M)
  set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif (HAVE_LIB_M)

# Include Directories
# In GCC, this will invoke the "-I" command
include_directories( src )

# Lists
# Setup the list of source files
set( V-REX-TERM_SOURCES
    src/main.c
)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" AND LUA_FROM_PKGCONFIG)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(lua REQUIRED IMPORTED_TARGET lua)
endif()

include(FindLua)
if (LUA_FOUND)
  message("-- lua found at ${LUA_LIBRARIES} ${LUA_INCLUDE_DIR}")
  message("-- lua version is ${LUA_VERSION_STRING}")
  add_compile_definitions(LUA_ENABLED)
else ()
  message( FATAL_ERROR " -- ERROR: lua not found.")
endif (LUA_FOUND)

add_executable( ${PROJECT_NAME} ${V-REX-TERM_SOURCES} )
target_include_directories(${PROJECT_NAME} PUBLIC src)

if (LUA_FOUND)
  target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
endif (LUA_FOUND)

# !!!!NOTE/WARNING!!!
# The coll package is placed ahead of json-c such that arraylist.h from coll is used.
# TODO: coll package should be fixed to use another header file name.
find_package(coll CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC coll::coll)

find_package(clibdocker CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC clibdocker::clibdocker)

find_package(zclk CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC zclk::zclk)

# see mustach documentation for this
add_definitions(-DNO_OPEN_MEMSTREAM)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
set(CMAKE_FIND_USE_PACKAGE_REGISTRY ON)

find_package(json-c CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC json-c::json-c)

find_package(CURL CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC CURL::libcurl)

# To find and use libarchive, as the vcpkg build does not have cmake config
# See https://github.com/microsoft/vcpkg/issues/8839#issuecomment-558066466
# for additional lookup to ZLIB
find_package(ZLIB REQUIRED)
find_package(lz4 REQUIRED)
find_package(BZip2 REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(LibLZMA REQUIRED)
find_path(LIBARCHIVE_INCLUDE_DIR archive.h)
find_library(LIBARCHIVE_LIBRARY archive)
include_directories(${LIBARCHIVE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBARCHIVE_LIBRARY} lz4::lz4 BZip2::BZip2 LibXml2::LibXml2 LibLZMA::LibLZMA ZLIB::ZLIB)
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})
