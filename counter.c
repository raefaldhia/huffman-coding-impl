#include "counter.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "util.h"

void generate_counter_tree(counter_t* tree, char* string_input){
	int i;
	int length = strlen(string_input);
	
	for(i=0; i<length; i++){
		counter_push(tree, string_input[i]);
	}
}

tree_retval_t counter_push(counter_t* tree, char character) {
	assert(tree != NULL);

	/* create a stack to store visited node */
	stack_t parent_stack;
	stack_init(&parent_stack, sizeof(counter_node_t**));
	
	/* find suitable place to put new character
	 * only add its count if the character already exist
	 */
	counter_node_t
		**location = &tree->head;
		
	while (*location != NULL_NODE) {
		if (stack_push(&parent_stack, location)) { 
			stack_delete(&parent_stack);
			return NMEMORY; 
		}

		if (character < (*location)->character) {
			location = &(*location)->left;
		} else if (character > (*location)->character) {
			location = &(*location)->right;
		} else {
			(*location)->count += 1;
			
			stack_delete(&parent_stack);
			return OK;
		}
	}

	/* create a node and put it in specified place */
	counter_node_t*
		node = counter_node_create(character);
	
	if (node == NULL_NODE) {
		return NMEMORY;
	}
	*location = node;

	/* rebalance the counter tree after added new node to the tree */
	while ((location = stack_top(&parent_stack)) != NULL) {
		counter_node_height_calculate(*location);

		balance_factor_t 
			balance = counter_node_balance_factor(*location);

		if (balance < LEFT) {
			if (counter_node_balance_factor((*location)->left) >= RIGHT) {
				(*location)->left = counter_node_rotate_left((*location)->left);
			}
			*location = counter_node_rotate_right(*location);
		} else if (balance > RIGHT) {
			if (counter_node_balance_factor((*location)->right) <= LEFT) {
				(*location)->right = counter_node_rotate_right((*location)->right);
			}
			*location = counter_node_rotate_left((*location));
		}
		stack_pop(&parent_stack);
	}
	stack_delete(&parent_stack);
	
	return OK;
}

counter_node_t* counter_node_create(char character) {
	counter_node_t
		*node = malloc(sizeof(counter_node_t));

	if (node == NULL) {
		return NULL_NODE;
	}
	
	node->character = character;
	node->count     = 1;
	node->height	= 1;
	node->left     	= NULL_NODE;
	node->right     = NULL_NODE;
	
	return node;
}

int counter_node_height_calculate(counter_node_t* node) {
	assert(node != NULL);
	assert(node != NULL_NODE);

	return max(node->left->height, node->right->height) + 1;
}

counter_node_t* counter_node_rotate_left(counter_node_t* node) {
	assert(node != NULL);
	assert(node != NULL_NODE);

	counter_node_t
		*root = node->right;	

	node->right = root->left;
	root->left  = node;

	node->height = counter_node_height_calculate(node);
	root->height = counter_node_height_calculate(root);

	return root;
}

counter_node_t* counter_node_rotate_right(counter_node_t* node) {
	assert(node != NULL);
	assert(node != NULL_NODE);

	counter_node_t
		*root = node->left;

	node->left  = root->right;
	root->right = node;

	node->height = counter_node_height_calculate(node);
	root->height = counter_node_height_calculate(root);

	return root;
}

balance_factor_t counter_node_balance_factor(counter_node_t* node) {
	assert(node != NULL);
	assert(node != NULL_NODE);

	return (node->right->height - node->left->height);
}


