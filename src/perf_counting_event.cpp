#include "perf_counting_event.hpp"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <asm/unistd.h>
#include <unistd.h>

PerfCountingEvent::PerfCountingEvent(unsigned long long config) : fd_(-1) {
  struct perf_event_attr pe;
  std::memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = config;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  pid_t pid = 0;     // current thread
  int cpu = -1;      // all cpus
  int group_fd = -1; // event group leader
  unsigned long flags = 0;
  fd_ = perf_event_open(&pe, pid, cpu, group_fd, flags);


  if (fd_ == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
    exit(EXIT_FAILURE);
  }
}

PerfCountingEvent::~PerfCountingEvent() {
    close(fd_);
}

void PerfCountingEvent::reset() { ioctl(fd_, PERF_EVENT_IOC_RESET, 0); }

long long PerfCountingEvent::read() {
  long long count;
  ::read(fd_, &count, sizeof(count));
  return count;
}

long PerfCountingEvent::perf_event_open(struct perf_event_attr *hw_event,
                                        pid_t pid, int cpu, int group_fd,
                                        unsigned long flags) {
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
  return ret;
}
