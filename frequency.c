#include "frequency.h"

#include <assert.h>
#include <stdlib.h>

frequency_node_t* frequency_push(frequency_node_t* node, char character, int frequency) {
		if(node==NULL_NODE){
			frequency_node_t* newNode = malloc(sizeof(frequency_node_t));
			newNode->character = character;
			newNode->_reserved = 0; // reset to 0, required for huffman_itermediate to work
			newNode->frequency = frequency;
			newNode->left = NULL_NODE;
			newNode->right = NULL_NODE;
			return newNode;
		}
		
		if(node->frequency <= frequency){
			node->right = frequency_push(node->right, character, frequency);
		} else {
			//based on counter_reinit_frequency when letter has a same frequency push it to the left node
			node->left = frequency_push(node->left, character, frequency);
		}
}

void counter_reinit_frequency(counter_t* tree) {
	assert(tree != NULL);

	/* use node extractor to extract every node from the tree */
	node_extractor_t extractor;
	node_extractor_init(&extractor, (node_t*)tree->head);

	/* mark as empty to reinitialize the tree */ 
	tree->head = NULL_NODE;

	frequency_node_t 
		*node,
		**location;

	/* reinsert extracted node to the tree, order by frequency */
	while ((node = node_extractor_pop(&extractor)) != NULL_NODE) {
		node->_reserved = 0; // reset to 0 as it required for intermediate tree

		location = (frequency_node_t**)&tree->head;
		while (*location != NULL_NODE) {
			if (node->frequency <= (*location)->frequency) {
				location = &(*location)->left;
			} else {
				location = &(*location)->right;
			}
		}
		*location = node;
	}
}
