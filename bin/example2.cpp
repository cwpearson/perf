#include "event_group.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  EventGroup group;
  group.add(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
  group.add(PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
  group.add(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES);
  group.add(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES);
  // group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_L1D | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16));
  // group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_L1I | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16));
  // group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_LL | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16));
  // group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_DTLB | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16));
  // group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_ITLB | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16));
  // group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_BPU | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16));
  // group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_NODE | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16));


  group.reset();
  group.enable();

  printf("Measuring cache reference count for this printf\n");

  group.disable();

  std::vector<long long> vals = group.read();

  for (auto &e : vals) {
    std::cerr << e << "\n";
  }

}