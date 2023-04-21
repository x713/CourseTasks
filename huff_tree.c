#include "huff_tree.h"
#include "stdio.h"

// Print debug output
// #define _DEBUG_PRN

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

CharFreqNode* make_Huffman_tree_pqueue(PQueue* queue)
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

CharFreqNode* make_Huffman_tree_node_buff(BitBuffer* bitbuffer, unsigned int* bit_pointer)
{
  // count - 2
  // pointer - 0
  if (*bit_pointer >= bitbuffer->count)
  {
    printf("Unxepected end of buffer\n");
    return NULL;
  }

  unsigned char ln = get_bit_buffer(bitbuffer, (*bit_pointer)++);
  unsigned char rn = get_bit_buffer(bitbuffer, (*bit_pointer)++);

  CharFreqNode* node = (CharFreqNode*)malloc(sizeof(CharFreqNode));
  if (node == NULL)
  {
    printf("Error: Creating Tree. Memory allocation unsuccessfull!\n");
    return NULL;
  }

  node->ch = 0;
  node->freq = 0;

  node->left_node = NULL;
  node->right_node = NULL;

  if (ln)
  {
    node->left_node = make_Huffman_tree_node_buff(bitbuffer, bit_pointer);
  }
  if (rn)
  {
    node->right_node = make_Huffman_tree_node_buff(bitbuffer, bit_pointer);
  }

  return node;
}

CharFreqNode* make_Huffman_tree_buff(BitBuffer* bitbuffer)
{
  unsigned int ptr = 0;
  CharFreqNode* root = make_Huffman_tree_node_buff(bitbuffer, &ptr);

  if (root == NULL)
  {
    printf("Error: Tree is empty");
    return NULL; // FAULT!!
  }

  return root;
};


int get_tree_depth(CharFreqNode* root, int counter)
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


void print_tree_line(CharFreqNode* root, int deep, int counter, int max_deep)
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
        //printf("N: %d", root->freq);
        printf("N:      ");
      }
      else
      {
        //printf("S: %d%c", root->freq, root->ch);
        unsigned char fmt = root->ch;
        fmt = ((fmt > ' ' && fmt < '~') ? fmt : '*');
        printf("S: %c  ", fmt);
      }
      for (int i = 0; i < (max_deep - deep); ++i)
        printf("   ");
    }
  }
}

void print_tree(CharFreqNode* root)
{

  int d = get_tree_depth(root, 0);

  for (int i = 0; i <= d; ++i)
  {
    for (int j = 0; j < (d - i); ++j)
      printf("      ");
    print_tree_line(root, i, 0, d);
    printf("\n");
  }
}

void delete_Huffman_tree(CharFreqNode** root)
{
  if (*root == NULL)
    return;

  delete_Huffman_tree(&((*root)->left_node));
  delete_Huffman_tree(&((*root)->right_node));

  free(*root);
  root = NULL;
}

void serialize_tree_structure(CharFreqNode* root, BitBuffer* buffer)
{
  if (root == NULL)
    return;

  // nodes - (packed in 2 bits)
  // 00 - no nodes
  // 10 - left leaf
  // 01 - right leaf
  // 11 - both leaves

  add_bit_buffer(buffer, (root->left_node != NULL ? 1 : 0));
  add_bit_buffer(buffer, (root->right_node != NULL ? 1 : 0));

  if (root->left_node != NULL)
  {
    serialize_tree_structure(root->left_node, buffer);
  }
  if (root->right_node != NULL)
  {
    serialize_tree_structure(root->right_node, buffer);
  }
}

void save_tree_alphabeth(CharFreqNode* root, FILE* fp)
{
  if (root != NULL)
  {
    if (root->left_node == NULL && root->right_node == NULL)
    {
      fputc(root->ch, fp);
      return;
    }
    else
    {
      if (root->left_node != NULL)
      {
        save_tree_alphabeth(root->left_node, fp);
      }
      if (root->right_node != NULL)
      {
        save_tree_alphabeth(root->right_node, fp);
      }
    }
  }
}

void load_tree_node(CharFreqNode* root, FILE* fp, bool* err)
{
  if (!(*err) && root != NULL)
  {
    if (root->left_node == NULL && root->right_node == NULL)
    {
      int ch = fgetc(fp);
      if (ch == EOF)
      {
        *err = true;
        return;
      }
      else
      {
        root->ch = (char)ch;
      }
    }
    else
    {
      if (root->left_node != NULL)
      {
        load_tree_node(root->left_node, fp, err);
      }
      if (root->right_node != NULL)
      {
        load_tree_node(root->right_node, fp, err);
      }
    }
  }
}

void prepare_tree_codes(CharFreqNode* root, BitBuffer** codes, BitBuffer* ongoing_code)
{
  if (root == NULL)
  {
    return;
  }

  if (root->left_node == NULL && root->right_node == NULL)
  {

#ifdef _DEBUG_PRN
    printf("////////prepare codes//////\n");
    printf("char: '%c' code\n", root->ch);
    print_bit_buffer(ongoing_code);
#endif

    copy_buffer(ongoing_code, codes[root->ch]);
    pop_bit_buffer(ongoing_code);
    return;
  }

  if (root->left_node != NULL)
  {
    add_bit_buffer(ongoing_code, 0);
    prepare_tree_codes(root->left_node, codes, ongoing_code);
  }

  if (root->right_node != NULL)
  {
    add_bit_buffer(ongoing_code, 1);
    prepare_tree_codes(root->right_node, codes, ongoing_code);
  }
  pop_bit_buffer(ongoing_code);
}

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

  CharFreqNode a[7];
  a[0].freq = 7;
  a[1].freq = 5;
  a[2].freq = 6;
  a[3].freq = 3;
  a[4].freq = 1;
  a[5].freq = 11;
  a[6].freq = 4;
  for (int i = 0; i < 7; ++i)
  {
    a[i].ch = 'A' + i;
    push_pqueue(queue, &a[i]);
    pr;
  }

  delete_pqueue(&queue);
  pr;

}


void test_create_Huffman()
{
  const char* test_str = "sbbbssssaaapjshadtfhqa[wec\\erna[grfnhfj;zdsfghnz'n;4syt; rg'sptazw4smmllllllll";

  char ch;

  CharFreqNode alph[256];
  unsigned char alph_size = 0;

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
      if (node == NULL)
      {
        while (!is_empty(queue))
        {
          CharFreqNode* data = pop_pqueue(queue);
          if (data != NULL)
            free(data);
        }
        return;
      }

      node->ch = alph[i].ch;
      node->freq = alph[i].freq;
      node->left_node = NULL;
      node->right_node = NULL;
      push_pqueue(queue, node);

      //pr;
    }
  }


  CharFreqNode* root = make_Huffman_tree_pqueue(queue);

  if (root != NULL)
  {
    printf("Tree depth: %d\n\n", get_tree_depth(root, 0));
    print_tree(root);

    //
    delete_Huffman_tree(&root);
  }


  delete_pqueue(&queue);
};
