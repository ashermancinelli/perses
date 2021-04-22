#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <nvml.h>

#define NVML_RT_CALL(call) \
    { \
        auto status = static_cast<nvmlReturn_t>( call ); \
        if ( status != NVML_SUCCESS ) \
            fprintf(stderr, \
                    "ERROR: CUDA NVML call\"%s\" in line %d of file %s " \
                    "failed with %s (%d).\n", \
                    #call, \
                    __LINE__, \
                    __FILE__, \
                    nvmlErrorString(status), \
                    status); \
    }

struct NVMLDeviceWatcher {
  NVMLDeviceWatcher(int id=0) : device_id_{id} {
    NVML_RT_CALL(nvmlDeviceGetHandleByIndex(device_id_, &device_));
  }

  [[nodiscard]]
  std::string utilization() const;

  [[nodiscard]]
  std::string memory() const;

  [[nodiscard]]
  std::string temperature() const;

  friend std::ostream& operator<<(std::ostream& os,
      NVMLDeviceWatcher& watcher) {
    os << watcher.utilization() << watcher.memory() << watcher.temperature();
    return os;
  }
private:
  int device_id_;
  nvmlDevice_t device_;
};

struct NVMLWatcher {
  /** Initialize watcher for device ids passed by initialization list.
   *
   * Example:
   *
   * ```cpp
   * NVMLWatcher w({0, 3, 4, 5});
   * std::cout << w;
   * ```
   */
  NVMLWatcher(std::initializer_list<int> ids) {
    NVML_RT_CALL(nvmlInit());
    if (ids.size() == 0) {
      unsigned int devcount;
      NVML_RT_CALL(nvmlDeviceGetCount_v2(&devcount));
      initCardRange(devcount);
      return;
    }
    for (auto const& i : ids) {
      dev_watchers_.push_back(std::make_unique<NVMLDeviceWatcher>(i));
    }
  }

  NVMLWatcher(int num_ids) {
    NVML_RT_CALL(nvmlInit());
    initCardRange(num_ids);
  }

  ~NVMLWatcher() {
    NVML_RT_CALL(nvmlShutdown());
  }

  /** Initializes a range of cards */
  inline void initCardRange(int range) {
    for (int i=0; i<range; i++) {
      dev_watchers_.push_back(std::make_unique<NVMLDeviceWatcher>(i));
    }
  }

  friend std::ostream& operator<<(std::ostream& os, NVMLWatcher& nvml) {
    for(auto &watcher : nvml.dev_watchers_) {
      os << *watcher;
    }
    return os;
  }
private:
  std::vector<std::unique_ptr<NVMLDeviceWatcher>> dev_watchers_;
};
