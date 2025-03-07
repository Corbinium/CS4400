#include <stdio.h>
#include <time.h>
#include <unistd.h>

// Helper to print the hour:min:sec
void print_time();


// continually read one byte at a time until EOF
// print its arrival time, followed by the byte
int main()
{
  char readbyte;
  while (1) {
    int readnum = read(0, &readbyte, 1);
    if (readnum == 0) {
      break;
    } else {
      print_time();
      printf(": %d\n", readbyte);
    }
  }

  printf("reader received EOF\n");

}




void print_time()
{
  time_t t = time(0);
  struct tm* tm;
  tm = localtime(&t);

  printf("%d:%d:%d", tm->tm_hour, tm->tm_min, tm->tm_sec);
}
