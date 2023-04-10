/*
  Write a function to count a total number of duplicate elements in an array.
    Write a function to find the maximum and minimum element in an array.
    Write a function to merge one sorted array into another sorted array.
    Expected Output :
  The given Array 1 is : 1 3 5 7
    The given Array 2 is : 2 4 6 8
    After merged the new Array 3 is : 1 2 3 4 5 6 7 8
*/
#include <stdio.h>
#include <stdlib.h>
#include <cstring>


int get_duplicate_num(const int arr[], int a_size)
{
  int count = 0;

  for (int i = 0; i < a_size; i++)
    for (int j = i + 1; j < a_size; j++)
      if (arr[i] == arr[j])
        count += 1;

  return count;
}

int* merge_arrays(const int source[], const int dest[], int source_size, int dest_size)
{
  int i = 0;
  int j = 0;
  int k = 0;

  int new_size = source_size + dest_size;

  int* new_arr = (int*)malloc(new_size * sizeof(int));

  if (new_arr == NULL)
    return NULL;

  while (k < new_size)
  {
    if (i < source_size && j < dest_size)
      new_arr[k] = source[i] < dest[j] ? source[i++] : dest[j++];
    else
      new_arr[k] = i < source_size ? source[i++] : dest[j++];
    k++;
  }
  return new_arr;
}


int main(void)
{
  int a[] = { 1, 2, 3, 2, 3, 1, 7, 1, 2, 3, 7 };
  int size = sizeof(a) / sizeof(a[0]);

  printf("%d\n", get_duplicate_num(a, size));

  int a1[] = { 1, 2, 3, 5, 5, 7, 9, 11, 14 };
  int a2[] = { 2, 4, 6, 8, 10 ,20 };

  int size_a1 = sizeof(a1) / sizeof(a1[0]);
  int size_a2 = sizeof(a2) / sizeof(a2[0]);

  int* m = merge_arrays(a1, a2, size_a1, size_a2);

  for (int i = 0; i < size_a1 + size_a2; i++)
    printf("%d ", m[i]);
  printf("\n");

  free(m); m = NULL;
}
