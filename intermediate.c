#include "intermediate.h"

#include <assert.h>

void intermediate_push(intermediate_t* tree, intermediate_node_t* node) {
	assert(tree != NULL);
	assert(node != NULL);
	assert(node != NULL_NODE);

	intermediate_node_t 
		**find = &tree->head;

	/* find empty place to store pushed node */
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
	assert(node != NULL);

	intermediate_node_t
		*chain;
	
	/* reuse node's carrier as chain if the node is a carrier */
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
	assert(first != NULL);
	assert(second != NULL);

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
	assert(node != NULL);
	assert(*node != NULL_NODE);

	stack_init(&extractor->stack, sizeof(intermediate_node_t**));
	if (intermediate_node_extractor_push(extractor, node) != 0) {
		return 1;
	}
	return 0;
}

int intermediate_node_extractor_push(intermediate_node_extractor_t* extractor, intermediate_node_t** node) {
	assert(extractor != NULL);
	assert(node != NULL);
	assert(*node != NULL_NODE);

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

	intermediate_node_t
		**node = stack_top(&extractor->stack);
	
	if (node != NULL) {
		/* if node is a carrier there's a chance to have a left child that's not
		 * pushed onto the stack (more analysis required since it's prone to bug) 
		 */
		if ((*node)->left->is_chain && 
			(*node)->left->left != NULL_NODE) {
			node = &(*node)->left->left;
		} else if ((*node)->left->is_chain == false &&
				   (*node)->left != NULL_NODE) {
			node = &(*node)->left;
		} else {
			stack_pop(&extractor->stack);            
		}

		intermediate_node_t
			*retval = *node;

		/* push the right child onto the stack if it exist */
		*node = (*node)->right;
		if (*node != NULL_NODE) {
			intermediate_node_extractor_push(extractor, node);
		}     
		retval->right = NULL_NODE;

		return retval;
	}
	return NULL_NODE;
}