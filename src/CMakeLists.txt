add_subdirectory(lib)
add_executable(${PROJECT_NAME} driver.cpp)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  NVMLWatcher
  boost_system
  boost_iostreams
  )
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
