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


struct stack_t
{
  void* prev;
  void* curr;
  void* data;
};

typedef struct stack_t* stack_t;

stack_t stack_new(void);
void stack_free(stack_t stk);
void stack_push(stack_t stk, void* x);
void* stack_pop(stack_t stk);
bool stack_empty(stack_t stk);

/*
bool stack_empty(stack_t stk);

*/

stack_t stack_new(void)
{
  stack_t stack;
  stack = (stack_t)malloc(sizeof(stack_t));

  //memset(stack, 0, sizeof(stack_t));
  if (stack != NULL)
  {
    stack->curr = NULL;
    stack->prev = NULL;
    stack->data = NULL;
  }
  return stack;
}

void stack_free(stack_t stk)
{
  while (!stack_empty)
    stack_pop(stk);

  free(stk);
  stk == NULL;
}

void stack_push(stack_t stk, void* x)
{
  if (stk != NULL)
  {
    if (stk->curr == NULL)
    {
      stk->data = x;
      stk->curr = &stk;
      stk->prev = NULL;
    }
    else
    {

      stack_t el;
      el = (stack_t)malloc(sizeof(stack_t));

      if (el != NULL)
      {
        el->curr = &el;
        el->prev = &stk->curr;
        el->data = x;

        stk->curr = el;
      }
    }
  }
}

void* stack_pop(stack_t stk)
{
  if (stk != NULL && stk->curr != NULL)
  {
    stack_t el = (stack_t)stk->curr;
    void* result = el->data;

    stk->curr = el->prev;

    free(el);

    return result;
  }
}

bool stack_empty(stack_t stk)
{
  return stk == NULL || stk->curr == NULL ? true : false;
}

const char* str = "1234567890";

int main(void)
{
  int i = 1;
  char b1 = 2;

  int* pi;
  char* pc;

  struct S {
    int v[2];

  } s, * ps;

  pi = &i;
  pc = &b1;

  ps = &s;

  printf("%p %p %p\n", pi, pc, ps);
  ++pi, ++pc, ++ps;
  printf("%p %p %p\n", pi, pc, ps);

  printf("%c\n", *((char*)str + 2));
  printf("%c\n", *((short*)str + 2));

  int a;
  int b;
  int const* ptr_a = &a;
  int* const ptr_b = &b;

  //*ptr_a = 42;
  ptr_a = &b;

  //ptr_b = 42;
  //ptr_b = &a;

  return 0;
}
