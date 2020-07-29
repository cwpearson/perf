#pragma once

#include <errno.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <vector>

#define INLINE __attribute__((always_inline))

/*
TODO: The official way of knowing if perf_event_open() support is enabled is
checking  for  the existence of the file /proc/sys/kernel/perf_event_paranoid.
*/
class EventGroup {
private:
  int fd_;
  int nr_; // number of events in the group

public:
  EventGroup();
  ~EventGroup();
  EventGroup(EventGroup &&) = delete;
  EventGroup(const EventGroup &) = delete;

  void add(unsigned int type, unsigned long long config);

  void reset();

  /* inline to avoid extra instructions
   */
  INLINE void enable() {
    ioctl(fd_, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
  }
  /* inline to avoid extra instructions
   */
  INLINE void disable() {
    ioctl(fd_, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
  }

  std::vector<long long> read();

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