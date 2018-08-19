#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define NLOOP_FOR_ESTIMATION 1000000000UL
#define NSECS_PER_MSEC 1000000UL
#define NSECS_PER_SEC 1000000000UL

static inline long diff_nsec(struct timespec before, struct timespec after)
{
  return ((after.tv_sec * NSECS_PER_SEC + after.tv_nsec)
         - (before.tv_sec * NSECS_PER_SEC + before.tv_nsec));
}

static unsigned long loops_per_msec()
{
  unsigned long i;
  struct timespec before, after;

  clock_gettime(CLOCK_MONOTONIC, &before);

  for (i = 0; i < NLOOP_FOR_ESTIMATION; i++)
    ;

  clock_gettime(CLOCK_MONOTONIC, &after);

  int ret;
  return NLOOP_FOR_ESTIMATION * NSECS_PER_MSEC / diff_nsec(before, after);
}

int main(int argc, char *argv[])
{
  int ret = EXIT_FAILURE;

  if (argc < 3) {
    fprintf(stderr, "usage: %s <total[ms]> <resolution[ms]>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int nproc = 2;
  int total = atoi(argv[1]);
  int resol = atoi(argv[2]);

  if (total < 1) {
    fprintf(stderr, "<total>(%d) should be >= 1\n", total);
    exit(EXIT_FAILURE);
  }

  if (resol < 1) {
    fprintf(stderr, "<resol>(%d) should be >= 1\n", resol);
    exit(EXIT_FAILURE);
  }

  if (total % resol) {
    fprintf(stderr, "<total>(%d) should be multiple of <resolution>(%d)\n", total, resol);
    exit(EXIT_FAILURE);
  }

  int nrecord = total / resol;

  struct timespec *logbuf = malloc(nrecord * sizeof(struct timespec));
  if (!logbuf) err((EXIT_FAILURE), "malloc(logbuf) failed");
  unsigned long nloop_per_resol = loops_per_msec() * resol;
}
