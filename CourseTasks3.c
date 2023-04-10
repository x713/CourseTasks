/*
Write 3 functions that implement the following behaviour :
Set the required bits in transmitted parameter according to the transmitted mask if the corresponded bits are cleared.
Clear the required bits in transmitted parameter according to the transmitted mask if the corresponded bits are set.
Toggle the required bits in transmitted parameter according to the transmitted mask.
Write a function that determines whether the transmitted integer parameter is even and odd.

*/

#include <stdio.h>
#include <stdint.h>

int set_bits(int param, int mask)
{
  /* logical OR 
  * p/m  0  1
  *  0   0  1 
  *  1   1  1
  */
  return param | mask;
}

int clear_bits(int param, int mask)
{
  /* logical AND NOT
  * p/m  0  1
  *  0   0  0
  *  1   1  0
  */

  return param & ~mask;
}

int toggle_bits(int param, int mask)
{
  /* logical XOR
  * p/m  0  1
  *  0   0  1
  *  1   1  0
  */

  return param ^ mask;
}

bool is_odd(int param)
{
  if ((param & 0x01) == 1)
    return true;
  else
    return false;
}

int main(int argc, char* argv[])
{

  return 0;
}
