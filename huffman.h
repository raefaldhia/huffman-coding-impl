#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include <stddef.h>

#include "tree.h"
#include "canonical.h"

/* each field must have equal size as described in node_t (tree.h) */
typedef struct huffman_node_s {
	char character;
	size_t code_length;
	uint16_t is_chain;
	struct huffman_node_s* right;
	struct huffman_node_s* left;
} huffman_node_t;

typedef struct huffman_node_extractor_s {
	huffman_node_t* stack;
} huffman_node_extractor_t;

void display_huffman_code(canonical_t* tree);
void print_code(uint8_t code, int code_length);

void encode(tree_t tree, char* string_input, int* total_length);
int get_code(tree_t tree, char letter, uint8_t* code, int* total_length);

void huffman_node_extractor_init(huffman_node_extractor_t* node_extractor, huffman_node_t* node);
void huffman_node_extractor_push(huffman_node_extractor_t* node_extractor, huffman_node_t* node, int level);
huffman_node_t* huffman_node_extractor_pop(huffman_node_extractor_t* node_extractor);

#endif // HUFFMAN_H
