/*
Write function that:
Take two strings as arguments. If the first exists in the second as a substring, return a pointer to
the first occurrence, otherwise zero.
Write function that determine endianness at run time.
Write function for reversing the byte order (swap endianness).
Write function that find the largest and smallest word in a string.
Function takes three strings as arguments and has the following prototype:
void find_max_min_str(const char * input_str, char * max_str, char * min_str).
Example:
If input_str is "It is a string with smallest and largest word" then max_str is "smallest" and min_str is "a".*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

char* locate_string(const char* loc_str, const char* source_str)
{
  int i = 0;
  int j = 0;

  char* k = (char*)source_str;

  while (*k != 0)
  {
    bool found = false;
    if (loc_str[0] == k[0])
    {
      found = true;
      int i = 0;
      while (*(k + i) != 0 && loc_str[i] != 0)
      {
        if (*(k + i) != loc_str[i])
        {
          found = false;
          break;
        }
        i++;
      }
      if (found)
        return k;
    }
    k++;
  }

  return 0;
}

bool is_big_endian()
{
  union int32_endian
  {
    unsigned int a;
    unsigned char s[4];
  } i = { 0x0A0B0C0D };

  return i.s[0] == 0x0A;
}

void swap(unsigned char& x, unsigned char& y)
{
  x ^= y;
  y ^= x;
  x ^= y;
};

int toggle_endian(unsigned int num)
{
  union int32_endian
  {
    unsigned int a;
    unsigned char s[4];
  } i = { num };

  swap(i.s[0], i.s[3]);
  swap(i.s[1], i.s[2]);

  return i.a;
}

// If input_str is "It is a string with smallest and largest word" then max_str is "smallest" and min_str is "a".*/
void find_max_min_str(const char* input_str, char* &max_str, char* &min_str)
{
  if (!(max_str == NULL && min_str == NULL && input_str != NULL))
    return;

  int i = 0;

  const char* small_word = NULL;
  const char* big_word = NULL;
  int small_word_lenght = (int)strlen(input_str);
  int big_word_lenght = 0;
  bool word_search = false;

  int tmp_length = 0;
  const char* tmp_word = NULL;

  while (input_str[i] != 0)
  {

    if (
      input_str[i+1] != 0 &&
      input_str[i] != ' ' &&
      input_str[i] != '.' &&
      input_str[i] != ',' &&
      input_str[i] != '!' &&
      input_str[i] != ';' )
    {
      if (!word_search)
      {
        word_search = true;
        tmp_word = &input_str[i];
      }
      tmp_length++;
      if (tmp_length > big_word_lenght)
      {
        big_word_lenght = tmp_length;
        big_word = tmp_word;
      }
    }
    else
    {

      if (tmp_length < small_word_lenght)
      {
        small_word_lenght = tmp_length;
        small_word = tmp_word;
      }

      word_search = false;
      tmp_length = 0;
      tmp_word = NULL;
    }



    i++;
  }

  max_str = (char*)big_word;
  min_str = (char*)small_word;

}

int main(void)
{
  char strl[] = "123";
  char str[] = "12 456 467 23 56 123 677856";

  char* ind = locate_string(strl, str);

  for (int i = 0; i < strlen(strl); i++)
    printf("%c", ind[i]);

  printf("\n");

  int test_nbr = 0x0A0B0C0D;
  printf("%08X\n", test_nbr);
  printf("%08X\n", toggle_endian(test_nbr));


  // If input_str is "It is a string with smallest and largest word" then max_str is "smallest" and min_str is "a".
  char* min = NULL;
  char* max = NULL;
  find_max_min_str("It is a string with smallest and largest word", max, min);

  printf("\nMax word :");
  for (int i = 0; max[i] != ' '; i++)
  {
    printf("%c", max[i]);
  }
  printf("\nMin word : ");
  for (int i = 0; min[i] != ' '; i++)
  {
    printf("%c", min[i]);
  }

  printf("\n");


  return 0;
}


