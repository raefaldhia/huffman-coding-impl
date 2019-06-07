#include "frequency.h"

#include <assert.h>
#include <stdlib.h>

#include "counter.h"

void frequency_push(tree_t* tree, char character, int frequency) {

}

void counter_reinit_frequency(tree_t* tree) {
    assert(tree != NULL);

    frequency_node_t 
        *node,
        **location;

    node_extractor_t extractor;
    node_extractor_init(&extractor, tree->head);

    /* mark as empty to reinitialize the tree */ 
    tree->head = NULL_NODE;
    while ((node = node_extractor_get(&extractor)) != NULL_NODE) {
        node->_reserved = false; // reset to 0, required for huffman_itermediate to work

        /* insert to a new empty tree in BST way based on the frequency */
        location = (frequency_node_t**)&tree->head;
        while (*location != NULL_NODE) {
            if (node->frequency <= (*location)->frequency) {
                location = &(*location)->left;
            } else {
                location = &(*location)->right;
            }
        }
        *location = node;
    }
}