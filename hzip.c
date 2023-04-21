#include <stdio.h>
#include <string.h>

//#define _DEBUG_PRN

#include "huff_tree.h"

// Loads Bytes to bit_buffer
void refresh_buffer(BitBuffer* buf, FILE* fp_in)
{
  buf->count = fread_s(&buf->bit_buffer, BIT_BUFFER_MAX, 1, BIT_BUFFER_MAX, fp_in);
  buf->count *= 8; // Save bits count
}

// Main encoding func

// TODO : refactor/ perf
// more incapsulation and abstraction?
// eliminate stack involving / inlines
void decode_buffer(BitBuffer* src, CharFreqNode* node, FILE* fp_in, FILE* fp_out, unsigned int* pos)
{
  if (src->count == 0)
    return;

  if (node->left_node == NULL && node->right_node == NULL)
  {
    fputc(node->ch, fp_out);

    gl_filesize--;

#ifdef _DEBUG_PRN
    printf("Found char: %c\n", node->ch);
#endif
  }
  else
  {
    char bit = get_bit_buffer(src, *pos);

#ifdef _DEBUG_PRN
    printf("Processed bit : %d   Processed pos : %d\n", bit, *pos);
#endif

    (*pos)++;

    if ((*pos) >= BIT_BUFFER_WIDTH)
    {
      (*pos) = 0;
      refresh_buffer(src, fp_in);
    }

    if (node->left_node != NULL && bit == 0)
    {
      decode_buffer(src, node->left_node, fp_in, fp_out, pos);
    }
    if (node->right_node != NULL && bit == 1)
    {
      decode_buffer(src, node->right_node, fp_in, fp_out, pos);
    }
  }
}


void save_int_e(int i, FILE* fp_out)
{
  if (is_little_endian())
  {
    //printf("little e\n");
    i = toggle_endian(i);
  }
  else
  {
    //printf("big\n");
  }
  fwrite(&i, gl_int_size, 1, fp_out);
}

void load_int_e(int* i, FILE* fp_in)
{
  fread_s(&(*i), gl_int_size, gl_int_size, 1, fp_in);
  if (is_little_endian())
  {
    *i = toggle_endian(*i);
  }
}

void encode(BitBuffer** codes, BitBuffer* tmp, BitBuffer* tmp_slice, FILE* fp_in, FILE* fp_out)
{
  long ps = 0;
  int t = fgetc(fp_in);
  while (t != EOF)
  {

    if (codes[t] != NULL)
    {
      tmp_slice->count = 0;

      add_buffer_slice(codes[t], tmp, tmp_slice);
      //printf("%c", t);
      do
      {
        if (is_buffer_full(tmp))
        {
          fwrite(&tmp->bit_buffer, 1, BIT_BUFFER_MAX, fp_out);
          tmp->count = 0;
        }
        if (tmp_slice->count > 0)
        {
          add_buffer_slice(tmp_slice, tmp, tmp_slice);
        }

      } while (tmp_slice->count > 0);
    }
    t = fgetc(fp_in);
    ps++;
  }
  printf("\nProcessed: %d bytes\n", ps);
}


void make_archive(char* filename_in, char* filename_out)
{
  FILE* fp_in;

  fopen_s(&fp_in, filename_in, "rb");

  if (!fp_in)
  {
    return;
  }

  unsigned int alph_size = 0;
  CharFreqNode alph[256];

  for (int i = 0; i < 256; ++i)
  {
    alph[i].ch = i;
    alph[i].freq = 0;
    alph[i].left_node = NULL;
    alph[i].right_node = NULL;
  }

  gl_filesize = 0;
  int ch;
  while ((ch = getc(fp_in)) != EOF) {
    alph[(unsigned char)ch].freq++;
    gl_filesize++;
  }

  printf("filesize: %d\n", gl_filesize);


  PQueue* queue = create_pqueue(&cmp_freq, NULL);

  for (int i = 0; i < 256; ++i)
  {
    if (alph[i].freq > 0)
    {
      CharFreqNode* node = (CharFreqNode*)malloc(sizeof(CharFreqNode));
      if (node == NULL)
      {
        CharFreqNode* data;
        while (!is_empty(queue))
        {
          data = pop_pqueue(queue);
          if (data != NULL)
          {
            free(data);
            data = NULL;
          }
        }
        return;
      }

      alph_size++;
      node->ch = alph[i].ch;
      node->freq = alph[i].freq;
      node->left_node = NULL;
      node->right_node = NULL;
      push_pqueue(queue, node);
    }
  }

  //pr;


  if (alph_size == 0)
  {
    printf("Warning: Nothing to compress. Exiting..\n");
  }
  else
  {
    CharFreqNode* root = NULL;

    root = make_Huffman_tree_pqueue(queue);

    if (root == NULL)
      return; // FAULT!!

    printf("Huffman Tree depth: %d\n\n", get_tree_depth(root, 0));
    //print_tree(root);

    ///
    ///  SAVE TREE
    /// 
    BitBuffer* tree_buff = create_bit_buffer();
    if (tree_buff != NULL)
    {
      // save tree structure to bitBuffer
      serialize_tree_structure(root, tree_buff);

      //printf("Alp size :%d\n", alph_size);
      //printf("Bb size :%d\nBb Bsize :%d\nBb offs :%d\nBb: ", tree_buff->count, tree_buff->count / 8, tree_buff->count % 8);
      //print_bit_buffer(tree_buff);

      // Save Header
      FILE* fp_out;
      fopen_s(&fp_out, filename_out, "wb");

      if (fp_out != NULL)
      {
        // Write signature
        fputc('H', fp_out);
        fputc('F', fp_out);
        fputc('A', fp_out);
        fputc('0', fp_out);
        fputc('1', fp_out);

        unsigned int fs_segment = gl_filesize / 32;
        unsigned int fs_offset = gl_filesize % 32;

        // Write orig FILESIZE
        save_int_e(fs_segment, fp_out);
        save_int_e(fs_offset, fp_out);

        unsigned int segment = tree_buff->count / 8;
        unsigned char offset = tree_buff->count % 8;

        // Write Buffered tree size
        save_int_e(segment, fp_out);
        fputc(offset, fp_out);

        segment = segment + (offset > 0 ? 1 : 0);

        // Write Tree info buffer content to file.
        fwrite(&tree_buff->bit_buffer, 1, segment, fp_out);

        // Write alpabet characters
        save_tree_alphabeth(root, fp_out);

        // Allocate buffers for tree codes
        BitBuffer* codes[256];

        int alloceted_alph = 0;
        for (int i = 0; i < 256; ++i)
        {
          if (alph[i].freq > 0)
          {
            codes[i] = create_bit_buffer();
            if (codes[i] != NULL)
            {
              alloceted_alph++;
            }
            else
            {
              break;
            }
          }
          else
          {
            codes[i] = NULL;
          }
        }

        if (alloceted_alph != alph_size)
        {
          printf("Error: Allocating memory buffer HUffman codes failed on %d of %d!\n", alloceted_alph, alph_size);
        }
        else
        {
          BitBuffer* tmp = create_bit_buffer();
          BitBuffer* tmp_slice = create_bit_buffer();

          if (tmp != NULL && tmp_slice != NULL)
          {
            prepare_tree_codes(root, codes, tmp);

            printf("Decoding...");
            fseek(fp_in, 0, SEEK_SET);

            tmp->count = 0;

            // TODO : Progress bar

            encode(codes, tmp, tmp_slice, fp_in, fp_out);

            if (!is_empty_bit_buffer(tmp))
            {
              int size_in_bytes = (tmp->count / 8) + ((tmp->count % 8) > 0 ? 1 : 0);
              fwrite(&tmp->bit_buffer, 1, size_in_bytes, fp_out);
              tmp->count = 0;
            }

            // TODO : add Checksum here

            printf("Encoded - %s \n", filename_out);

            delete_bit_buffer(&tmp);
          }
          else
          {
            printf("Error: Allocating tmp memory failed\n");
            if (tmp != NULL)
              delete_bit_buffer(&tmp);
            if (tmp_slice != NULL)
              delete_bit_buffer(&tmp);
          }
        }

        // Clean created buffers
        for (int i = 0; i < 256; ++i)
        {
          if (codes[i] != NULL)
          {
            delete_bit_buffer(&codes[i]);
            codes[i] = NULL;
          }
        }

        fclose(fp_out);
      }


      delete_bit_buffer(&tree_buff);
    }

    delete_Huffman_tree(&root);
  }
  fclose(fp_in);

  delete_pqueue(&queue);
};

// | HFA | version |
// | filesize_segment (int32 / 64) | filesize_ofsset (int32 % 64) |
// | treesize_segment (int32 / 8) | tresize_ofsset (char8) |
// | nodes_bits | ch | .. | ch |
// | encoded bits |

void decode_archive(char* filename_in, char* filename_out)
{
  // Read Header
  FILE* fp_in;
  fopen_s(&fp_in, filename_in, "rb");

  if (fp_in != NULL)
  {
    char h[5];

    if (fread_s(&h, 5, 1, 5, fp_in) != 5)
    {
      printf("Error: Can't read Header\n");
      fclose(fp_in);
      return;
    }
    else if (h[0] != 'H' || h[1] != 'F' || h[2] != 'A')
    {
      printf("Error: Not HFA archive\n");
      fclose(fp_in);
      return;
    }
    else if (h[3] != '0' || h[4] != '1')
    {
      printf("Error: wrong HFA version\n");
      fclose(fp_in);
      return;
    }

    printf("Reading archive tree\n");

    unsigned int fs_segment = 0;
    unsigned int fs_offset = 0;

    // Load orig FILESIZE
    load_int_e(&fs_segment, fp_in);
    load_int_e(&fs_offset, fp_in);

    gl_filesize = fs_segment * 32 + fs_offset;

    // Read BuffSize
    unsigned int segment;
    unsigned char offset;
    const my_int_size = 4;

    load_int_e(&segment, fp_in);
    offset = fgetc(fp_in);

    //printf("Segment: %d\n Offset: %d\n", segment, offset);

    unsigned int expected_buff_size = segment + (offset > 0 ? 1 : 0);

    // Read Tree Buf
    BitBuffer* tree_buff = create_bit_buffer();
    if (tree_buff != NULL)
    {
      unsigned int bytes_read = (unsigned int)fread_s(&tree_buff->bit_buffer, BIT_BUFFER_MAX, 1, expected_buff_size, fp_in);

      if (bytes_read < expected_buff_size)
      {
        printf("Error: Unexpected EOF");
      }
      if (bytes_read != expected_buff_size)
      {
        printf("Error: Unexpected IO error");
      }
      else
      {
        tree_buff->count = segment * 8 + offset;
        //printf("Bb: ");
        //print_bit_buffer(tree_buff);
        //printf("Bb count: %d\n", tree_buff->count);
      }
      //printf("\n");

      CharFreqNode* root = make_Huffman_tree_buff(tree_buff);

      //printf("Tree depth: %d\n\n", get_tree_depth(root, 0));

      // Read Tree alphabet
      bool err_flag = false;
      load_tree_node(root, fp_in, &err_flag);

      // MAIN MISSION
      // START TO DECODE  O_o /* ~-<<< << {{ [ [  :  :  
      if (err_flag)
      {
        printf("Error: EOF while loading tree alphabeth.\n");
      }
      else
      {

#ifdef _DEBUG_PRN
        print_tree(root);
#endif
        printf("Start decoding\n");

        BitBuffer* read_buffer = create_bit_buffer();

        if (read_buffer != NULL)
        {
          FILE* fp_out;
          fopen_s(&fp_out, filename_out, "wb");

          if (fp_out != NULL)
          {
            unsigned int pos = 0;
            refresh_buffer(read_buffer, fp_in);
            //print_bit_buffer(read_buffer);

            bool procesing = read_buffer->count > 0;

            while (procesing)
            {
              decode_buffer(read_buffer, root, fp_in, fp_out, &pos);

              if (gl_filesize == 0)
                procesing = false;
            }


            fclose(fp_out);
          }
          {
            printf("\nError: Can't create file - %s \n", filename_out);
          }

          delete_bit_buffer(&read_buffer);
        }
      }

      delete_Huffman_tree(&root);

      delete_bit_buffer(&tree_buff);
    }


    fclose(fp_in);
    printf("\nDecoded successfully - %s\n", filename_out);
  }
  else {
    printf("\nError: Can't open file - %s \n", filename_in);
  }
}

void print_help()
{
  printf("Compressing file with Huffman codes\n");
  printf("Using:\n");
  printf("   -c filename_in filename_out\n");
  printf("     to compress file\n");
  printf("   -d filename_in filename_out\n");
  printf("     to decompress file\n\n");
}

int main(int argc, char** argv)
{

  if (argc < 4)
  {
    print_help();
    return 0;
  }

  char* param = argv[1];
  char* filename_in = argv[2];
  char* filename_out = argv[3];

  if (0 == strcmp("-c", param))
  {
    make_archive(filename_in, filename_out);
  }
  else if (0 == strcmp("-d", param))
  {
    decode_archive(filename_in, filename_out);
  }

  /*
  * refactor
  *  new file_buffer > bit_buffer size 256
  *  perf test on buff_size
  */

  return 0;
}


