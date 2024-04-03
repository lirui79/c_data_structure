#include  <stddef.h>
#include  <stdio.h>
#include  <stdint.h>
#include  <stdlib.h>
#include  <string.h>

#include  "cdeque.h"

static void test_print_front(cdeque *queue) {
    void *it = NULL;
    printf("%lld %lld %d\n", queue->size(queue), queue->typesize(queue), queue->empty(queue));
    while(!queue->empty(queue)) {
        it = queue->front(queue);
    	int value = *((int*)it);
        queue->pop_front(queue);
    	printf("%x ", value);
    }
    printf("\n");    
}

static void test_print_back(cdeque *queue) {
    void *it = NULL;
    printf("%lld %lld %d\n", queue->size(queue), queue->typesize(queue), queue->empty(queue));
    while(!queue->empty(queue)) {
        it = queue->back(queue);
        int value = *((int*)it);
        queue->pop_back(queue);
        printf("%x ", value);
    }
    printf("\n");    
}

static void test_deque1();

static void test_deque2();

int main(int argc, const char *argv[]) {
	test_deque1();
	test_deque2();
	return 0;
}

void test_deque1() {
    int buf[] = {0x01, 0x12, 0x23, 0x34, 0x45};
    cdeque *queue  = cdeque_alloc(sizeof(int));
    cdeque *queue1 = cdeque_alloc(sizeof(int)); 
    for (int i = 0 ; i < 5; ++i) {
        queue->push_back(queue, &buf[i]);
    }
    queue->copy(queue, queue1);
    printf("%d\n", queue1->equal(queue1, queue));
    test_print_front(queue);
    test_print_back(queue);    
    queue->clear(queue);
    test_print_front(queue);
    queue->free(queue);
    for (int i = 0 ; i < 5; ++i) {
        queue1->push_front(queue1, &buf[i]);
    }
    test_print_front(queue1);
    test_print_back(queue1);    
    queue1->clear(queue1);
    test_print_front(queue1);
    queue1->free(queue1);
}

void test_deque2() {

}