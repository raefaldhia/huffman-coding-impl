#include "intermediate.h"

#include <assert.h>

void intermediate_push(intermediate_t* tree, intermediate_node_t* node) {
    intermediate_node_t** find = &tree->head;
	while (*find != NULL_NODE) {
    	if ((*find)->is_chain || 
            node->weight <= (*find)->weight) {
			find = &(*find)->left;
    	} else {
		    find = &(*find)->right;
	    }
	}
	*find = node;
}

intermediate_node_t* intermediate_chain_create(intermediate_node_t** node) {
    intermediate_node_t* chain;
    if ((*node)->left != NULL_NODE) {
        chain = (*node);
        *node = (*node)->left;
    } else {
        chain = (intermediate_node_t*)malloc(sizeof(intermediate_node_t));
        if (chain == NULL) {
            return NULL_NODE;
        }
    }
    return chain;
}

intermediate_node_t* intermediate_carrier_create(intermediate_node_t* first, intermediate_node_t* second) {
    intermediate_node_t
        *chain   = intermediate_chain_create(&first),
        *carrier = intermediate_chain_create(&second);
    
    if (chain   == NULL_NODE ||
        carrier == NULL_NODE) {
            return NULL_NODE;
    }

    chain->weight      = first->weight + second->weight;
    chain->right       = second;
    chain->right->left = first;
    chain->left        = NULL_NODE;
    chain->is_chain    = 1;
    
    carrier->weight   = chain->weight;
    carrier->is_chain = 0;
    carrier->left     = chain;
    carrier->right    = NULL_NODE;

    return carrier;
}

tree_retval_t frequency_reinit_intermediate(frequency_t* tree) {
    assert(tree != NULL);

    intermediate_node_extractor_t extractor;
    intermediate_node_extractor_init(&extractor, (intermediate_node_t*)&tree->head);

    intermediate_node_t
        *first,
        *second,
        *carrier;
        
    do {
        first  = intermediate_node_extractor_pop(&extractor);
        second = intermediate_node_extractor_pop(&extractor);

        carrier = intermediate_carrier_create(first, second);
        if (carrier == NULL_NODE) {
            return NMEMORY;
        }
        intermediate_push(tree, carrier);
    } while (second != NULL_NODE);
    
    return OK;
}

int intermediate_node_extractor_init(intermediate_node_extractor_t* extractor, intermediate_node_t** node) {
    assert(extractor != NULL);

    stack_init(&extractor->stack, sizeof(intermediate_node_t**));
    if (intermediate_node_extractor_push(extractor, node) != 0) {
        return 1;
    }
    return 0;
}

int intermediate_node_extractor_push(intermediate_node_extractor_t* extractor, intermediate_node_t** node) {
    assert(extractor != NULL);

    while (*node != NULL_NODE) {
        if (stack_push(&extractor->stack, node) != 0) {
            return 1;
        }

        if ((*node)->left->is_chain) {
            node = &(*node)->left->left;
        } else {
            node = &(*node)->left;
        }
    }
    return 0;
}

void* intermediate_node_extractor_pop(intermediate_node_extractor_t* extractor) {
    assert(extractor != NULL);

    intermediate_node_t** node = stack_top(&extractor->stack);
    if (node != NULL) {
        if ((*node)->left->is_chain && 
            (*node)->left->left != NULL_NODE) {
            node = &(*node)->left->left;
        } else {
            stack_pop(&extractor->stack);            
        }
        
        intermediate_node_t* retval = *node;

        *node = (*node)->right;
        if (*node != NULL_NODE) {
            intermediate_node_extractor_push(extractor, node);
        }
        
        if (!retval->left->is_chain) {
            retval->left  = NULL_NODE;
        }
        retval->right = NULL_NODE;

        return retval;
    }
    return NULL_NODE;
}