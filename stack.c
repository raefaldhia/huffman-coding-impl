#include "stack.h"

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

void stack_init(stack_t* stack, size_t element_size) {
    stack->head = NULL;
    stack->next = 0;
    stack->capacity = 0;
    stack->element_size = element_size;
}

int stack_push(stack_t* stack, void* content) {
    stack->next += 1;
    if (stack->next > stack->capacity) {
        stack->capacity = stack->next * stack->next;
        
        void** address = (void**)realloc(stack->head, stack->element_size * stack->capacity);
        if (address == NULL) {
            return 1;    
        }
        stack->head = address;
    }
    stack->head[stack->next - 1] = content;

    return 0;
}

void stack_pop(stack_t* stack) {  
    stack->next    -= 1;
    stack->capacity = stack->capacity - 1;
    
    stack->head = (void**)realloc(stack->head, stack->element_size * stack->capacity);
}

void* stack_top(stack_t* stack) {
    if (!stack->next) {
        return NULL;
    }
    return stack->head[stack->next - 1];
}

void stack_delete(stack_t* stack) {
    free(stack->head);
}