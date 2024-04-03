#include <string.h>
#include <stdlib.h>
#include "cstack.h"


struct cstack_node_t;
typedef struct  cstack_node_t  cstack_node;

struct cstack_node_t {
    cstack_node   *next;
    void          *data;
};


/*   alloc: alloc new node
 *   data: item data pointer
 *   return:  return node pointer
 */
static cstack_node* cstack_node_alloc(uint64_t typesize, const void *data) {
	cstack_node* node = malloc(sizeof(cstack_node));
    node->data = malloc(typesize);
    if (data != NULL) {
    	memcpy(node->data, data, typesize);
    }
	node->next = node;
	return node;
}

/*   insert: insert new node
 *   prev: node pointer , node insert behind prev
 *   node: node pointer
 *   return:  return node pointer
 */
static cstack_node* cstack_node_insert(cstack_node* prev, cstack_node* node) {
	cstack_node *next = prev->next;
	prev->next      = node;
	node->next        = next;
	return prev;
}

/*   erase: erase node
 *   prev: node pointer , node erase behind prev
 *   node: node pointer
 *   return:  return node pointer
 */
static cstack_node* cstack_node_erase(cstack_node* prev, cstack_node* node) {
	cstack_node *next = node->next;
	prev->next = next;
	node->next = node;
	return node;
}

/*   free: free node
 *   prev: node pointer , node free behind prev
 *   node: node pointer
 */
static void        cstack_node_free(cstack_node* prev, cstack_node *node) {
	cstack_node *next = node->next;
	prev->next = next;
	node->next = node;
	if (node->data)
		free(node->data);
	free(node);
}

/*   get data: get node data pointer
 *   node: node pointer
 *   return: return data pointer
 */
static void*       cstack_node_data(cstack_node* node) {
	return node->data;
}

/*   next: get next node pointer
 *   node: node pointer
 *   return: return next node pointer
 */
static cstack_node* cstack_node_next(cstack_node* node) {
	return node->next;
}



struct cstack_data_t {
	cstack                      stack;
    cstack_node                 topped;
    uint64_t                    count;
    uint64_t                    typesize;
};

typedef struct cstack_data_t  cstack_data;


/*   clear: clear data, but not free
 *   thiz: cstack pointer
 */
static    void    cstack_static_clear(cstack *_thiz) {
	cstack_node *node = NULL, *next = NULL, *prev = NULL;
	cstack_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cstack_data*) _thiz;
	if (thiz->count <= 0)
		return;
	prev = &(thiz->topped);
	node = thiz->topped.next;
	while(node != &(thiz->topped)) {
		next = node->next;
        cstack_node_free(prev, node);
        node = next;
	}
	thiz->topped.next = &(thiz->topped);
    thiz->count = 0;
}

/*   free: free thiz and data mem
 *   thiz: cstack pointer
 */
static    void    cstack_static_free(cstack *_thiz) {
	cstack_node *node = NULL, *next = NULL, *prev = NULL;
	cstack_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cstack_data*) _thiz;
	if (thiz->count <= 0) {
		free(thiz);
		return;
	}
	prev = &(thiz->topped);
	node = thiz->topped.next;
	while(node != &(thiz->topped)) {
		next = node->next;
        cstack_node_free(prev, node);
        node = next;
	}
	thiz->topped.next = &(thiz->topped);
    thiz->count = 0;
	free(thiz);
}

/*   typesize: get item size
 *   thiz: cstack pointer
 *   return  item size > 0
 */
static uint64_t    cstack_static_typesize(cstack *_thiz) {
	cstack_data *thiz = NULL;
	if (_thiz == NULL) 
		return 0;
	thiz = (cstack_data*) _thiz;
	return thiz->typesize;
}

/*   size: get item count
 *   thiz: cstack pointer
 *   return  item count > 0
 */
static uint64_t    cstack_static_size(cstack *_thiz) {
	cstack_data *thiz = NULL;
	if (_thiz == NULL) 
		return 0;
	thiz = (cstack_data*) _thiz;
	return thiz->count;
}

/*   empty: item count == 0
 *   thiz: cstack pointer
 *   return  item count == 0
 */
static uint8_t    cstack_static_empty(cstack *_thiz) {
	cstack_data *thiz = NULL;
	if (_thiz == NULL) 
		return 0;
	thiz = (cstack_data*) _thiz;
	if (thiz->count == 0)
		return 1;
	return 0;
}

/*   top: top item pointer
 *   thiz: cstack pointer
 *   return top item pointer
 */
static    void*    cstack_static_top(cstack *_thiz) {
	cstack_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cstack_data*) _thiz;
	if (thiz->topped.next == &(thiz->topped))
		return NULL;
    return cstack_node_data(thiz->topped.next);
}

/*   push: add top item behind 
 *   thiz: cstack pointer
 *   val:  item pointer
 */
static    void    cstack_static_push(cstack *_thiz, const void* val) {
	cstack_node *node = NULL;
	cstack_data *thiz = NULL;
	if ((_thiz == NULL) || (val == NULL))
		return;
	thiz = (cstack_data*) _thiz;
	node = cstack_node_alloc(thiz->typesize, val);
	cstack_node_insert(&(thiz->topped), node);
	++thiz->count;
}

/*   pop: delete top item 
 *   thiz: cstack pointer
 */
static    void    cstack_static_pop(cstack *_thiz) {
	cstack_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cstack_data*) _thiz;
	if (thiz->count <= 0)
		return;
	cstack_node_free(&(thiz->topped), thiz->topped.next);
    --thiz->count;
}

/*   copy: copy value from thiz to that
 *   thiz: cstack pointer
 *   that: cstack pointer
 */
static    void    cstack_static_copy(cstack *_thiz, cstack *_that) {
	cstack_node *node = NULL, *next = NULL;
	cstack_data *thiz = NULL, *that = NULL;
	if ((_thiz == NULL) || (_that == NULL)) {
		return;
	}
	thiz = (cstack_data*) _thiz;
	that = (cstack_data*) _that;
    node = thiz->topped.next;
    _that->clear(_that);
    that->typesize = thiz->typesize;
    next = that->topped.next;
    while (node != &(thiz->topped)) {
    	cstack_node_insert(next, cstack_node_alloc(thiz->typesize, node->data));
    	node = node->next;
    	next = next->next;
    }
    that->count = thiz->count;
}

/*   equal: compare thiz with that
 *   thiz: cstack pointer
 *   that: cstack pointer
 *   return: thiz == that
 */
static    uint8_t    cstack_static_equal(cstack *_thiz, cstack *_that) {
	cstack_node *node = NULL, *next = NULL;
	cstack_data *thiz = NULL, *that = NULL;
	if ((_thiz == NULL) || (_that == NULL)) {
		return 0;
	}
	thiz = (cstack_data*) _thiz;
	that = (cstack_data*) _that;
	if ((thiz->typesize != that->typesize) || (thiz->count != that->count)) {
		return 0;
	}

    node = thiz->topped.next;
    next = that->topped.next;
    while (node != &(thiz->topped)) {
    	if (memcmp(node->data, next->data, thiz->typesize) != 0)
    		return 0;
    	node = node->next;
    	next = next->next;
    }
    return 1;	
}

/*   cstack_alloc: malloc cstack pointer
 *   typesize: cstack item size
 *   return: cstack pointer
 */
cstack* cstack_alloc(uint64_t typesize) {
	cstack *thiz = NULL;
	cstack_data *thiz_data = NULL;
	if (typesize <= 0) {
		return NULL;
	}

	thiz_data = (cstack_data *)malloc(sizeof(cstack_data));
	if (thiz_data == NULL) {
		return NULL;
	}

    thiz_data->typesize = typesize;
    thiz_data->count    = 0;
    thiz_data->topped.next = &(thiz_data->topped);
    thiz_data->topped.data = NULL;

    thiz = (cstack *) &(thiz_data->stack);

	thiz->clear = cstack_static_clear;
	thiz->free  = cstack_static_free;
	thiz->typesize  = cstack_static_typesize;
	thiz->size      = cstack_static_size;
	thiz->empty     = cstack_static_empty;

	thiz->top    = cstack_static_top;
	thiz->push   = cstack_static_push;
	thiz->pop    = cstack_static_pop;
	thiz->copy   = cstack_static_copy;
	thiz->equal  = cstack_static_equal;

    return thiz;
}
