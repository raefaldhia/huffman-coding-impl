#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <stddef.h>
#include <stdbool.h>

#include "tree.h"

// linked with node_t
typedef struct frequency_node_s {
	char character;
	size_t frequency;
	bool _reserved; // initialize to 0, for later use by huffman intermediate
	struct frequency_node_s* right;
	struct frequency_node_s* left;
} frequency_node_t;

void frequency_push(tree_t* tree, char character, int frequency);

void counter_reinit_frequency(tree_t* tree);

#endif // FREQUENCY_H
