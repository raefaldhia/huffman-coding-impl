#include "tree.h"
#include "counter.h"
#include "frequency.h"
#include "huffman.h"

#include <stdio.h>
#include <stdint.h>

int main() {
	tree_t tree;
	tree_init(&tree);

	counter_push(&tree, 'H');
	counter_push(&tree, 'U');
	counter_push(&tree, 'F');
	counter_push(&tree, 'F');
	counter_push(&tree, 'M');
	counter_push(&tree, 'A');
	counter_push(&tree, 'N');
	counter_push(&tree, ' ');
	counter_push(&tree, 'C');
	counter_push(&tree, 'O');
	counter_push(&tree, 'D');
	counter_push(&tree, 'I');
	counter_push(&tree, 'N');
	counter_push(&tree, 'G'); 

	counter_reinit_frequency(&tree);
	frequency_reinit_huffman(&tree);

	huffman_node_t* curr = (huffman_node_t*)tree.head;
	while (curr != NULL_NODE) {
		printf("%c\t:\t ", curr->character);

		for (int i = 1; i <= curr->code_length; i++) {
			printf("%u", (curr->code >> (curr->code_length - i)) & 1);
		}
		printf("\n");

		curr = curr->next;
	}

	tree_close(&tree);

	return 0;
}

