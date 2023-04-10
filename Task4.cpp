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


int main(int argc, char* argv[])
{
  printf(" %d \n", sum(6, 1, 2, 3, 4, 5, 6));

  printf(" Compiled on ’%s %s ’\n",
    __DATE__, __TIME__);
  printf("We are in ’%s’ (%s:%u)\n",
    __func__, __FILE__, __LINE__);

# ifdef __cplusplus
  printf("g++ in use\n.");
# else
  printf(" gcc in use , STD version is %lu\n.",
    __STDC_VERSION__);
# endif


  //Variadic macros (since C99)
#define eprintf(...) fprintf( stderr , __VA_ARGS__ )

  eprintf("%s:%d: ", __FILE__, __LINE__);
  //== > fprintf ( stderr , "%s:%d: ", __FILE__ , __LINE__ )



#define WARN_IF(EXP) \
 do { if ( EXP ) \
 fprintf ( stderr , " Warning : " # EXP "\n"); \
 } while (0)

  WARN_IF(argc == 1);

  /*
  void quit_command();
  void help_command();

  struct command
  {
    const char* name;
    void (*function) (void);
  };

  struct command commands1[] =
  {
    { "quit", quit_command },
    { "help", help_command },
  };

# define COMMAND(NAME) { # NAME, NAME ## _command }

  struct command commands2[] =
  {
    COMMAND(quit),
    COMMAND(help),
  };
  */
  // I is not defined
#define SQR(I) (I * I)
  int x1 = 1;
  printf("%d", SQR(x1 + 2)); // x + 2 * x + 2 = x + (2 * x) + 2

// 19: only one line after (if) will be executed
#define SWAP(x , y) \
tmp = x ; \
x = y ; \
y = tmp ; \

  int x=3, y=2, z=0, tmp;

  if (z == 0)
    SWAP(x, y);


  return 0;

}
