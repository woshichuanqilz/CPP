/* time example */
#include <windows.h>
#include <stdio.h>      /* printf */
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

int main ()
{
  time_t timer;
  time_t timer2;
  double seconds;

  time(&timer);  /* get current time; same as: timer = time(NULL)  */
  Sleep (2000);

  time(&timer2);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,timer2);

  printf ("%.f seconds since January 1, 2000 in the current timezone", seconds);

  return 0;
}
