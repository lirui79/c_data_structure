#include  <stddef.h>
#include  <stdio.h>
#include  <stdint.h>
#include  <stdlib.h>
#include  <string.h>

#include  "cqueue.h"

static void test_print(cqueue *queue) {
    void *it = NULL;
    printf("%lld %lld %d\n", queue->size(queue), queue->typesize(queue), queue->empty(queue));
    while(!queue->empty(queue)) {
        it = queue->front(queue);
    	int value = *((int*)it);
        queue->pop(queue);
    	printf("%x ", value);
    }
    printf("\n");    
}

static void test_queue1();

static void test_queue2();

int main(int argc, const char *argv[]) {
	test_queue1();
	test_queue2();
	return 0;
}

void test_queue1() {
    int buf[] = {0x01, 0x12, 0x23, 0x34, 0x45};
    cqueue *queue  = cqueue_alloc(sizeof(int));
    cqueue *queue1 = cqueue_alloc(sizeof(int)); 
    for (int i = 0 ; i < 5; ++i) {
        queue->push(queue, &buf[i]);
    }
    queue->copy(queue, queue1);
    printf("%d\n", queue1->equal(queue1, queue));
    test_print(queue);

    queue->clear(queue);
    test_print(queue);
    queue->free(queue);
    for (int i = 0 ; i < 5; ++i) {
        queue1->push(queue1, &buf[i]);
    }
    test_print(queue1);

    queue1->clear(queue1);
    test_print(queue1);
    queue1->free(queue1);
}

void test_queue2() {

}