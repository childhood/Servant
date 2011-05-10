#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int
 timeval_subtract (result, x, y)
      struct timeval *result, *x, *y;
 {
   /* Perform the carry for the later subtraction by updating y. */
   if (x->tv_usec < y->tv_usec) {
     int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
     y->tv_usec -= 1000000 * nsec;
     y->tv_sec += nsec;
   }
   if (x->tv_usec - y->tv_usec > 1000000) {
     int nsec = (x->tv_usec - y->tv_usec) / 1000000;
     y->tv_usec += 1000000 * nsec;
     y->tv_sec -= nsec;
   }

   /* Compute the time remaining to wait.
      tv_usec is certainly positive. */
   result->tv_sec = x->tv_sec - y->tv_sec;
   result->tv_usec = x->tv_usec - y->tv_usec;

   /* Return 1 if result is negative. */
   return y->tv_sec - x->tv_sec;
 }


int main() {  
//...
struct timeval tv1, tv2, diff;

// get the first time:
gettimeofday(&tv1, NULL);

sleep(2);
// ...

// get the second time:
gettimeofday(&tv2, NULL);

// get the difference:

int result = timeval_subtract(&diff, &tv2, &tv1);

printf("Difference: %d\n", diff.tv_sec);
return 0;
}
