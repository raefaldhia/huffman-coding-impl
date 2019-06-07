#include "counter.h"

#include <assert.h>
#include <stdlib.h>

#include "stack.h"
#include "util.h"

int counter_push(tree_t* tree, char character) {
	assert(tree != NULL);

	stack_t call_stack;
	stack_init(&call_stack, sizeof(counter_node_t**));
	if (stack_push(&call_stack, &tree->head)) {
		stack_delete(&call_stack);
		return 1;
	}
	
	counter_node_t** location;
	while ((*(location = (counter_node_t**)stack_top(&call_stack))) != NULL_NODE) {
		int NMEMORY = 0;
		if (character < (*location)->character) {
			NMEMORY = stack_push(&call_stack, &(*location)->left);
		} else if (character > (*location)->character) {
			NMEMORY = stack_push(&call_stack, &(*location)->right);
		} else {
			(*location)->count += 1;
			stack_delete(&call_stack);
			return 0;
		}

		if (NMEMORY) { 
			stack_delete(&call_stack);
			return NMEMORY; 
		}
	}

	counter_node_t* node = malloc(sizeof(counter_node_t));
	if (node == NULL) {
		return 1;
	}

	node->character = character;
	node->count     = 1;
	node->height	= 1;
	node->left     	= NULL_NODE;
	node->right     = NULL_NODE;

	(*location) = node;
	stack_pop(&call_stack);

	while ((location = (counter_node_t**)stack_top(&call_stack)) != NULL) {
		counter_node_height_calculate(*location);

		balance_factor_t balance = counter_node_balance_factor(*location);
		if (balance < LEFT) {
			if (counter_node_balance_factor((*location)->left) > EQUAL) {
				(*location)->left = counter_node_rotate_left((*location)->left);
			}
			*location = counter_node_rotate_right(*location);
		} else if (balance > RIGHT) {
			if (counter_node_balance_factor((*location)->right) < EQUAL) {
				(*location)->right = counter_node_rotate_right((*location)->right);
			}
			*location = counter_node_rotate_left((*location));
		}
		stack_pop(&call_stack);
	}

	stack_delete(&call_stack);
	return 0;
}

void counter_node_height_calculate(counter_node_t* node) {
	assert(node != NULL_NODE);

	node->height = max(node->left->height, node->right->height) + 1;
}

counter_node_t* counter_node_rotate_left(counter_node_t* node) {
	assert(node != NULL_NODE);

	counter_node_t* root = node->right;	

	node->right = root->left;
	root->left = node;

	counter_node_height_calculate(node);
	counter_node_height_calculate(root);

	return root;
}

counter_node_t* counter_node_rotate_right(counter_node_t* node) {
	assert(node != NULL_NODE);

	counter_node_t* root = node->left;

	node->left = root->right;
	root->right = node;

	counter_node_height_calculate(node);
	counter_node_height_calculate(root);

	return root;
}

balance_factor_t counter_node_balance_factor(counter_node_t* node) {
	assert(node != NULL_NODE);

	return (node->right->height - node->left->height);
}