#include <stdio.h>
#include <limits.h>
//#include <stdbool.h>

#include <stdlib.h>
//#include <unistd.h>
#include <errno.h>


#define SIZE (256 * 1024 * 1024)

char A[SIZE];

int ping(unsigned char a, unsigned char b,
  unsigned char c, unsigned char d)
{
  // do all stuff
  return 0;
}

extern char etext, edata, end;

int main_t2(int argc, char* argv[])
{

  printf(" value %zu\n", sizeof(5)); // 4
  printf(" value %zu\n", sizeof(5L)); // 4
  printf(" value %zu\n", sizeof(5LL)); // 8
  printf(" value %zu\n", sizeof(5.0)); // 8
  printf(" value %zu\n", sizeof(5.0F)); // 4
  printf(" value %zu\n", sizeof(5.0L)); // 8
  printf(" value %zu\n", sizeof('x')); // 1



  printf("%d\n", CHAR_BIT);

  double x;
  for (x = 100000001.0; x <= 100000010.0; x++)
  {
    printf("%f\n", x);
  }

  // void * ptr = malloc (16); // 16 bytes in the heap , ptr in stack
  ping(192, 168, 121, 221);
  ping(192, 168, 121, 121);
  ping(192, 168, 121, 021); // 021 base8 - base10 (17)


  for (int i = 0; i < SIZE; i++)
  {
    A[i] = 0xCC;
  }

  printf(" First address past \n");
  //  printf(" program text ( etext ): %20 p\n", &etext);
  //  printf(" initialized data ( edata ): %20 p\n", &edata);
  //  printf(" uninitialized data ( end ): %20 p\n", &end);
  //  printf(" Program break ( brk ): %20p\n", sbrk(0));
  printf(" Address of argc : %20p\n", &argc);
  printf(" Address of errno : %20p\n", &errno);
  printf(" Address of printf : %20p\n", printf);
  printf(" Adddress of malloced memory : %20p\n", malloc(16));

}
