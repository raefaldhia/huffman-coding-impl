#include "stream.h"

#include <assert.h>
#include <stdio.h>

#include "canonical.h"
#include "counter.h"

void stream_encode(FILE* in, FILE* out, canonical_t* tree) {
	assert(in   != NULL);
	assert(out  != NULL);
	assert(tree != NULL);

	stream_canonical_write(out, tree);

	size_t 
		zero = 0;
	fwrite(&zero, sizeof(size_t), 1, out);

	uint16_t 
		block;
	size_t 
		block_length;
	char 
		character;
	int
		retval;

	block_length = 0;
	while (fread(&character, sizeof(char), 1, in) != 0) {
 		retval = stream_encode_write(out, tree, character, &block, &block_length);
		if (retval != 0) {
			break;
		}
	}
	retval = stream_encode_write(out, tree, '\0', &block, &block_length);
	if (retval != 0) {
		return;
	}
	if (block_length) {
		block <<= 16 - block_length;
		fwrite(&block, sizeof(uint16_t), 1, out);
	}
}
int stream_encode_write(FILE* out, canonical_t* tree, char character, uint16_t* block, size_t* block_length) {
	assert(tree 		!= NULL);
	assert(out 			!= NULL);
	assert(block 		!= NULL);
	assert(block_length != NULL);

	uint16_t 
		code = 0;
	size_t
		code_length = 0;
	int 
		retval = canonical_getcode(tree, character, &code, &code_length);

	if(retval == 0) {
		return 1;
	}

	while (code_length) {
		if (*block_length == 16) {
			fwrite(block, sizeof(uint16_t), 1, out);
			
			*block        = 0;
			*block_length = 0;
		}
		code_length -= 1;

		*block <<= 1;
		*block  |= (code >> (code_length)) & 1;
		*block_length += 1;
	}

	return 0;
}

void stream_decode(FILE* in, FILE* out) {
	assert(in  != NULL);
	assert(out != NULL);

	canonical_t tree;
	tree.head = NULL_NODE;

	stream_canonical_get(in, &tree);

	canonical_node_t
		**location = &tree.head;

	uint16_t 
		block = 0,
		code  = 0;
	size_t 
		code_len  = 0,
		block_len = 0;

	read:
	while (fread(&block, sizeof(uint16_t), 1, in) != 0) {
		block_len = 0;

		while (true) {
			while (code_len != (*location)->code_length) {
				if (block_len == 16) {
					goto read;
				}

				code     <<= 1;
				code      |= (block >> (15 - block_len) & 1);
				code_len  += 1;
				block_len +=1;
			}

			int 
				jmp = code - (*location)->code;
				
			while (jmp) {
				if ((*location)->next == NULL_NODE) {
					return;
				}
				location = &(*location)->next;
				jmp--;
			}
			
			if (code_len == (*location)->code_length) {	
				if ((*location)->character == '\0') {
					return;
				}
				fwrite(&(*location)->character, sizeof(char), 1, out);
				code     = 0;
				code_len = 0;
				location = &tree.head;
			}
		}
	}
}

void stream_counter_read(FILE* in, counter_t* tree) {
	assert(tree != NULL);
	assert(in 	!= NULL);
	
	char 
		stop,
		character;

	if (in == stdin) {
		stop = '\n';
	} else {
		stop = '\0';
	}

	while (fread(&character,sizeof(char), 1, in) != 0) {
		if (character == stop || character == '\0') {
			return;
		}
		counter_push(tree, character);
	}
}

void stream_canonical_write(FILE* out, canonical_t* tree) {
	assert(out != NULL);
	assert(tree != NULL);

	canonical_node_t
		*canonical_item = tree->head;

	while (canonical_item != NULL_NODE) {
		fwrite(&canonical_item->code_length, sizeof(size_t), 1, out);
		fwrite(&canonical_item->character, sizeof(char), 1, out);
		canonical_item = canonical_item->next;
	}
}

void stream_canonical_get(FILE* in, canonical_t* tree) {
	assert(in   != NULL);
	assert(tree != NULL);

	size_t
		code_len;
	char
		character;
	while (fread(&code_len, sizeof(size_t), 1, in) != 0){
		if (code_len == 0) {
			break;
		}
		if (fread(&character, sizeof(char), 1, in) == 0) {
			return;
		}
		canonical_pushr(tree, character, code_len);
	}
	canonical_gencode(tree);
}