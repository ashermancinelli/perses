
#[[

Exports target `Boost::boost`

Users may set the following variables:

- Boost_DIR

]]

find_library(BOOST_LIBRARY
  NAMES
  libboost_iostreams.so
  PATHS
  ${BOOST_DIR} $ENV{BOOST_DIR} ${Boost_DIR} $ENV{Boost_DIR}
  ENV LD_LIBRARY_PATH ENV DYLD_LIBRARY_PATH
  PATH_SUFFIXES
  lib64 lib)

if(BOOST_LIBRARY)
  get_filename_component(BOOST_LIBRARY_DIR ${BOOST_LIBRARY} DIRECTORY)
endif()

find_path(BOOST_INCLUDE_DIR
  NAMES
  filesystem.hpp
  PATHS
  ${BOOST_DIR} $ENV{BOOST_DIR} ${Boost_DIR} ${BOOST_LIBRARY_DIR}/..
  PATH_SUFFIXES
  include include/boost)

if(BOOST_LIBRARY)
  message(STATUS "Found boost include: ${BOOST_INCLUDE_DIR}")
  add_library(BOOST INTERFACE)
  target_link_libraries(BOOST INTERFACE
    ${BOOST_LIBRARY_DIR}/libboost_iostreams.so
    ${BOOST_LIBRARY_DIR}/libboost_system.so
    )
  target_include_directories(BOOST INTERFACE ${BOOST_INCLUDE_DIR})
  message(STATUS "Found BOOST library: ${BOOST_LIBRARIES}")
else()
  message(STATUS "BOOST was not found.")
endif()

set(BOOST_INCLUDE_DIR CACHE PATH "Path to boost.h")
set(BOOST_LIBRARIES CACHE PATH "Path to boost libraries")

