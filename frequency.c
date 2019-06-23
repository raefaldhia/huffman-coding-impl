#include "frequency.h"

#include <assert.h>
#include <stdlib.h>

#include "counter.h"

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

		
		if(node->frequency > frequency){
		    node->right = frequency_push(node->right, character, frequency);
		}
		else if(node->frequency <= frequency){
			//based on counter_reinit_frequency when letter has a same frequency push it to the left node
			node->left = frequency_push(node->left, character, frequency);
		}
		

}

void counter_reinit_frequency(tree_t* tree) {
    assert(tree != NULL);

    frequency_node_t 
        *node,
        **location;

    node_extractor_t extractor;
    node_extractor_init(&extractor, tree->head);

    /* mark as empty to reinitialize the tree */ 
    tree->head = NULL_NODE;
    while ((node = node_extractor_get(&extractor)) != NULL_NODE) {
        node->_reserved = 0; // reset to 0, required for huffman_itermediate to work

        /* insert to a new empty tree in BST way based on the frequency */
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
