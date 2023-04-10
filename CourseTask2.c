
#include <stdio.h>

int main(int argc, char* argv[])
{
  // Update the program at the slide 6 in such a way to use variables instead of literals in the printf calls.
  int value_int = 5;
  long value_long = 5;
  long long value_long_long = 5;
  double value_double = 5;
  float value_float = 5;
  long double value_long_double = 5;
  char value_char = 'x';

  printf(" value %zu\n", sizeof(value_int)); // 4
  printf(" value %zu\n", sizeof(value_long)); // 4
  printf(" value %zu\n", sizeof(value_long_long)); // 8
  printf(" value %zu\n", sizeof(value_double)); // 8
  printf(" value %zu\n", sizeof(value_float)); // 4
  printf(" value %zu\n", sizeof(value_long_double)); // 8
  printf(" value %zu\n", sizeof(value_char)); // 1

  //Write a program with a declaration of constants for a length = 10 and a width = 15 using 3 ways and display them using printf call.

#define length_defined 10
#define width_defined 15

  const char length_const = 10;
  const char width_const = 15;
  typedef enum {
    length_enum = 10,
    width_enum = 15
  } rect_enum;

  printf("length_defined = %d\n", length_defined);
  printf("width_defined = %d\n", width_defined);

  printf("length_const = %d\n", length_const);
  printf("width_const = %d\n", width_const);

  printf("length_enum = %d\n", length_enum);
  printf("width_enum = %d\n", width_enum);

}
