#ifndef COUNTER_H
#define COUNTER_H

#include <stddef.h>
#include <stdint.h>

#include "tree.h"

/* Every field have to be same size as described in node_t (tree.h) */
typedef struct counter_node_s {
	char character;
	size_t count;
	uint16_t height;
	struct counter_node_s* right;
	struct counter_node_s* left;
} counter_node_t;

typedef enum {
	LEFT  = -1,
	EQUAL =  0,
	RIGHT =  1
} balance_factor_t;

void generate_counter_tree(tree_t* tree, char* string_input);
int counter_push(tree_t* tree, char character);

void counter_node_height_calculate(counter_node_t* node);
counter_node_t* counter_node_rotate_left(counter_node_t* node);
counter_node_t* counter_node_rotate_right(counter_node_t* node);
balance_factor_t counter_node_balance_factor(counter_node_t* node);

#endif //COUNTER_H
