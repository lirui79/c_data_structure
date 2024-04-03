#include  <stddef.h>
#include  <stdio.h>
#include  <stdint.h>
#include  <stdlib.h>
#include  <string.h>

#include  "clist.h"

static void test_print(clist *list) {
    clist_node *it = NULL;
    printf("%lld %lld %d\n", list->size(list), list->typesize(list), list->empty(list));
    for (it = list->begin(list); it != list->end(list); it = clist_node_next(it)) {
    	int value = *((int*)clist_node_data(it));
    	printf("%x ", value);
    }

    printf("\n");
    void *val = list->front(list);
    if (val != NULL) {
    	int front = *((int*) val);
        printf("front:%x\n",front);
    }

    val = list->back(list);
    if (val != NULL) {
    	int back = *((int*) val);
        printf("back:%x\n",back);
    }
}

static void test_rprint(clist *list) {
    clist_node *it = NULL;
    printf("%lld %lld %d\n", list->size(list), list->typesize(list), list->empty(list));
    for (it = list->rbegin(list); it != list->rend(list); it = clist_node_prev(it)) {
        int value = *((int*)clist_node_data(it));
        printf("%x ", value);
    }

    printf("\n");
}

static void test_list1();

static void test_list2();

int main(int argc, const char *argv[]) {
	test_list1();
	test_list2();
	return 0;
}

void test_list1() {
    int buf[] = {0x21, 0x42, 0x63, 0x84, 0xa5};
    clist *list1 = clist_alloc(sizeof(int));
    list1->assign(list1, buf, &buf[5]);
    test_print(list1);
    test_rprint(list1);
    list1->push_back(list1, &buf[1]);
    test_print(list1);
    list1->push_front(list1, &buf[4]);
    test_print(list1);
    list1->remove(list1, &buf[3]);
    test_print(list1);

    uint64_t index = 3;
    void *val = clist_node_data(list1->at(list1, index));
    if (val != NULL) {
        int  value = *((int*)val);
        printf("index:%lld %x\n", index, value);
    }

    val =clist_node_data(list1->find(list1, &buf[4]));
    if (val != NULL) {
        int  value = *((int*)val);
        printf("find:%x\n", value);
    }

    list1->reverse(list1);
    test_print(list1);
    list1->clear(list1);
    test_print(list1);
    list1->free(list1);
}

void test_list2() {
    clist *list1 = clist_alloc(sizeof(int));
    clist *list2 = clist_alloc(sizeof(int));
    int i, value;
    for (i = 0x100; i < 0x120; ++i) {
    	list1->push_back(list1, &i);
    }

    for (i = 0x100; i < 0x120; ++i) {
        list1->push_front(list1, &i);
    }

    test_print(list1);
    list1->pop_front(list1);
    test_print(list1);
    list1->pop_back(list1);
    test_print(list1);

    list1->copy(list1, list2);
    printf("%d\n", list1->equal(list1, list2));
    list1->free(list1);
    list2->free(list2);
}