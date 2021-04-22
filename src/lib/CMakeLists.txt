add_library(NVMLWatcher NVMLWatcher.cpp)
target_include_directories(NVMLWatcher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(NVMLWatcher PUBLIC Boost::boost NVML::NVML)
