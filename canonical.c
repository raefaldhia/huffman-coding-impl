#include "canonical.h"

#include <assert.h>
#include <stdlib.h>

#include "tree.h"
#include "huffman.h"

#include "intermediate.h"
#include "huffman.h"

int canonical_push(canonical_t* tree, size_t code_length, char character) {
	canonical_node_t* node = malloc(sizeof(canonical_node_t));
	if (node == NULL) {
		return 1;
	}

	canonical_node_t** location = &tree->head;
	while (*location != NULL_NODE) {
	    if (node->code_length < (*location)->code_length) {
		    break;
		}
		location = &(*location)->next;
	}	
	node->next = *location;
	*location  = node;

	return 0;
}

void canonical_huffman_push(canonical_t* tree, huffman_node_t* node) {
	/* push to the canonical tree */
	canonical_node_t** location = &tree->head;
	while (*location != NULL_NODE) {
	    if (node->code_length < (*location)->code_length) {
		    break;
		}
		location = &(*location)->next;
	}

    /* create canonical node from huffman node and place the node in specified place */
    canonical_node_t* canonical_node = realloc(node, sizeof(canonical_node_t));
    canonical_node->next = *location;
	*location            = canonical_node;
}

void intermediate_reinit_canonical(intermediate_t* tree) {
    assert(tree != NULL);

    huffman_node_t*
        node;
    
    /* delete the carrier */
    node = tree->head;
	tree->head = node->left;
	free(node);

	node_extractor_t node_extractor;
	huffman_node_extractor_init(&node_extractor, tree->head);

	tree->head = NULL_NODE;  // mark as empty tree for canonical form of huffman coding

	while ((node = huffman_node_extractor_get(&node_extractor)) != NULL_NODE) {
		/* in canonical form of huffman coding the external node is not required anymore */
		if (node->is_chain) {
			free(node);
			continue;
		}

		/* push to the canonical tree */
        canonical_huffman_push(tree, node);
	}
    canonical_code_generate(tree);
}

void canonical_code_generate(canonical_t* tree) {
    canonical_node_t* curr = tree->head;

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