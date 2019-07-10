#include "tree.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "canonical.h"
#include "counter.h"
#include "frequency.h"
#include "intermediate.h"
#include "util.h"
#include "stream.h"
int main() {
	FILE* fhandler, *in, *out; 
	tree_t tree;
	int choice, i, quantity, frequency, total_length, str_length;
	char letter;
	bool end;
	bool finished=false;
	
	char *file_input_name = malloc(20* sizeof(char));
	char *file_output_name = malloc(20* sizeof(char));
	
	while(!finished){
		tree_init(&tree);
		printf("===Generate Huffman Code===\n\n");
		printf("1 : Based on Program ('HUFFMAN CODING')\n");
		printf("2 : Based on Input (String)\n");
		printf("3 : Based on Input (Frequency)\n");
		printf("4 : From File\n");
		printf("5 : Exit Program\n");
		printf("Choice : ");
		scanf("%d", &choice);
		
		switch(choice){
			case 1 :
				generate_counter_tree(&tree, "HUFFMAN CODING");
				counter_reinit_frequency((counter_t*)&tree);
				frequency_push((frequency_node_t*)tree.head, '\0', 1);
			break;
			
			case 2 :
				printf("String : ");
				clearstdin();
				stream_counter_read(stdin, (counter_t*)&tree);
				counter_reinit_frequency((counter_t*)&tree);
				frequency_push((frequency_node_t*)tree.head, '\0', 1);
			break;
			
			case 3 :
				frequency_push((frequency_node_t*)tree.head, '\0', 1);
				printf("How Many Letters : ");
				scanf("%d", &quantity);
				
				for(i=0; i<quantity; i++){
					printf("Letter    :");scanf(" %c", &letter);
					printf("Frequency :");scanf("%d", &frequency);
					tree.head = frequency_push((frequency_node_t*)tree.head, letter, frequency);				
				}
			break;
			
			case 4 :
				printf("Ensure the File is within the same folder and have a .txt extension\n");
				printf("File Name : ");
				clearstdin();
				gets(file_input_name);
				
				//Make counter tree from input.txt file
				fhandler = fopen(file_input_name , "r");
				stream_counter_read(fhandler, &tree);
				fclose(fhandler);
				
				//Change tree from counter tree to frequency tree
				counter_reinit_frequency((counter_t*)&tree);
				frequency_push((frequency_node_t*)tree.head, '\0', 1);
			break;
			
			case 5 :
				return 0;
			break;
			
			default:
			
			break;		
		}
		frequency_reinit_intermediate((intermediate_t*)&tree);
		intermediate_reinit_canonical(&tree);
		
		//write huffman code to a file
		fhandler = fopen("huffman_table.dat", "wb");
		stream_canonical_write(fhandler, &tree);
		fclose(fhandler);
		
		printf("Huffman code generated...");
		getch();
		
		end = false;
		
		while(!end){
			system("cls");
			printf("===Main Menu===\n\n");
			printf("1 : Table of Huffman Code\n");
			printf("2 : Encode (Input to File)\n");
			printf("3 : Decode (Input to File)\n");
			printf("4 : Encode (File to File)\n");
			printf("5 : Decode (File to File)\n");
			printf("6 : Exit Menu\n");
			printf("Choice : ");scanf("%d", &choice);
			
			switch(choice){
				case 1 :
					display_huffman_code((canonical_t*)&tree);
					getch();	
				break;
				
				case 2 :
					printf("Output File Name : ");
					gets(file_output_name);
					printf("String Input : ");
					fhandler = fopen(file_output_name, "w");
					clearstdin();
					stream_encode(stdin, fhandler, &tree);
					fclose(fhandler);
					
					printf("File %s is generated", file_output_name);
					printf("Press any key to continue...");
					getch();
				break;
				
				case 3 :
					printf("Output File Name : ");
					gets(file_output_name);
					printf("Code : ");
					fhandler = fopen(file_output_name, "w");
					clearstdin();
					stream_decode(stdin, fhandler);
					fclose(fhandler);
					
					printf("File %s is generated", file_output_name);
					printf("Press any key to continue...");
					getch();
				break;
				
				case 4:
					printf("Input File Name : ");
					gets(file_input_name);
					printf("Output File Name : ");
					gets(file_output_name);
					
					in = fopen(file_input_name, "r");
					out = fopen(file_output_name, "w");
					
					stream_encode(in, out, &tree);
					
					fclose(in);
					fclose(out);
					
					printf("File %s is generated", file_output_name);
					printf("Press any key to continue...");
					getch();
				break;
				
				case 5:
					printf("Input File Name : ");
					gets(file_input_name);
					printf("Output File Name : ");
					gets(file_output_name);
					
					in = fopen(file_input_name, "r");
					out = fopen(file_output_name, "w");
					
					stream_decode(in, out);
					
					fclose(in);
					fclose(out);
					
					printf("File %s is generated", file_output_name);
					printf("Press any key to continue...");
					getch();
				break;
				case 6 :
					end=true;
				break;
			}
		}
	
		tree_close(&tree);
	}

	return 0;
}

