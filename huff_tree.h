#ifndef HUFF_TREE_CLASS
#define HUFF_TREE_CLASS

#include "bitbuffer.h"
#include "pqueue.h"

#include <stdlib.h>
#include <stdio.h>


#define gl_int_size 4

unsigned long gl_filesize;

// V2

// STRUCTURES
struct CharFreqNode_t {
  unsigned long freq;
  unsigned char ch;
  struct CharFreqNode_t* left_node;
  struct CharFreqNode_t* right_node;
};
typedef struct CharFreqNode_t CharFreqNode;

// Comparing funcition
bool cmp_freq(void* d1, void* d2);

CharFreqNode* make_Huffman_tree_pqueue(PQueue* queue);

int get_tree_depth(CharFreqNode* root, int counter);

void delete_Huffman_tree(CharFreqNode** root);

CharFreqNode* make_Huffman_tree_buff(BitBuffer* bitbuffer);

void serialize_tree_structure(CharFreqNode* root, BitBuffer* buffer);

void load_tree_node(CharFreqNode* root, FILE* fp, bool* err);

void prepare_tree_codes(CharFreqNode* root, BitBuffer** codes, BitBuffer* ongoing_code);

void save_tree_alphabeth(CharFreqNode* root, FILE* fp);

void print_tree(CharFreqNode* root);

#endif // !HUFF_TREE_CLASS
