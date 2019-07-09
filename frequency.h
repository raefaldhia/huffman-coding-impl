#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <stddef.h>
#include <stdint.h>

#include "tree.h"
#include "counter.h"

/* each field must have equal size as described in node_t (tree.h) */
typedef struct frequency_node_s {
	char character;
	size_t frequency;
	int16_t _reserved;
	struct frequency_node_s* right;
	struct frequency_node_s* left;
} frequency_node_t;

typedef struct frequency_s {
	frequency_node_t* head;
} frequency_t;

frequency_node_t* frequency_push(frequency_node_t* node, char character, int frequency);

void counter_reinit_frequency(counter_t* tree);

#endif // FREQUENCY_H
