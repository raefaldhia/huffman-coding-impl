#include "huffman.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "intermediate.h"
#include "canonical.h"

void huffman_node_extractor_init(huffman_node_extractor_t* node_extractor, huffman_node_t* node) {
	assert(node_extractor != NULL);
	assert(node != NULL);
	assert(node != NULL_NODE);

	node_extractor->stack = NULL_NODE;
	huffman_node_extractor_push(node_extractor, node, 0);
}

void huffman_node_extractor_push(huffman_node_extractor_t* node_extractor, huffman_node_t* node, int level) {
	assert(node_extractor != NULL);
	assert(node != NULL);

	while (node != NULL_NODE) {
		if (node->is_chain == 1) {
			node->left 		  = node->right->left;
			node->right->left = NULL_NODE;
		}
		node->code_length = level++;

		node_t* 
			stacked = (node_t*)node;    

		node 				  = node->left;
		stacked->left 		  = node_extractor->stack;
		node_extractor->stack = stacked;
	}
}

huffman_node_t* huffman_node_extractor_pop(huffman_node_extractor_t* node_extractor) {
	assert(node_extractor != NULL);

	huffman_node_t
		*retval = NULL_NODE;

	// transversing iterative inorder and disasemble visited node
	if (node_extractor->stack != NULL_NODE) {
		huffman_node_t
			*right  = NULL_NODE;
		
		// pop from stack
		retval = (huffman_node_t*)node_extractor->stack;
		right  = retval->right;
		node_extractor->stack = node_extractor->stack->left;
	
		// clear the extracted node
		retval->right = NULL_NODE;
		retval->left  = NULL_NODE;

		// push right child to the stack
		huffman_node_extractor_push(node_extractor, right, retval->code_length + 1);
	}
	return retval;
}

void display_huffman_code(canonical_t* tree){
	canonical_node_t* current = tree->head;
	int i;
	printf("===============================================================");
	printf("\n|\t\t    TABLE OF HUFFMAN CODE\t\t      |");
	printf("\n===============================================================");
	printf("\nCharacter\t\tCode Length\t\tHuffman Code  |");
	printf("\n===============================================================");
	
	while(current != NULL_NODE){
		printf("\n   %c\t\t\t    %d \t\t\t   ", current->character, current->code_length);
		print_code(current->code, current->code_length);
		printf("\t      |");
		current = current->next;
		printf("\n---------------------------------------------------------------");
	}
}

void print_code(uint8_t code, int code_length){
	int i;
	
	for (i = 1; i <= code_length; i++) {
			printf("%u", (code >> (code_length - i)) & 1);
		}
}

void encode(tree_t tree, char* string_input, int* total_length){
	int i,string_length, code_length;
	uint8_t code;
	
	string_length = strlen(string_input);
	
	for(i=0; i<string_length; i++){
		code_length = get_code(tree, string_input[i], &code, &(*total_length));
		
		if(code_length != 0){
			print_code(code, code_length);
		}
	}	
}

int get_code(tree_t tree, char letter, uint8_t* code, int* total_length){
	canonical_node_t* current = (canonical_node_t*)tree.head;
	
	while(current != NULL_NODE){
		if(current->character == letter){
			*code = current->code;
			*total_length += current->code_length;
			return current->code_length;
		}
		current=current->next;
	}
	return 0;
}

void decode(){
	
}

//char get_character(tree_t* tree, ){
//	huffman_node_t* current_position = (huffman_node_t*)tree.head;
//}
