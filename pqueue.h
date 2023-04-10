#pragma once
//#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// List Node
struct ListNode
{
  void* data;            // Data stored in the list
  struct ListNode* next; // pointer to next stored element
};
typedef struct ListNode ListNode;

// Callbacks
typedef bool(*fp_cmp_data)(void*, void*); // Call back function to compare stored elements - 'true' if (first < second).
typedef void(*fp_delete_data)(void*); // Call back function for deleting stored data, if needed.

// Priority queue structure
//  - abstract comparing function 
//  - call back for data being deleted from queue
struct PQueue
{
  ListNode* begin;
  fp_delete_data on_delete; // callback to handle deleting of stored data_ptr
  fp_cmp_data compare_less;
};
typedef struct PQueue PQueue;


PQueue* create_pqueue(fp_cmp_data cmp_f, fp_delete_data delete_data_f);
void delete_pqueue(PQueue** pqueue);
void* pop_pqueue(PQueue** pqueue); // returns (void*)(data) stored
void push_pqueue(PQueue** pqueue, void* data); // store (void*)(data)
bool is_empty(PQueue* pqueue);


