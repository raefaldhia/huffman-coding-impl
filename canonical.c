#include "canonical.h"

#include <assert.h>
#include <stdlib.h>

#include "tree.h"
#include "huffman.h"
#include "intermediate.h"

void canonical_push(canonical_t* tree, canonical_node_t* node) {
	assert(tree != NULL);
	assert(node != NULL);
	assert(node != NULL_NODE);

	canonical_node_t
		**location = &tree->head;
	
	/* find suitable location to put the character */
	while (*location != NULL_NODE) {
		if (node->code_length < (*location)->code_length) {
			break;
		}
		location = &(*location)->next;
	}	
	node->next = *location;
	*location  = node;
}

tree_retval_t canonical_pushr(canonical_t* tree, char character, size_t code_length) {
	assert(tree != NULL);

	canonical_node_t
		*node = canonical_node_create(character, code_length);
	
	if (node == NULL_NODE) {
		return NMEMORY;
	}
	canonical_push(tree, node);

	return OK;
}

void canonical_gencode(canonical_t* tree) {
	assert(tree != NULL);

	canonical_node_t
		*curr = tree->head;

	size_t	
		len  = 0;
	uint16_t
		code = 0;

	while (curr != NULL_NODE) {
		code <<= (curr->code_length - len);
		curr->code = code;

		len   = curr->code_length;
		code += 1;
		
		curr = curr->next;
	}
}

int canonical_getcode(canonical_t* tree, char letter, uint16_t* code, size_t* code_length) {
	canonical_node_t
		*current = tree->head;
	
	while(current != NULL_NODE){
		if(current->character == letter){
			*code 		  = current->code;
			*code_length += current->code_length;
			
			return current->code_length;
		}
		current=current->next;
	}
	return 0;
}
canonical_node_t* canonical_node_create(char character, size_t code_length) {
	canonical_node_t
		*node = malloc(sizeof(canonical_node_t));

	if (node == NULL) {
		return NULL_NODE;
	}

	node->character   = character;
	node->code_length = code_length;
	node->next        = NULL_NODE;
	
	return node;
}

void intermediate_reinit_canonical(intermediate_t* tree) {
	assert(tree != NULL);

	huffman_node_t
		*node;
	
	/* delete the carrier */
	node = tree->head;
	tree->head = node->left;
	free(node);

	node_extractor_t node_extractor;
	huffman_node_extractor_init(&node_extractor, tree->head);

	tree->head = NULL_NODE;

	while ((node = huffman_node_extractor_pop(&node_extractor)) != NULL_NODE) {
		if (!node->is_chain) {
			/* shrink and push the node to the canonical tree */
			canonical_node_t
				*canonical_node = realloc(node, sizeof(canonical_node_t));

			canonical_push(tree, canonical_node);
		} else {
			free(node); // chain not used anymore in canonical
		}
	}
	canonical_gencode(tree);
}
