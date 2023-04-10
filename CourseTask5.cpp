//Write a program, which can take an indefinite amount of numbers and print their sum.
//Write a program, which takes a string as an argument and prints its reversed version into the console(use recursion to achieve that)

#include <stdio.h>
#include <stdarg.h>

int sum(int count, ...)
{
  long r = 0;

  va_list int_list;
  va_start(int_list, count);

  for (int i = 0; i < count; i++)
    r += va_arg(int_list, int);

  va_end(int_list);

  return r;
}


void recursive_revert(const char* str)
{
  if (str[0] != 0)
  {
    recursive_revert(str+1);
    printf("%c", str[0]);
  }
}

int main(int argc, char* argv[])
{
  printf("summ: %d \n", sum(6, 1, 2, 3, 4, 5, 6));

  recursive_revert("ghbdtn!");

  return 0;
}
