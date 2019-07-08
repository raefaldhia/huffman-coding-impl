#ifndef CANONICAL_H
#define CANONICAL_H

#include <stddef.h>
#include <stdint.h>

#include "huffman.h"
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

int canonical_push(canonical_t* tree, size_t code_length, char code);
void canonical_huffman_push(canonical_t* tree, huffman_node_t* node);
void intermediate_reinit_canonical(intermediate_t* tree);
void canonical_code_generate(canonical_t* tree);

//void huffman_reinit_canonical

#endif // CANONICAL_H