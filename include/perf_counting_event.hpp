#pragma once

#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define INLINE __attribute__((always_inline))

/*
TODO: The official way of knowing if perf_event_open() support is enabled is
checking  for  the existence of the file /proc/sys/kernel/perf_event_paranoid.
*/
class PerfCountingEvent {
private:
  int fd_;

public:
  PerfCountingEvent(unsigned long long config);
  ~PerfCountingEvent();
  PerfCountingEvent(PerfCountingEvent &&) = delete;
  PerfCountingEvent(const PerfCountingEvent &) = delete;

  void reset();

  /* inline to avoid extra instructions
  */
  INLINE void enable() { ioctl(fd_, PERF_EVENT_IOC_ENABLE, 0); }
  /* inline to avoid extra instructions
  */  
  INLINE void disable() { ioctl(fd_, PERF_EVENT_IOC_DISABLE, 0); }

  long long read();

  /*

    pid = 0: current thread
    pid > 0: specific proc is counted
    pid = -1: all procs counted

    cpu >=0: specific CPU is counted
    cpu = -1: all CPUs counted
  */
  static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                              int cpu, int group_fd, unsigned long flags);
};