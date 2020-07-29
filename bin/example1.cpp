#include "perf_counting_event.hpp"

#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  PerfCountingEvent counter(PERF_COUNT_HW_INSTRUCTIONS);

  counter.reset();
  counter.enable();

  printf("Measuring instruction count for this printf\n");

  counter.disable();

  printf("USed %lld instructions\n", counter.read());

}