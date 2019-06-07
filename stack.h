#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct stack_s {
    void** head;
    size_t next;
    size_t capacity;
    size_t element_size;
} stack_t;

void stack_init(stack_t* stack, size_t element_size);
int stack_push(stack_t* stack, void* content);
void stack_pop(stack_t* stack);
void* stack_top(stack_t* stack);
void stack_delete(stack_t* stack);

#endif // STACK_H