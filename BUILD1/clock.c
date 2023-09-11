#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "timelib.h"
unsigned long long get_clocks() {
  unsigned int a,d;
  __asm__ __volatile__("rdtsc":"=a" (a), "=d" (d));
  return ((unsigned long long)d << 32) | a;
}

unsigned long long get_elapsed_sleep(long sec, long nsec){
  unsigned long long bef, aft;
  bef = get_clocks();
  struct timespec tim, tim2;
  tim.tv_sec = sec;
  tim.tv_nsec = nsec;
  nanosleep(&tim,&tim2);
  aft=get_clocks();
  return aft-bef;
}

unsigned long long get_elapsed_busywait(long sec, long nsec){
  unsigned long long bef, aft;
  struct timespec start,end;
  clock_gettime(CLOCK_MONOTONIC,&start);
  bef=get_clocks();
  clock_gettime(CLOCK_MONOTONIC,&end);
  long secdiff=end.tv_sec-start.tv_sec;
  long nsecdiff=end.tv_nsec-start.tv_sec;
  while (1000000000*secdiff+nsecdiff<1000000000*sec+nsec) {
    clock_gettime(CLOCK_MONOTONIC,&end);
    long secdiff=end.tv_sec-start.tv_sec;
    long nsecdiff=end.tv_nsec-start.tv_sec;
  }
  aft=get_clocks();
  return aft-bef;
}

int main(int argc, char* argv[]) {
  long sec=atol(argv[1]);
  long nsec=atol(argv[2]);
  char method= *argv[3];

  if (method=='s') {
    unsigned long long x=get_elapsed_sleep(sec,nsec);
    printf("WaitMethod: %s\n","sleep");
    printf("WaitTime: %ld %ld\n",sec,nsec);
    printf("ClocksElapsed: %lld\n", x);
    printf("ClockSpeed: %ld\n", (1000000000*sec+nsec)/x);
  } else if (method=='b'){
    unsigned long long x= get_elapsed_busywait(sec,nsec);
    printf("WaitMethod: %s\n", "busywait");
    printf("WaitTime: %ld %ld \n", sec,nsec);
    printf("ClocksElapsed: %lld\n", x);
    printf("ClockSpeed: %ld\n", (1000000000*sec+nsec)/x);
  } else {
    printf("try again!\n" );
  }
  return 0;
}
