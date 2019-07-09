#ifndef CANONICAL_H
#define CANONICAL_H

#include <stddef.h>
#include <stdint.h>

#include "tree.h"
#include "intermediate.h"

/* Every field have to be same size as described in node_t (tree.h) */
typedef struct canonical_node_s {
	char character;
	size_t code_length;
	uint16_t code;
	struct canonical_node_s* next;
} canonical_node_t;

typedef struct canonical_s {
	canonical_node_t* head;
} canonical_t;

void canonical_push(canonical_t* tree, canonical_node_t* node);
tree_retval_t canonical_pushr(canonical_t* tree, char character, size_t code_length);
void canonical_gencode(canonical_t* tree);

int canonical_getcode(canonical_t* tree, char letter, uint16_t* code, size_t* code_length);

canonical_node_t* canonical_node_create(char character, size_t code_length);

void intermediate_reinit_canonical(intermediate_t* tree);

#endif // CANONICAL_H