#ifndef BIT_BUFFER_CLASS
#define BIT_BUFFER_CLASS
#include <stdbool.h>

#define BIT_BUFFER_MAX 256
#define BIT_BUFFER_WIDTH BIT_BUFFER_MAX*8

#define BIT_BUFFER_CODES_OK 8
#define BIT_BUFFER_CODES_NULL_BUFFER 9
#define BIT_BUFFER_CODES_SLICE 10

// STRUCTURES

typedef struct {
  unsigned int count;
  unsigned char bit_buffer[BIT_BUFFER_MAX];
} BitBuffer;

// FUNCTIONS

BitBuffer* create_bit_buffer();

void delete_bit_buffer(BitBuffer** buf);

void reset_bit_buffer(BitBuffer*);

bool add_bit_buffer(BitBuffer*, const unsigned char);

bool is_empty_bit_buffer(BitBuffer*);
bool is_buffer_full(BitBuffer* buf);

unsigned char pop_bit_buffer(BitBuffer*);
unsigned char get_bit_buffer(BitBuffer* buf, unsigned int pos);

void copy_buffer(BitBuffer* src, BitBuffer* dest);
void add_buffer_slice(BitBuffer* src, BitBuffer* dest, BitBuffer* src_slice);

void print_bit_buffer(BitBuffer*);

bool is_little_endian();
unsigned int toggle_endian(unsigned int num);

// TEST
void test_packaging();
void test_packaging2();

#endif // !BIT_BUFFER_CLASS