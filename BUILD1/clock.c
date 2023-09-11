#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "timelib.h"

int main(int argc, char* argv[]) {
  long sec=atol(argv[1]);
  long nsec=atol(argv[2]);
  char method= *argv[3];

  if (method=='s') {
    uint64_t x=get_elapsed_sleep(sec,nsec);
    printf("WaitMethod: %s\n","sleep");
    printf("WaitTime: %ld %ld\n",sec,nsec);
    printf("ClocksElapsed: %ld\n", x);
    printf("ClockSpeed: %ld\n", (1000000000*sec+nsec)/x);
  } else if (method=='b'){
    uint64_t x= get_elapsed_busywait(sec,nsec);
    printf("WaitMethod: %s\n", "busywait");
    printf("WaitTime: %ld %ld \n", sec,nsec);
    printf("ClocksElapsed: %ld\n", x);
    printf("ClockSpeed: %ld\n", (1000000000*sec+nsec)/x);
  } else {
    printf("try again!\n" );
  }
  return 0;
}
uint64_t get_clocks(){
  unsigned int a,d;
__asm__ __volatile__ (
		      "rdtsc"
		      : "=a" (a), "=d" (d)
		      );
  return (((uint64_t)d << 32) | a);
}
