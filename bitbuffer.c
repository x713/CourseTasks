#include "bitbuffer_f.h"

#include <stdlib.h>
#include <stdio.h>


BitBuffer* create_bit_buffer()
{
  BitBuffer* buf = calloc(1, sizeof(BitBuffer));
  return buf;
}

void delete_bit_buffer(BitBuffer** buf)
{
  if ((*buf) == NULL)
    return;

  free(*buf);
  *buf = NULL;
}


void reset_bit_buffer(BitBuffer* buf)
{
  buf->count = 0;
  for (int i = 0; i < BIT_BUFFER_MAX; ++i)
    buf->bit_buffer[i] = 0;
}

bool add_bit_buffer(BitBuffer* buf, unsigned char bit)
{
  if (buf->count >= BIT_BUFFER_WIDTH)
    return false;

  unsigned int segment = buf->count / 8;
  unsigned int offset = buf->count % 8;

  if (bit)
    buf->bit_buffer[segment] = buf->bit_buffer[segment] | (unsigned char)(0x01 << offset);
  else
    buf->bit_buffer[segment] = buf->bit_buffer[segment] & ~((unsigned char)(0x01 << offset));

  buf->count++;

  return true;
}

bool is_empty_bit_buffer(BitBuffer* buf)
{
  return buf->count == 0;
}

bool is_buffer_full(BitBuffer* buf)
{
  return buf->count >= BIT_BUFFER_WIDTH;
}

unsigned char pop_bit_buffer(BitBuffer* buf)
{
  unsigned int segment = buf->count / 8;
  unsigned int offset = buf->count % 8;

  buf->count--;

  return buf->bit_buffer[segment] & (0x01 << offset);
}

unsigned char get_bit_buffer(BitBuffer* buf, unsigned int pos)
{
  unsigned int segment = pos / 8;
  unsigned int offset = pos % 8;

  return (buf->bit_buffer[segment] & (0x01 << offset)) ? 1 : 0;
}

void set_bit_buffer(BitBuffer* buf, unsigned int pos, unsigned char bit)
{
  unsigned int segment = buf->count / 8;
  unsigned int offset = buf->count % 8;

  if (bit)
    buf->bit_buffer[segment] = buf->bit_buffer[segment] | (unsigned char)(0x01 << offset);
  else
    buf->bit_buffer[segment] = buf->bit_buffer[segment] & ~((unsigned char)(0x01 << offset));
}

void copy_buffer(BitBuffer* src, BitBuffer* dest)
{
  unsigned int segment = src->count / 8;
  unsigned int offset = src->count % 8;

  unsigned int in_bytes = segment + (offset > 0 ? 1 : 0);

  for (unsigned int i = 0; i < in_bytes; ++i)
  {
    dest->bit_buffer[i] = src->bit_buffer[i];
  }
  dest->count = src->count;
}

void add_buffer_slice(BitBuffer* src, BitBuffer* dest, BitBuffer* src_slice)
{
  src_slice->count = 0;
  int free_space = BIT_BUFFER_WIDTH - dest->count;

  if (free_space > 0)
  {
    if (free_space - src->count > 0)
    {
      for (unsigned int i = 0; i < src->count; ++i)
      {
        add_bit_buffer(dest, get_bit_buffer(src, i));
      }
    }
    else
    {
      for (int i = 0; i < free_space; ++i)
      {
        add_bit_buffer(dest, get_bit_buffer(src, i));
      }

      int rest = src->count - free_space;
      src_slice->count = rest;
      for (int i = 0; i < rest; ++i)
      {
        set_bit_buffer(src_slice, i, get_bit_buffer(src, i + free_space));
      }
    }
  }
}


void print_bit_buffer(BitBuffer* buf)
{
  for (unsigned int i = 0; i < buf->count; ++i)
  {
    unsigned int segment = i / 8;
    unsigned int offset = i % 8;

    printf("%d", buf->bit_buffer[segment] & (unsigned char)(0x01 << offset) ? 1 : 0);
  }
  printf("\n");
}

void test_packaging()
{
  BitBuffer* buf = create_bit_buffer();
  if (buf == NULL)
    return;

  reset_bit_buffer(buf);

  int i = 0;
  while (i++ < 10)
  {
    add_bit_buffer(buf, 0);
    add_bit_buffer(buf, 1);
    add_bit_buffer(buf, 1);
  }
  print_bit_buffer(buf);

  delete_bit_buffer(&buf);
}

bool is_little_endian()
{
  int num = 1;

  return (*(char*)&num == 1) ? true : false;
};


void swap(unsigned char* x, unsigned char* y)
{
  *x ^= *y;
  *y ^= *x;
  *x ^= *y;
};

typedef union
{
  unsigned int num;
  unsigned char ch[4];
} int32_endian;


unsigned int toggle_endian(unsigned int num)
{

  int32_endian i = { num };

  swap(&i.ch[0], &i.ch[3]);
  swap(&i.ch[1], &i.ch[2]);

  return i.num;
}

void test_packaging2()
{
  BitBuffer* buf = create_bit_buffer();

  if (buf == NULL)
    return;

  //reset_bit_buffer(buf);

  int i = 0;
  while (i++ < 10)
  {
    add_bit_buffer(buf, 0);
    add_bit_buffer(buf, 1);
    add_bit_buffer(buf, 1);
  }
  print_bit_buffer(buf);

  FILE* fp;
  fopen_s(&fp, "hfzout.x7", "wb");

  if (!fp)
  {
    delete_bit_buffer(&buf);
    return;
  }

  int32_endian segment = { buf->count / 8 };
  unsigned char offset = buf->count % 8;

  printf("o: %d\n", offset);
  printf("s: %d\n", segment.num);

  if (is_little_endian())
  {
    printf("little\n");
    unsigned int nnum = toggle_endian(segment.num);
    printf("s: %d\n", nnum);
  }
  else
  {
    printf("big\n");
    printf("s: %d\n", segment.num);
  }

  //fwrite(buf->bit_buffer, 1,  fp);

  if (fp != NULL)
  {
    fclose(fp);
    fp = NULL;
  }

  delete_bit_buffer(&buf);
}