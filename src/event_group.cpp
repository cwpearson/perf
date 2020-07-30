#include "event_group.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <asm/unistd.h>
#include <errno.h>
#include <unistd.h>

EventGroup::EventGroup() : fd_(-1) {}

EventGroup::~EventGroup() { close(fd_); }

void EventGroup::add(unsigned int type, unsigned long long config, unsigned long long config1) {
  struct perf_event_attr pe;
  std::memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.size = sizeof(struct perf_event_attr);
  pe.type = type;
  pe.config = config;
  pe.config1 = config1;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID | PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;

  pid_t pid = 0; // current thread
  int cpu = -1;  // all cpus
  unsigned long flags = 0;


  int ret = perf_event_open(&pe, pid, cpu, fd_, flags);

    fprintf(stderr, "post-open: %s\n", strerror(errno));

  if (-1 == ret) {
    fprintf(stderr, "error opening event: %s\n", strerror(errno));
    errno = 0;
    return;
  } else {
    printf("attached type=%d config=%llu\n", type, config);
  }

  // if no leader yet, this is our leader
  if (-1 == fd_) {
    fd_ = ret;
  }

}

void EventGroup::reset() { 
  // fprintf(stderr, "pre-reset(): %s\n", strerror(errno)); 
  ioctl(fd_, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  // fprintf(stderr, "post-reset(): %s\n", strerror(errno)); 
}

std::vector<long long> EventGroup::read() {

#if 0
  struct read_format {
    u64 nr;           /* The number of events */
    u64 time_enabled; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
    u64 time_running; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
    struct {
      u64 value; /* The value of the event */
      u64 id;    /* if PERF_FORMAT_ID */
    } values[nr];
  };
#endif
  std::vector<unsigned char> readData(8);

  while (-1 == ::read(fd_, readData.data(), readData.size())) {
    if (ENOSPC == errno) {
      errno = 0;
      readData.resize(readData.size() + 16);
    } else {
      fprintf(stderr, "unexpected error\n");
      exit(EXIT_FAILURE);
    }
  }

  const uint64_t *ptr = reinterpret_cast<uint64_t *>(readData.data());
  const uint64_t nr = ptr[0];
  const uint64_t timeEnabled = ptr[1];
  const uint64_t timeRunning = ptr[2];

  // std::cerr << timeEnabled << " " << timeRunning << "\n";

  std::vector<long long> ret(nr);
  for (uint64_t i = 0; i < nr; ++i) {
    ret[i] = ptr[3 + 2 * i];
  }

  return ret;
}

long EventGroup::perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                                 int cpu, int group_fd, unsigned long flags) {
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
  return ret;
}
