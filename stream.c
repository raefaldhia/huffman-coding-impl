#include <stdio.h>

#include "tree.h"
#include "canonical.h"
#include "counter.h"

void stream_counter_read(counter_t* tree, FILE* in) {
	char character;
	while (fread(&character,sizeof(char), 1, in) != 0) {
		if (character == '\n') {
			return;
		}
		counter_push(tree, character);
	}
}

void stream_decode(FILE* in, FILE* out) {
	canonical_t tree;
	tree.head = NULL_NODE;
	
	size_t code_len;
	char   character;
	while (fread(&code_len, sizeof(size_t), 1, in) != 0){
		if (code_len == 0) {
			break;
		}
		if (fwrite(&character, sizeof(char), 1, out) == 0) {
			return;
		}
		canonical_pushr(&tree, character, code_len);
	}

	uint16_t block;
	while (fread(&block, sizeof(uint16_t), 1, in) != 0) {
		uint16_t
			code;
		size_t 
			code_len = 0;

		canonical_node_t** location = &tree.head;
		while (*location != NULL_NODE) {
			while (code_len != (*location)->code_length) {
				code    <<= code_len;
				code     |= (block >> (15 - code_len));
				code_len += 1;
			}

			int jmp = code - (*location)->code;
			while (jmp) {
				if ((*location)->next == NULL_NODE) {
					return;
				}
				location = &(*location)->next;
			}

			if(code_len == (*location)->code_length) {
				fwrite((*location)->character, sizeof(uint16_t), 1, out);
				break;
			}
		}
	}
}
