// #include "perfmon/pfmlib.h"
#include "perfmon/pfmlib_perf_event.h"

// #include <linux/perf_event.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>

void list_pmu_events(pfm_pmu_t pmu)
{
   pfm_event_info_t info;
   pfm_pmu_info_t pinfo;
   int i, ret;

   memset(&info, 0, sizeof(info));
   memset(&pinfo, 0, sizeof(pinfo));

   info.size = sizeof(info);
   pinfo.size = sizeof(pinfo);

   pfm_os_t os = PFM_OS_NONE;

   ret = pfm_get_pmu_info(pmu, &pinfo);
   if (ret != PFM_SUCCESS) {
       fprintf(stderr, "cannot get pmu info %s\n", pfm_strerror(ret));
       return;
   }

   for (i = pinfo.first_event; i != -1; i = pfm_get_event_next(i)) {
      ret = pfm_get_event_info(i, os, &info);
      if (ret != PFM_SUCCESS)
        exit(1); //, "cannot get event info");

        printf("Event: %s::%s\n",
               pinfo.name, info.name);

  struct perf_event_attr pe;
  std::memset(&pe, 0, sizeof(struct perf_event_attr));

    pfm_perf_encode_arg_t pea;
    std::memset(&pea, 0, sizeof(pea));
    pea.attr = &pe;
    pea.size = sizeof(pe);

    ret = pfm_get_os_event_encoding("RETIRED_INSTRUCTIONS", PFM_PLM3, PFM_OS_PERF_EVENT,  &pea);
    if (ret != PFM_SUCCESS) {
        fprintf(stderr, "cannot get encoding %s\n", pfm_strerror(ret));
        exit(1); //, "cannot get event info");

    }
    printf("config %llu\n", pe.config);

  }
}

int main(void) {

    pfm_initialize();

    int pmu;
    pfm_for_all_pmus(pmu)
        list_pmu_events(static_cast<pfm_pmu_t>(pmu));
}