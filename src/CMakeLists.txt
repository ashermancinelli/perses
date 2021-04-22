add_subdirectory(lib)
add_executable(${PROJECT_NAME} driver.cpp)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  NVMLWatcher
  pthread
  "-L${Boost_LIBRARY_DIR_RELEASE}"
  boost_system
  boost_iostreams
  )
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
