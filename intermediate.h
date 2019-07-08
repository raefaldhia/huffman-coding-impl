#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "tree.h"
#include "stack.h"
#include "frequency.h"

typedef struct intermediate_node_s {
	char character;
	size_t weight;
	bool is_chain;
	struct intermediate_node_s* right;
	struct intermediate_node_s* left;
} intermediate_node_t;

typedef struct intermediate_s {
    intermediate_node_t* head;
} intermediate_t;

typedef struct intermediate_node_extractor_s {
    stack_t stack;
} intermediate_node_extractor_t;

void intermediate_push(intermediate_t* tree, intermediate_node_t* node);
intermediate_node_t* intermediate_chain_create(intermediate_node_t** node);
intermediate_node_t* intermediate_carrier_create(intermediate_node_t* first, intermediate_node_t* second);

tree_retval_t frequency_reinit_intermediate(frequency_t* tree);

int intermediate_node_extractor_init(intermediate_node_extractor_t* extractor, intermediate_node_t** node);
int intermediate_node_extractor_push(intermediate_node_extractor_t* extractor, intermediate_node_t** node);
void* intermediate_node_extractor_pop(intermediate_node_extractor_t* extractor);

#endif //INTERMEDIATE_H