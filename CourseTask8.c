/*
Implement Linked list and insert and delete operations for it.
Implement a function to compare the structure variables.
Implement a program to extract and print individual bytes
 from an unsigned int using the following union:
          union tagname
          {
               unsigned int a;
               unsigned char s[4];
         };

and answer the question: Will the output depend on endianness?
*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>


//////////////////
// List implementation
/////////////////

#pragma pack(push,1)

struct Node
{
  void* data;
  //  Node* prev;
  Node* next;
};

struct List
{
  Node* ls_begin = NULL;
  int ls_size = 0;
};

#pragma pack(pop)

List* create_list()
{
  List* list = (List*)malloc(sizeof(List));
  if (list == NULL) return NULL;

  list->ls_begin = NULL;
  list->ls_size = 0;

  return list;
}

bool is_empty(List* list)
{
  return list->ls_size == 0;
}


void* get_first(List* list)
{
  if (list->ls_begin != NULL)
    return list->ls_begin->data;

  return NULL;
}

void* get_at_index(List* list, int index)
{
  if (index > list->ls_size)
  {
    return NULL;
  }

  Node* curr = list->ls_begin;
  for (int i = 0; i < index; i++)
    curr = curr->next;

  return curr->data;
}

void* insert_at_start(List* list, void* data)
{
  Node* node = (Node*)malloc(sizeof(Node));
  if (node != NULL)
  {
    node->data = data;
    node->next = list->ls_begin;
    list->ls_begin = node;
    list->ls_size++;
    return node;
  }
  else
    return NULL;
}

void* remove_at_end(List* list)
{

  Node* curr = list->ls_begin;
  if (curr == NULL)
    return NULL;

  if (curr->next != NULL)
    while (curr->next->next != NULL)
      curr = curr->next;

  Node* last = curr->next;
  void* data = last->data;
  free(last);
  curr->next = NULL;
  list->ls_size--;

  return data;
}


void* remove_at_start(List* list)
{

  Node* curr = list->ls_begin;
  if (curr == NULL)
    return NULL;

  void* data = curr->data;
  list->ls_begin = curr->next;
  list->ls_size--;
  free(curr);

  return data;
}

void* remove_at_index(List* list, int index)
{
  if (index > list->ls_size)
  {
    return NULL;
  }

  if (index == 0)
    return remove_at_start(list);

  if (index == list->ls_size - 1)
    return remove_at_end(list);

  Node* curr = list->ls_begin;
  for (int i = 0; i < index - 1; i++) // (index - 1) we are going to stop Before needed index;
    curr = curr->next;

  Node* next = curr->next->next;
  Node* prev = curr;
  curr = curr->next;

  void* data = curr->data;

  prev->next = next;

  free(curr);
  list->ls_size--;

  return data;
}

struct complex {
  int x, y;
  char c;
};

void print_list(List* list)
{
  printf("List Size: %d \n", list->ls_size);
  printf("List : { ");
  for (int i = 0; i < list->ls_size; i++)
  {
    void* data = get_at_index(list, i);
    printf("%d ", *(int*)data);
  }
  printf("} \n");

}

void delete_list(List* list)
{
  while (is_empty(list))
    remove_at_start(list);
}

//////////////////
// EOF List implementation
/////////////////

// Implement a function to compare the structure variables.
bool compare(complex s1, complex s2)
{
  return s1.x == s2.x && s1.y == s2.y && s1.c == s2.c;
}

//Implement a program to extract and print individual bytes
union tagname
{
  unsigned int a;
  unsigned char s[4];
};

int extract_byte(int num, int idx)
{
  if (idx > sizeof(int))
    return 0;

  tagname tmp;
  tmp.a = num;

  return tmp.s[idx];
}


int main(void)
{
  List* list = create_list();

  int a[] = { 1,7,9,14,5,2,8,2,8,45,7,3,6,9 };
  int size = sizeof(a) / sizeof(int);

  for (int i = 0; i < size; i++)
  {
    insert_at_start(list, &a[i]);
  }

  print_list(list);

  remove_at_start(list);
  remove_at_end(list);
  remove_at_index(list, 7);

  size -= 3;

  print_list(list);
  remove_at_end(list);
  print_list(list);

  delete_list(list);


  int num = 0x0A0B0C0D;

  int byte = 0;

  // on my machine - big endian "0D 0C 0B 0A" - 13 12 11 10
  printf("%d ", extract_byte(num, 0));
  printf("%d ", extract_byte(num, 1));
  printf("%d ", extract_byte(num, 2));
  printf("%d\n", extract_byte(num, 3));

  return 0;
}


