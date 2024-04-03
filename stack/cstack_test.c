#include  <stddef.h>
#include  <stdio.h>
#include  <stdint.h>
#include  <stdlib.h>
#include  <string.h>

#include  "cstack.h"

static void test_print(cstack *stack) {
    void *it = NULL;
    printf("%lld %lld %d\n", stack->size(stack), stack->typesize(stack), stack->empty(stack));
    while(!stack->empty(stack)) {
        it = stack->top(stack);
    	int value = *((int*)it);
        stack->pop(stack);
    	printf("%x ", value);
    }
    printf("\n");    
}

static void test_stack1();

static void test_stack2();

int main(int argc, const char *argv[]) {
	test_stack1();
	test_stack2();
	return 0;
}

void test_stack1() {
    int buf[] = {0x01, 0x12, 0x23, 0x34, 0x45};
    cstack *stack  = cstack_alloc(sizeof(int));
    cstack *stack1 = cstack_alloc(sizeof(int)); 
    for (int i = 0 ; i < 5; ++i) {
        stack->push(stack, &buf[i]);
    }
    stack->copy(stack, stack1);
    printf("%d\n", stack1->equal(stack1, stack));
    test_print(stack);

    stack->clear(stack);
    test_print(stack);
    stack->free(stack);

    stack1->clear(stack1);
    test_print(stack1);
    stack1->free(stack1);
}

void test_stack2() {

}