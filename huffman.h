#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tree.h"

typedef struct intermediate_node_s {
	char character;
	size_t code_length;
	bool is_chain;
	struct intermediate_node_s* right;
	struct intermediate_node_s* left;
} intermediate_node_t;

typedef struct huffman_node_s {
	char character;
	size_t code_length;
	uint8_t code;
	struct huffman_node_s* next;
} huffman_node_t;

void huffman_push(tree_t* tree, char character, int code_length);

void frequency_reinit_huffman(tree_t* tree);
void intermediate_reinit_huffman(tree_t* tree);

void intermediate_push(tree_t* tree, intermediate_node_t** nodeptr);
void intermediate_push_node(tree_t* tree, intermediate_node_t* node);
void intermediate_node_extractor_init(node_extractor_t* node_extractor, intermediate_node_t* node);
void intermediate_node_extractor_push(node_extractor_t* node_extractor, intermediate_node_t* node, int level);
intermediate_node_t* intermediate_node_extractor_get(node_extractor_t* node_extractor);

void display_huffman_code(tree_t tree);
void print_code(huffman_node_t* node);

void encode(tree_t tree, char* string_input);
void decode();
#endif // HUFFMAN_H
