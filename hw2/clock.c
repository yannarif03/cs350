#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "timelib.h"

/* uint64_t get_clocks(){ */
/*   uint32_t a,d; */
/* __asm__ __volatile__ ("rdtsc": "=a" (a), "=d" (d)); */
/*   return (((uint64_t)d << 32) | a); */
/*   } */

int main(int argc, char* argv[]) {
  //convert arguments into desired data types
  (void)argc;
  long sec=atol(argv[1]);
  long nsec=atol(argv[2]);
  char method= *argv[3];
  /*depending on third argument input, use different get_elapsed
  method and store the output in x.  use x and the input times to
  compute the clockspeed in MHz. */
  if (method=='s') {
    uint64_t x=get_elapsed_sleep(sec,nsec);
    printf("WaitMethod: %s\n","SLEEP");
    printf("WaitTime: %ld %ld\n",sec,nsec);
    printf("ClocksElapsed: %lu\n", x);
    double time=(double)sec + (double)nsec/1e9;
    printf("ClockSpeed: %.2f\n", (x/(double)time)*1e-6);
  } else if (method=='b'){
    uint64_t x= get_elapsed_busywait(sec,nsec);
    printf("WaitMethod: %s\n", "BUSYWAIT");
    printf("WaitTime: %ld %ld \n", sec,nsec);
    printf("ClocksElapsed: %lu\n", x);
    double time=(double)sec + ((double)nsec)/1e9;

    printf("ClockSpeed: %.2f\n", (x/(double)time)*1e-6);
  } else {
    //in case input method is wrong. if this weren't a problem set I
    //would implement much more safeguards towards invalid inputs.
    printf("try again!\n" );
  }
  return 0;
}
