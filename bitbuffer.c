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


unsigned char add_sliced_byte_buffer(BitBuffer* buf, unsigned char data, unsigned char slice)
{
   // TODO : DODO

  if (buf == NULL)
  {
    return BIT_BUFFER_CODES_NULL_BUFFER;
  }

  unsigned char space = BIT_BUFFER_WIDTH - buf->count;

  if (space >= 9)
  {
    add_bit_buffer_f(buf, data);
    return BIT_BUFFER_CODES_OK;
  }
  
  /*
  add slice

  */
  return BIT_BUFFER_CODES_SLICE; // + slice offset
}


bool is_empty_bit_buffer(BitBuffer* buf)
{
  if (buf == NULL)
    return true;

  return buf->count > 0;
}

unsigned char pop_bit_buffer(BitBuffer* buf)
{
  unsigned int segment = buf->count / 8;
  unsigned int offset = buf->count % 8;

  buf->count--;

  return buf->bit_buffer[segment] & (0x01 << offset);

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