
#include <iostream>
#include <sstream>
#include "NVMLWatcher.hpp"

std::string NVMLDeviceWatcher::utilization() const {
  std::stringstream ss;
  nvmlUtilization_t util;
  NVML_RT_CALL(nvmlDeviceGetUtilizationRates(device_, &util));
  ss << "gpu_processor_utilizaton{card=" << device_id_ << "} " << util.gpu
    << "\ngpu_memory_utilizaton{card=" << device_id_ << "} " << util.memory
    << "\n";
  return ss.str();
}

std::string NVMLDeviceWatcher::memory() const {
  std::stringstream ss;
  nvmlMemory_t memory;
  NVML_RT_CALL(nvmlDeviceGetMemoryInfo(device_, &memory));
  ss << "gpu_memory_used{card=" << device_id_ << "} " << memory.used
    << "\ngpu_memory_total{card=" << device_id_ << "} " << memory.total
    << "\n";
  return ss.str();
}

std::string NVMLDeviceWatcher::temperature() const {
  std::stringstream ss;
  unsigned int temperature;
  NVML_RT_CALL(nvmlDeviceGetTemperature(device_, NVML_TEMPERATURE_GPU, &temperature));
  ss << "gpu_temperature{card=" << device_id_ << "} " << temperature << "\n";
  return ss.str();
}
