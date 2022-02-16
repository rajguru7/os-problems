#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

typedef int value_t;

typedef struct node { 
    value_t data; 
    struct node *next;
} Node;
Node *top; // top of stack
void push(value_t item) { 
    Node *old_node;
    Node *new_node;
    new_node = (Node*)malloc(sizeof(Node)); 
    new_node->data = item;
    do {
        old_node = top;
        new_node->next = old_node;
    } while (compare_and_swap(top,old_node,new_node) != 0);
    
}

value_t pop() { 
    Node *old_node; 
    Node *new_node;
    do {
        old_node = top;
        if (old_node == NULL)
        return -1;
        new_node = old_node->next;
    } while (_compare_and_swap(top,old_node,new_node) != 0); 
    return old_node->data;
}

int main() {
    // Node *top = NULL;
    push(5);
    push(10);
    push(15);
    printf("%d", top->data);
    int value = pop();
    printf("%d", top->data);
    value = pop();
    printf("%d", top->data);
    value = pop();
}