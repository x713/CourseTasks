

#include <stdio.h>
#include <stdint.h>

void func(int a, int* b) {
  printf("%d %d\n", a, *b);
};


int main_t3(int argc, char* argv[])
{
  int a = 17 % 3;
  int b = 17 % -3;
  int c = -17 % 3;
  int d = -17 % -3; // right sign is not counted
  // The meaning of the remainder operator for negative operands was
  //  implementation - defined.

  printf("%d\n", a);
  printf("%d\n", b);
  printf("%d\n", c);
  printf("%d\n", d);

  /*
  signed char x = 8;
  signed char y = -8;
  unsigned char z = 8;

  printf("%d %d %d\n", x << 1, x << -1, x << 20); // OK UB UB
  printf("%d %d %d\n", y << 1, y << -1, y << 20); // UB UB UB
  printf("%d %d %d\n", z << 1, z << -1, y << 20); // OK UB UB
  */

  /*
  signed char x = 8;
  signed char y = -128;
  unsigned char z = 8;

  printf("%d %d %d\n", x >> 1, x >> -1, x >> 25); // OK UB UB
  printf("%d %d %d\n", y >> 1, y >> -1, y >> 25); // ID UB UB
  printf("%d %d %d\n", z >> 1, z >> -1, z >> 25); // OK UB UB
  */

  int a1 = 7;
  int b1 = 5;
  int c1 = 1;

  //printf("%s\n", (a1 > b1 > c1) ? " true " : " false ");

  float pi;
  pi = 3, 14;
  printf("%f\n", pi);

  int i, j;
  i = j = 0;

  //(i, j) = 3;
  printf("%d %d\n", i, j);

  //printf("%d\n", a+++++b); /* Why it fails to compile */

  printf("%d\n", a++ + ++b); /* and this one is compilable . */

  printf("%d\n", a++ + ++b); /* This one is also OK , */

  //printf("%d\n", a++ +++b); /* but this one is not . */

  printf("%d\n", a + ++b);

  float fa = 4321;
  float fb = 1234;
  float* pa = &fa;
  float* pb = &fb;

  float fc = *pa / *pb;
  printf("% f\n ", fc);


  {
    int i = 0;
    int x[20] = { 0 };

    i = i++;
    i = ++i;
    x[i] = i++;
    x[i++] = i;

    x[0] = i;
  };


  int x = 0;

  func(x++, &x);

  uint8_t port = 0x5a;
  uint8_t result_8 = ~port >> 4;

  printf("0x%02x\n", result_8);

  return 0;
}
