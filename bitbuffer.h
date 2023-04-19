#pragma once
#include <stdbool.h>

#define BIT_BUFFER_MAX 256
#define BIT_BUFFER_WIDTH 256*8

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

unsigned char pop_bit_buffer(BitBuffer*);

void print_bit_buffer(BitBuffer*);

// TEST
void test_packaging();

// bool is_8bits_free_bit_buffer(BitBuffer*); // can add 8 bits
//unsigned int add_byte_buffer(BitBuffer*, unsigned char);
//unsigned int add_byte_slice(BitBuffer*, unsigned char data, unsigned char slice); // byte data, byte slice from [0-slice]

