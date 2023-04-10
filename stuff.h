#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

int some_stuff()
{
  // get parameters - code/decode
// code
//  calculate freq - create queue_p<char, freq, cmp_f>
//  create Huffman tree - pop 2 min freq create leaf, put back into queue_p with freq1+freq2. while queue_p>1;
//
//  serizlize huffman tree
//  save huffman tree
//  if source !EOF 
//    encode bytes - paging?
//    save page size
//    save encoded page
// 
//  decode
//   deserialize tree
//   if source !EOF
//   read page size
//   decode page
//

/*  if (argv != 3 || argc[1] != "-c")
  {
    //printf("Incorrect paramater: %s", argc[1]);
    print_help();
    return -2;
  }
  */
  //  char filename[] = "D:\\Projects\\cpp\\emp_c\\CourseTasks\\filename.txt";
  char filename[] = "D:\\x360ce.zip";
  FILE* file;
  fopen_s(&file, filename, "r");
  if (file == NULL)
  {
    printf("Can't open file %s", filename);

    return -1;
  }

  //  PQueue* pqueue = create_pqueue();

  unsigned int freq_arr[256];
  memset(freq_arr, 0, sizeof(freq_arr));

  char one_char;

  while ((one_char = fgetc(file)) != EOF)
  {
    printf("%c", one_char);
    freq_arr[one_char]++;
  }
  printf("\n");

  for (char a = 'A'; a != 'Z'; a++)
    printf("Char %c : %d\n", a, freq_arr[a]);

  for (char a = 'a'; a != 'z'; a++)
    printf("Char %c : %d\n", a, freq_arr[a]);

  /*
    Tree* tree = create_tree();

    if (!tree)
    {
      printf("Memory can't be assigned");

      fclose(file);
      return -1;
    }

    delete_tree(tree);
    */
  fclose(file);
  printf("Done");

}
