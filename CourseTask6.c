//Write a function for sorting an array of integer.
// Like the library function qsort your function should take a pointer to the function for comparing of two elements.
// 
//Write a simple implementation for a stack structure.Use an abstract data type and an opaque pointer.


//  void qsort ( void * base , size_t nmemb , size_t size ,
//  int (*compar)(const void*, const void*));

#include <stdio.h>
#include <stdlib.h>
//#include <vcruntime_string.h>
#include <cstring>


struct stack_s
{
  void* next;
  void* data;
};

typedef struct stack_s* stack_p;


stack_p stack_new(void);
bool stack_empty(stack_p stk);
void stack_push(stack_p& stk, void* x);
void* stack_pop(stack_p& stk);
void stack_free(stack_p& stk);

stack_p stack_new(void)
{
  stack_p el = (stack_p)malloc(sizeof(stack_s));
  if (el != NULL)
  {
    el->data = NULL;
    el->next = NULL;
  }
  return el;
}

bool stack_empty(stack_p stk)
{
  return stk == NULL ? true : false;
}

void stack_push(stack_p& stk, void* x)
{
  stack_p el = (stack_p)malloc(sizeof(stack_s));
  if (el != NULL)
  {
    el->data = x;
    el->next = stk;

    stk = el;
  }
}

void* stack_pop(stack_p& stk)
{
  if (!stack_empty(stk))
  {
    stack_p el = stk;
    void* result = el->data;
    stk = (stack_p)el->next;
    free(el);

    return result;
  }
}

void stack_free(stack_p& stk)
{
  while (!stack_empty(stk))
    stack_pop(stk);
  stk = NULL;
}

int compare_int(int a, int b)
{
  return a > b;
}

void sort_array(int * source_arr, int count, int compare(int, int))
{

  for (int i =0; i< count; i++)
    for (int j=i; j < count; j++)
      if (compare(source_arr[i], source_arr[j]))
      {
        int tmp = source_arr[i];
        source_arr[i] = source_arr[j];
        source_arr[j] = tmp;
      }
}

int main(void)
{

  stack_p stack = stack_new();

  int a1 = 80;
  int a2 = 50;
  int a3 = 70;
  int a4 = 9;

  stack_push(stack, &a1);
  stack_push(stack, &a2);
  stack_push(stack, &a3);
  stack_push(stack, &a4);
  stack_push(stack, &a1);

  stack_free(stack);

  stack_push(stack, &a2);
  stack_push(stack, &a3);
  stack_push(stack, &a4);

  while (!stack_empty(stack))
    printf("%d\n", *(int*)stack_pop(stack));

  if (!stack_empty(stack))
    stack_free(stack);

  int arr[8] = { 3, 7, 1, 8, 4, 6, 2, 5 };

#define print_arr do { \
      for (int i = 0; i < 8; i++) \
        printf(" %d ", arr[i]); \
        printf("\n"); \
  }while(0);

  print_arr;

  sort_array(arr, 8, compare_int);

  print_arr;

  return 0;
}
