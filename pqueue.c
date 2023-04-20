#include "pqueue.h"

#include <stdlib.h>


PQueue* create_pqueue(fp_cmp_data cmp_f, fp_delete_data delete_data_f)
{
  if (cmp_f == NULL)
    return NULL;

  PQueue* queue = (PQueue*)malloc(sizeof(PQueue));

  if (queue == NULL)
    return NULL;

  queue->begin = NULL;
  queue->on_delete = delete_data_f;
  queue->compare_less = cmp_f;

  return queue;
}

void delete_pqueue(PQueue** pqueue)
{
  if ((*pqueue) != NULL)
  {
    if ((*pqueue)->begin != NULL)
    {
      ListNode* curr = (*pqueue)->begin;
      ListNode* next = NULL;
      do
      {
        next = curr->next;

        if ((*pqueue)->on_delete != NULL)
          (*pqueue)->on_delete(curr->data);

        free(curr);

        curr = next;

      } while (curr != NULL);
    }
    free(*pqueue);
    *pqueue = NULL;
  }
}

void* pop_pqueue(PQueue* pqueue)
{
  if (pqueue != NULL)
  {
    if (pqueue->begin != NULL)
    {
      ListNode* tmp = pqueue->begin;

      void* data = tmp->data;

      pqueue->begin = tmp->next;
      free(tmp);

      return data;
    }
  }

  return NULL;
}

void push_pqueue(PQueue* pqueue, void* data)
{
  if (pqueue == NULL)
    return;

  if (pqueue->compare_less == NULL)
    return;

  ListNode* node = (ListNode*)malloc(sizeof(ListNode));

  if (node == NULL)
    return;

  node->data = data;

  if (pqueue->begin == NULL)
  {
    node->next = NULL;
    pqueue->begin = node;

    return;
  }
  else
  {
    ListNode* curr = pqueue->begin;

    bool is_small_data = pqueue->compare_less(data, curr->data);
    if (is_small_data)
    {
      node->next = curr;
      pqueue->begin = node;
      return;
    }

    while (curr->next != NULL)
    {
      is_small_data = pqueue->compare_less(data, curr->next->data);

      if (is_small_data)
      {
        break;
      }

      curr = curr->next;
    }

    if (is_small_data)
    {
      node->next = curr->next;
      curr->next = node;
      return;
    }
    else
    {
      node->next = NULL;
      curr->next = node;
      return;
    }
  }
}

bool is_empty(PQueue* pqueue)
{
  return (pqueue == NULL) || (pqueue->begin == NULL);
}
