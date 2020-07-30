#include "event_group.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  EventGroup group;

  // LLC reads/writes
  // group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_LL |
  // (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS <<
  // 16)); group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_LL |
  // (PERF_COUNT_HW_CACHE_OP_WRITE << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS <<
  // 16));
    group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_L1D |
                                      (PERF_COUNT_HW_CACHE_OP_WRITE << 8) |
                                      (PERF_COUNT_HW_CACHE_RESULT_ACCESS <<
                                      16));
  //   group.add(PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_NODE |
  //                                     (PERF_COUNT_HW_CACHE_OP_READ << 8) |
  //                                     (PERF_COUNT_HW_CACHE_RESULT_ACCESS <<
  //                                     16));

  /*
  llc_references.pcie_read                          
       [PCIe read current. Derived from unc_c_tor_inserts.opcode. Unit: uncore_cbox]
        uncore_cbox_4/umask=0x1,event=0x35,filter_opc=0x19e/ 
  llc_references.pcie_write                         
       [PCIe write references (full cache line). Derived from unc_c_tor_inserts.opcode. Unit: uncore_cbox]
        uncore_cbox_4/umask=0x1,event=0x35,filter_opc=0x1c8,filter_tid=0x3e/ 
*/
//   group.add(PERF_TYPE_RAW, 0x35, 0x19e);
//   group.add(PERF_TYPE_RAW, 0xB0,0x08);
  group.add(PERF_TYPE_RAW, 0x51);
//   group.add(PERF_TYPE_RAW, 0x1c8, 0x35);

  cudaFree(0);
  // TODO: cuda sets errno sometimes?
  errno = 0;

  char *dst, *src;
  cudaMalloc(&dst, 1 << 20);
  cudaHostAlloc(&src, 1 << 20, cudaHostAllocDefault);

  //   group.reset();
  //   group.enable();

  for (int bytes = 1; bytes <= (1 << 20); bytes *= 2) {

    std::memset(src, bytes, 1);

    group.reset();
    group.enable();

    cudaMemcpy(dst, src, bytes, cudaMemcpyDefault);
    for (int i = 0; i < bytes; ++i) {
        src[i] = 0;
    }
    // std::memset(dst, 1, bytes);
    asm volatile("" ::"r,m"(src) : "memory");

    group.disable();

    std::cerr << bytes << ": ";

    std::vector<long long> vals = group.read();
    for (auto &e : vals) {
      std::cerr << e << " ";
    }
    std::cerr << "\n";
  }

  //   group.disable();

  //   std::vector<long long> vals = group.read();
  //   for (auto &e : vals) {
  //     std::cerr << e << "\n";
  //   }

//   cudaFree(src);
//   cudaFreeHost(dst);
}