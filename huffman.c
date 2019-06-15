#include "huffman.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void huffman_push(tree_t* tree, char character, int code_length) {

}

void frequency_reinit_huffman(tree_t* tree) {
    intermediate_push(tree, (intermediate_node_t**)&tree->head);
	intermediate_reinit_huffman(tree);
}

void intermediate_reinit_huffman(tree_t* tree) {
	assert(tree != NULL);

	/* delete the carrier */
	intermediate_node_t* node = tree->head;
	tree->head = ((intermediate_node_t*)tree->head)->left;
	free(node);
	
	node_extractor_t node_extractor;
	intermediate_node_extractor_init(&node_extractor, tree->head);

	tree->head = NULL_NODE;  // mark as empty tree for canonical form of huffman coding

	while ((node = intermediate_node_extractor_get(&node_extractor)) != NULL_NODE) {
		/* in canonical form of huffman coding the external node is not required anymore */
		if (node->is_chain) {
			free(node);
			continue;
		}

		/* push to the canonical tree */
		huffman_node_t** location = (huffman_node_t**)&tree->head;
		while (*location != NULL_NODE) {
			if (node->code_length < (*location)->code_length) {
				break;
			}
			location = &(*location)->next;
		}
		node = realloc(node, sizeof(huffman_node_t));
		
		((huffman_node_t*)node)->next = *location;
		*location = (huffman_node_t*)node;
	}

	huffman_node_t* curr = tree->head;

	size_t len  = 0;
	int8_t code = 0;

	while (curr != NULL_NODE) {
		code <<= (curr->code_length - len);
		curr->code = code;

		len   = curr->code_length;
		code += 1;
		
		curr = curr->next;
	}
}

void intermediate_push_node(tree_t* tree, intermediate_node_t* node) {
    assert(tree != NULL);

	intermediate_node_t** find = (intermediate_node_t**)&tree->head;
	while (*find != NULL_NODE) {
		if ((*find)->is_chain || node->code_length <= (*find)->code_length) {
			find = &(*find)->left;
		} else {
			find = &(*find)->right;
		}
	}

	*find = node;
}

void intermediate_push(tree_t* tree, intermediate_node_t** nodeptr) {
	intermediate_node_t* node = *nodeptr;

	if (node == NULL_NODE) {
		return;
	}

	// construct the left child
	intermediate_node_t** left;
	if (node->left->is_chain == 1 && node->left != NULL_NODE) {
		intermediate_push(tree, &node->left->left);
		left = &node->left->left;
	} else {
		intermediate_push(tree, &node->left);
		left = &node->left;
	}

	if ((*left) == NULL_NODE) {
		// rotate to left if node does have right child
		if (node->right != NULL_NODE) {
			intermediate_node_t* root = node->right;

			if (root->left->is_chain == 1 && root->left != NULL_NODE) {
				node->right = root->left->left;
				root->left->left  = node;
			} else {
				node->right = root->left;
				root->left  = node;
			}

			*nodeptr = root;

			// reconstruct new root
			intermediate_push(tree, nodeptr);
		}     
		return;
	}

	*nodeptr = node->right;
	node->right =  NULL_NODE;

	// create a chain between node->left and node
	intermediate_node_t
		*chain,
		*carrier;

	if ((*left)->left != NULL_NODE) {
		chain = *left;
		left  = &(*left)->left;
	} else {
		chain = (intermediate_node_t*)malloc(sizeof(intermediate_node_t));
	}
	
	if (node->left->is_chain == 1) {
		carrier = node;
		node = carrier->left;
	} else {
		carrier = (intermediate_node_t*)malloc(sizeof(intermediate_node_t));
	}

	chain->code_length = node->code_length + (*left)->code_length;
	chain->right = node;
	chain->right->left = *left;
	chain->left  = NULL_NODE;
	chain->is_chain = 1;

	carrier->code_length = chain->code_length;
	carrier->is_chain = 0;
	carrier->left = chain;
	carrier->right = NULL_NODE;

	intermediate_push_node(tree, carrier);

	// construct the right child
	intermediate_push(tree, nodeptr);
}

void intermediate_node_extractor_init(node_extractor_t* node_extractor, intermediate_node_t* node) {
	assert(node_extractor != NULL);

	node_extractor->stack = NULL_NODE;

	intermediate_node_extractor_push(node_extractor, node, 0);
}

void intermediate_node_extractor_push(node_extractor_t* node_extractor,  intermediate_node_t* node, int level) {
	while (node != NULL_NODE) {
		if (node->is_chain == 1) {
			node->left = node->right->left;
			node->right->left = NULL_NODE;
		}
		node->code_length = level++;
		node_t* stacked = (node_t*)node;            
		node = node->left;
		stacked->left = node_extractor->stack;
		node_extractor->stack = stacked;
	}
}

intermediate_node_t* intermediate_node_extractor_get(node_extractor_t* node_extractor) {
	intermediate_node_t* retval = NULL_NODE;

	// transversing iterative inorder and disasemble visited node
	if (node_extractor->stack != NULL_NODE) {
		intermediate_node_t* right  = NULL_NODE;
		
		// pop from stack
		retval = (intermediate_node_t*)node_extractor->stack;
		right  = retval->right;
		node_extractor->stack = node_extractor->stack->left;
	
		// clear the extracted node
		retval->right = NULL_NODE;
		retval->left  = NULL_NODE;

		// push right child to the stack
		intermediate_node_extractor_push(node_extractor, right, retval->code_length + 1);
	}

	return retval;
}

void display_huffman_code(tree_t tree){
	huffman_node_t* current = (huffman_node_t*)tree.head;
	int i;
	
	while(current != NULL_NODE){
		printf(" %c  : ", current->character);
		print_code(current);
		printf("\n");
		current = current->next;
	}
}

void print_code(huffman_node_t* node){
	int i;
	
	for (i = 1; i <= node->code_length; i++) {
			printf("%u", (node->code >> (node->code_length - i)) & 1);
		}
}

void encode(tree_t tree, char* string_input){
	int i,length;
	huffman_node_t* current;
	
	length = strlen(string_input);
	
	
	for(i=0; i<length; i++){
		current = (huffman_node_t*)tree.head;
		
		while(current!= NULL_NODE){
			if(string_input[i] == current->character){
				print_code(current);
				break;
			}
			current = current->next;
		}
	}	
}

void decode(){
	
}
