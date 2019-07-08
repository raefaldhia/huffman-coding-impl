#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

#include "counter.h"

void stream_counter_read(counter_t* tree, FILE* in);
void stream_decode(FILE* in, FILE* out);

#endif // STREAM_H