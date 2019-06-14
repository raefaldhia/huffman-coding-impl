#include "tree.h"
#include "counter.h"
#include "frequency.h"
#include "huffman.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
	tree_t tree;
	int choice, i, quantity, frequency;
	char* string_input;
	char letter;
	bool end=false;
	
	tree_init(&tree);
	printf("===Generate Huffman Code===\n\n");
	printf("1 : Based on Program ('HUFFMAN CODING')\n");
	printf("2 : Based on Input (String)\n");
	printf("3 : Based on Input (Frequency)\n");
	printf("Choice : ");
	scanf("%d", &choice);
	
	switch(choice){
		case 1 :
			generate_counter_tree(&tree, "HUFFMAN CODING");
			counter_reinit_frequency(&tree);
		break;
		
		case 2 :
			string_input=malloc(100*sizeof(char)); //not effective
			fflush(stdin);
			gets(string_input);
			generate_counter_tree(&tree, string_input);
			counter_reinit_frequency(&tree);
		break;
		
		case 3 :
			printf("How Many Letters : ");
			scanf("%d", &quantity);
			
			for(i=0; i<quantity; i++){
				
				printf("Letter    :");scanf(" %c", &letter);
				printf("Frequency :");scanf("%d", &frequency);
				frequency_push((frequency_node_t*)tree.head, letter, frequency);
				
			}
		break;
		
		default:
		
		break;		
	}
	frequency_reinit_huffman(&tree);
	generate_huffman_code(&tree);
	printf("Huffman code generated...");
	getch();
	
	while(!end){
		system("cls");
		printf("===Main Menu===\n\n");
		printf("1 : Table of Huffman Code\n");
		printf("2 : Conversion String to Huffman Code\n");
		printf("3 : \n");
		printf("4 : Exit\n");
		printf("Choice : ");scanf("%d", &choice);
		
		switch(choice){
			case 1 :
				display_huffman_code(tree);
				getch();	
			break;
			
			case 4 :
				end=true;
			break;
		}
	}

	tree_close(&tree);

	return 0;
}

