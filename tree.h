#ifndef TREE_H
#define TREE_H

#define NULL_NODE (void*)&null_node

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct node_s {
	char character;
	/* Variable name that used in various tree's ADT */
	union {
		size_t count; 	    // counter
		size_t frequency;   // frequency
		size_t code_length; // huffman (canonical)
	};
	union {
		uint16_t height;   // counter
		uint16_t is_chain; // huffman
		uint16_t code; 	   // huffman (canonical)
	};
	union {
		struct node_s* right;
		struct node_s* next;  // huffman (canonical)
	};
	// unused in huffman (canonical)
	struct node_s* left;
} node_t;

typedef struct tree_s {
    void* head;
} tree_t;

typedef struct node_extractor_s {
    node_t* stack;
} node_extractor_t;

void tree_init(tree_t* tree);
void tree_close(tree_t* tree);

void node_extractor_init(node_extractor_t* node_extractor, node_t* node);
void node_extractor_push(node_extractor_t* node_extractor, node_t* node);
void* node_extractor_get(node_extractor_t* node_extractor);

extern node_t null_node;

#endif // TREE_H