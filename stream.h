#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

#include "counter.h"
#include "canonical.h"
void stream_counter_read(counter_t* tree, FILE* in);
void stream_encode(canonical_t* tree, FILE* in, FILE* out);
void stream_decode(FILE* in, FILE* out);

#endif // STREAM_H