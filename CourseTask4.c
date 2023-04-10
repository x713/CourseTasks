//Write a program to calculate power using while & for loop
//Write a program to print the number days in the months using switch statement.
//Write a program to check uppercase or lowercase alphabets.

#include <stdio.h>

int main(int argc, char* argv[])
{

  int iter_nbr = 7;
  int power = 0;
  int power_amount = 13;

  for (int i = 0; i < iter_nbr; i++)
    power += power_amount;

  printf("Power was increased after %d iterations and is equal to : %d\n", iter_nbr, power);

  int i = 0;
  while (i++ < iter_nbr)
  {
    power -= power_amount;
  }
  printf("Power was decreased after %d iterations and is equal to : %d\n", iter_nbr, power);

  int month_no = 6;
  int days_nbr = 0;

  switch (month_no)
  {
  case 1: days_nbr = 31; break;
  case 2: days_nbr = 28; break;//29
  case 3: days_nbr = 31; break;
  case 4: days_nbr = 30; break;
  case 5: days_nbr = 31; break;
  case 6: days_nbr = 30; break;
  case 7: days_nbr = 31; break;
  case 8: days_nbr = 31; break;
  case 9: days_nbr = 30; break;
  case 10: days_nbr = 31; break;
  case 11: days_nbr = 30; break;
  case 12: days_nbr = 31; break;
  }

  printf("Month nr %d has %d days in it.\n", month_no, days_nbr);

  bool letter_not_passed = true;
  do {
    printf("Please enter one letter :");
    char c;
    //error C4996 : 'scanf' : This function or variable may be unsafe.Consider using scanf_s instead.
    //scanf("%c", &c);
    scanf_s(" %c", &c);

    if (c >= 'A' && c <= 'Z')
    {
      printf("It's an UPPERCASE letter!\n");
      letter_not_passed = false;
    }
    if (c >= 'a' && c <= 'z')
    {
      printf("It's an lowercase letter!\n");
      break;
    }

  } while (letter_not_passed);


}
