#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

#include "counter.h"
#include "canonical.h"

void stream_encode(FILE* in, FILE* out, canonical_t* tree);
int stream_encode_write(FILE* out, canonical_t* tree, char character, uint16_t* block, size_t* block_length);

void stream_decode(FILE* in, FILE* out);

void stream_counter_read(FILE* in, counter_t* tree);
void stream_canonical_write(FILE* out, canonical_t* tree);
void stream_canonical_get(FILE* in, canonical_t* tree);

#endif // STREAM_H