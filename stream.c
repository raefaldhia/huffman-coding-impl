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

	stream_encode_write(in, out, tree);
}

void stream_encode_write(FILE* in, FILE* out, canonical_t* tree) {
	stream_block_t
		block;
	
	block.tree = tree;
	block.size = 0;
	block.out  = out;

	char 
		character;
	int
		retval;

	while (fread(&character, sizeof(char), 1, in) != 0) {
 		retval = stream_encode_writeb(&block, character);
		if (retval != 0) {
			break;
		}
	}
	retval = stream_encode_writeb(&block, '\0');
	if (retval != 0) {
		return;
	}
	if (block.size) {
		block.data <<= 16 - block.size;
		fwrite(&block.data, sizeof(uint16_t), 1, out);
	}
}

int stream_encode_writeb(stream_block_t* block, char character) {
	assert(block != NULL);

	uint16_t 
		code = 0;
	size_t
		code_length = 0;

	if (canonical_getcode(block->tree, character, &code, &code_length) == 0) {
		return 1;
	}

	while (code_length) {
		if (block->size == 16) {
			fwrite(&block->data, sizeof(uint16_t), 1, block->out);
			
			block->data = 0;
			block->size = 0;
		}
		code_length -= 1;

		block->data <<= 1;
		block->data  |= (code >> (code_length)) & 1;
		block->size  += 1;
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
	assert(out  != NULL);
	assert(tree != NULL);

	canonical_node_t
		*node = tree->head;

	while (node != NULL_NODE) {
		fwrite(&node->code_length, sizeof(size_t), 1, out);
		fwrite(&node->character, sizeof(char), 1, out);
		node = node->next;
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