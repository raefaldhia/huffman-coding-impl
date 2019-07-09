#include "tree.h"

#include <assert.h>
#include <stdlib.h>

#include "util.h"

node_t null_node = {'\0', 0, 0, &null_node, &null_node};

void tree_init(tree_t *tree) {
	assert(tree != NULL);

	tree->head = NULL_NODE;
}

void tree_close(tree_t *tree) {
	assert(tree != NULL);

	node_extractor_t node_extractor;
	node_extractor_init(&node_extractor, tree->head);

	node_t
		*node;
		
	while ((node = node_extractor_pop(&node_extractor)) != NULL_NODE) {
		free(node);
	}
	tree->head = NULL_NODE;
}

void node_extractor_init(node_extractor_t* node_extractor, node_t* node) {
	assert(node_extractor != NULL);
	assert(node != NULL);

	node_extractor->stack = NULL_NODE;
	node_extractor_push(node_extractor, node);
}

void node_extractor_push(node_extractor_t* node_extractor, node_t* node) {
	assert(node_extractor != NULL);
	assert(node != NULL);

	while (node != NULL_NODE) {
		node_t
			*stacked = node;

		// move to the next left child
		node = node->left;

		// push to the stack
		stacked->left = node_extractor->stack;
		node_extractor->stack = stacked;
	}
}

void* node_extractor_pop(node_extractor_t* node_extractor) {
	assert(node_extractor != NULL);
	
	// pop from the stack
	node_t
		*node = node_extractor->stack;

	node_extractor->stack = node_extractor->stack->left;
	
	if (node != NULL_NODE) {
		node_t 
			*right  = node->right;

		// clear the extracted node
		node->right = NULL_NODE;
		node->left  = NULL_NODE;

		// push the right child to the stack
		node_extractor_push(node_extractor, right);
	}
	return node;
}