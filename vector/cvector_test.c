#include  <stddef.h>
#include  <stdio.h>
#include  <stdint.h>
#include  <stdlib.h>
#include  <string.h>

#include  "cvector.h"

static void test_print(cvector *vec) {
    void *it = NULL;
    printf("%lld %lld %lld %d\n", vec->size(vec), vec->typesize(vec), vec->capacity(vec), vec->empty(vec));
    for (it = vec->begin(vec); it < vec->end(vec); it += vec->typesize(vec)) {
    	int value = *((int*)it);
    	printf("%x ", value);
    }

    printf("\n");
    it = vec->front(vec);
    if (it != NULL) {
    	int front = *((int*) it);
        printf("front:%x\n",front);
    }

    it = vec->back(vec);
    if (it != NULL) {
    	int back = *((int*) it);
        printf("back:%x\n",back);
    }
}

static void test_rprint(cvector *vec) {
    void *it = NULL;
    printf("%lld %lld %lld %d\n", vec->size(vec), vec->typesize(vec), vec->capacity(vec), vec->empty(vec));
    for (it = vec->rbegin(vec); it > vec->rend(vec); it -= vec->typesize(vec)) {
    	int value = *((int*)it);
    	printf("%x ", value);
    }
    printf("\n");
}

static void test_vector1();

static void test_vector2();

int main(int argc, const char *argv[]) {
	test_vector1();
	test_vector2();
	return 0;
}

void test_vector1() {
    int buf[] = {0x01, 0x12, 0x23, 0x34, 0x45};
    cvector *vec1 = cvector_alloc(6, sizeof(int));
    vec1->assign(vec1, buf, &buf[5]);
    test_print(vec1);
    test_rprint(vec1);

    vec1->insert(vec1, vec1->at(vec1, 2), buf, &buf[5]);
    test_print(vec1);

    vec1->fill(vec1, vec1->begin(vec1), 4, &buf[2]);
    test_print(vec1);

    uint64_t index = 4;
    void *val = vec1->at(vec1, index);
    if (val != NULL) {
    	int value = *((int*) val);
    	printf("index:%lld %x\n", index, value);
    }

    vec1->remove(vec1, vec1->at(vec1, 4));
    test_print(vec1);
    vec1->reverse(vec1);
    test_print(vec1);

    vec1->resize(vec1, 13, &buf[3]);
    test_print(vec1);
    index = 10;
    val = vec1->at(vec1, index);
    if (val != NULL) {
    	int value = *((int*) val);
    	printf("index:%lld %x\n", index, value);
    }
    val = vec1->data(vec1);
    if (val != NULL) {
    	int value = *((int*) val);
    	printf("data:%x\n", value);
    	vec1->erase(vec1, val, val + vec1->typesize(vec1));
    	test_print(vec1);
    }

    vec1->clear(vec1);
    test_print(vec1);
    vec1->free(vec1);
}

void test_vector2() {
	printf("test\n");
    cvector *vec2 = cvector_alloc(6, sizeof(int));
    cvector *vec3 = cvector_alloc(6, sizeof(int));
    int i, value;
    for (i = 0; i < 20; ++i) {
    	vec2->push_back(vec2, &i);
    }

    for (i = 100; i < 120; ++i) {
    	vec2->push_front(vec2, &i);
    }

    test_print(vec2);

    vec2->pop_front(vec2);
    test_print(vec2);
    vec2->pop_back(vec2);
    test_print(vec2);

    vec2->copy(vec2, vec3);
    printf("%d\n", vec2->equal(vec2, vec3));
    vec2->free(vec2);
    vec3->free(vec3);
}