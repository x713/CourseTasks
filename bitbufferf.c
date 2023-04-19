#pragma once
#include "bitbuffer_f.h"

void add_bit_buffer_f(BitBuffer* buf, unsigned char bit)
{
  unsigned int segment = buf->count / 8;
  unsigned int offset = buf->count % 8;

  if (bit)
    buf->bit_buffer[segment] = buf->bit_buffer[segment] | (unsigned char)(0x01 << offset);
  else
    buf->bit_buffer[segment] = buf->bit_buffer[segment] & ~((unsigned char)(0x01 << offset));

  buf->count++;
}


void add_byte_buffer_f(BitBuffer* buf, unsigned char byte)
{
  // TODO: reconsider
  for (int offset = 0; offset < 8; ++offset)
  {
    add_bit_buffer_f(buf, byte & (unsigned char)(0x01 << offset)); 
  }
}