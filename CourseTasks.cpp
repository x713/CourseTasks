#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "pqueue.c"

void     print_help()
{
  printf("Compressing file with Huffman codes\n");
  printf("Using:\n");
  printf("   -c filename\n");
  printf("     to compress file\n");
  printf("   -d filename\n");
  printf("     to decompress file\n\n");
}


// V2

// STRUCTURES
struct CharFreqNode {
  unsigned long freq = 0;
  unsigned char ch = 0;
  CharFreqNode* left_node = NULL;
  CharFreqNode* right_node = NULL;
};



void print_charfreq_queue(PQueue* pqueue)
{
  if (is_empty(pqueue))
    return;

  ListNode* curr = pqueue->begin;
  do
  {
    printf("%d%c ", ((CharFreqNode*)curr->data)->freq, ((CharFreqNode*)curr->data)->ch);
    curr = curr->next;
  } while (curr != NULL);

  printf("\n");
}


CharFreqNode* make_Huffman_tree(PQueue* queue)
{
  CharFreqNode* node = NULL;

  while (!is_empty(queue))
  {

    CharFreqNode* first = (CharFreqNode*)pop_pqueue(queue);

    if (is_empty(queue))
      return first;

    CharFreqNode* second = (CharFreqNode*)pop_pqueue(queue);

    CharFreqNode* node = (CharFreqNode*)malloc(sizeof(CharFreqNode));
    if (node == NULL)
      return NULL;

    node->freq = first->freq + second->freq;

    node->left_node = first;
    node->right_node = second;

    push_pqueue(queue, node);
  }

  return node;
}

// Tests
bool cmp_freq(void* d1, void* d2)
{
  return ((CharFreqNode*)d1)->freq < ((CharFreqNode*)d2)->freq;
}


#ifndef pr
#define pr print_charfreq_queue(queue)
#endif

void test_prio_queue()
{
  PQueue* queue = create_pqueue(&cmp_freq, NULL);

  pr;

  CharFreqNode a, b, c, d, e, f, g;
  a.freq = 7;
  b.freq = 5;
  c.freq = 6;
  d.freq = 3;
  e.freq = 1;
  f.freq = 11;
  g.freq = 4;

  push_pqueue(queue, &a);
  pr;
  push_pqueue(queue, &b);
  pr;
  push_pqueue(queue, &c);
  pr;
  push_pqueue(queue, &d);
  pr;
  push_pqueue(queue, &e);
  pr;
  push_pqueue(queue, &f);
  pr;
  push_pqueue(queue, &g);
  pr;

  delete_pqueue(&queue);
  pr;

}



int get_tree_depth(CharFreqNode* root, int counter = 0)
{
  int max_deep = 0;

  if (root == NULL)
    return counter;

  int l_deep = get_tree_depth(root->left_node, counter + 1);
  int r_deep = get_tree_depth(root->right_node, counter + 1);

  max_deep = (l_deep > max_deep) ? l_deep : max_deep;
  max_deep = (r_deep > max_deep) ? r_deep : max_deep;

  return max_deep;

}


void print_tree_line(CharFreqNode* root, int deep = 0, int counter = 0, int max_deep = 0)
{
  if (root != NULL)
  {
    if (counter < deep)
    {
      print_tree_line(root->left_node, deep, counter + 1, max_deep);
      print_tree_line(root->right_node, deep, counter + 1, max_deep);
    }
    else if (counter == deep)
    {
      if (root->left_node != NULL && root->right_node != NULL)
      {
        printf("N: %d", root->freq);
      }
      else
      {
        printf("S: %d%c", root->freq, root->ch);
      }
      for (int i = 0; i < (max_deep-deep); ++i)
        printf("   ");
    }
  }
}




void print_tree(CharFreqNode* root)
{

  int d = get_tree_depth(root);

  for (int i = 0; i <= d; ++i)
  {
    for (int j = 0; j < (d - i); ++j)
      printf("   ");
    print_tree_line(root, i, 0, d);
    printf("\n");
  }
}

void delete_Huffman_tree(CharFreqNode*& root)
{
  if (root == NULL)
    return;

  delete_Huffman_tree(root->left_node);
  delete_Huffman_tree(root->right_node);

  free(root);
  root = NULL;
}

void test_create_Huffman()
{
  const char* test_str = "sbbbssssaaapjshadtfhqa[wec\\erna[grfnhfj;zdsfghnz'n;4syt; rg'sptazw4smmllllllll";

  char ch;

  CharFreqNode alph[256];

  for (int i = 0; i < 256; ++i)
  {
    alph[i].ch = i;
    alph[i].freq = 0;
    alph[i].left_node = NULL;
    alph[i].right_node = NULL;
  }

  for (int i = 0; (ch = test_str[i]) != 0; ++i)
  {
    alph[ch].freq++;
  }

  PQueue* queue = create_pqueue(&cmp_freq, NULL);

  for (int i = 0; i < 256; ++i)
  {
    if (alph[i].freq > 0)
    {
      CharFreqNode* node = (CharFreqNode*)malloc(sizeof(CharFreqNode));
      node->ch = alph[i].ch;
      node->freq = alph[i].freq;
      node->left_node = NULL;
      node->right_node = NULL;
      push_pqueue(queue, node);

      pr;
    }
  }

  CharFreqNode* root = NULL;

  root = make_Huffman_tree(queue);

  if (root == NULL)
    return; // FAULT!!

  printf("Tree depth: %d\n\n", get_tree_depth(root));
  print_tree(root);

  delete_Huffman_tree(root);

  delete_pqueue(&queue);
};


void test_packaging()
{


}

int main(int argc, char* argv[])
{
  //test_prio_queue();
  test_create_Huffman();

  return 0;
}

