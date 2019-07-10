#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

#include "counter.h"
#include "canonical.h"

typedef struct stream_block_s {
	FILE* out;
    canonical_t* tree;
	uint16_t data;
	size_t size;
} stream_block_t;

void stream_encode(FILE* in, FILE* out, canonical_t* tree);
void stream_encode_write(FILE* in, FILE* out, canonical_t* tree);
int stream_encode_writeb(stream_block_t* block, char character);

void stream_decode(FILE* in, FILE* out);

void stream_counter_read(FILE* in, counter_t* tree);
void stream_canonical_write(FILE* out, canonical_t* tree);
void stream_canonical_get(FILE* in, canonical_t* tree);

#endif // STREAM_H