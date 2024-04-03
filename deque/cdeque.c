#include <string.h>
#include <stdlib.h>
#include "cdeque.h"


struct cdeque_node_t;
typedef struct  cdeque_node_t  cdeque_node;

struct cdeque_node_t {
    cdeque_node   *prev;
    cdeque_node   *next;
    void          *data;
};



/*   alloc: alloc new node
 *   data: item data pointer
 *   return:  return node pointer
 */
static cdeque_node* cdeque_node_alloc(uint64_t typesize, const void *data) {
	cdeque_node* node = malloc(sizeof(cdeque_node));
    node->data = malloc(typesize);
    if (data != NULL) {
    	memcpy(node->data, data, typesize);
    }
	node->next = node->prev = node;
	return node;
}

/*   insert: insert new node
 *   head: node pointer , node insert behind head
 *   node: node pointer
 *   return:  return node pointer
 */
static cdeque_node* cdeque_node_insert(cdeque_node* head, cdeque_node* node) {
	cdeque_node *next = head->next;
	head->next       = node;
	node->next       = next;
	next->prev       = node;
	node->prev       = head;
	return head;
}

/*   erase: erase node
 *   node: node pointer
 *   return:  return node pointer
 */
static cdeque_node* cdeque_node_erase(cdeque_node* node) {
	cdeque_node *prev = node->prev, *next = node->next;
	prev->next = next;
	next->prev = prev;
	return node;
}

/*   free: free node
 *   node: node pointer
 */
static void        cdeque_node_free(cdeque_node *node) {
	cdeque_node *prev = node->prev, *next = node->next;
	prev->next = next;
	next->prev = prev;
	if (node->data)
		free(node->data);
	free(node);
}

/*   get data: get node data pointer
 *   node: node pointer
 *   return: return data pointer
 */
static void*       cdeque_node_data(cdeque_node* node) {
	return node->data;
}

/*   next: get next node pointer
 *   node: node pointer
 *   return: return next node pointer
 */
static cdeque_node* cdeque_node_next(cdeque_node* node) {
	return node->next;
}

/*   prev: get prev node pointer
 *   node: node pointer
 *   return: return prev node pointer
 */
static cdeque_node* cdeque_node_prev(cdeque_node* node) {
	return node->prev;
}



struct cdeque_data_t {
	cdeque                      deque;
    cdeque_node                 head;
    uint64_t                    count;
    uint64_t                    typesize;
};

typedef struct cdeque_data_t  cdeque_data;

/*   clear: clear data, but not free
 *   thiz: cdeque pointer
 */
static    void    cdeque_static_clear(cdeque *thiz) {
	cdeque_node *node = NULL, *next = NULL;
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cdeque_data *) thiz;
	if (thiz_data->count <= 0)
		return;
	node = thiz_data->head.next;
	while(node != &(thiz_data->head)) {
		next = node->next;
        cdeque_node_free(node);
        node = next;
	}
	thiz_data->head.next = thiz_data->head.prev = &(thiz_data->head);
    thiz_data->count = 0;
}

/*   free: free thiz and data mem
 *   thiz: cdeque pointer
 */
static    void    cdeque_static_free(cdeque *thiz) {
	cdeque_node *node = NULL, *next = NULL;
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cdeque_data *) thiz;
	if (thiz_data->count <= 0) {
		free(thiz_data);
		return;
	}
	node = thiz_data->head.next;
	while(node != &(thiz_data->head)) {
		next = node->next;
        cdeque_node_free(node);
        node = next;
	}
	thiz_data->head.next = thiz_data->head.prev = &(thiz_data->head);
    thiz_data->count = 0;
	free(thiz_data);
}

/*   typesize: get item size
 *   thiz: cdeque pointer
 *   return  item size > 0
 */
static uint64_t    cdeque_static_typesize(cdeque *thiz) {
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return 0;
	thiz_data = (cdeque_data *) thiz;
	return thiz_data->typesize;
}

/*   size: get item count
 *   thiz: cdeque pointer
 *   return  item count > 0
 */
static uint64_t    cdeque_static_size(cdeque *thiz) {
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return 0;
	thiz_data = (cdeque_data *) thiz;
	return thiz_data->count;
}

/*   empty: item count == 0
 *   thiz: cdeque pointer
 *   return  item count == 0
 */
static uint8_t    cdeque_static_empty(cdeque *thiz) {
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return 0;
	thiz_data = (cdeque_data *) thiz;
	if (thiz_data->count == 0)
		return 1;
	return 0;
}

/*   front: front item pointer
 *   thiz: cdeque pointer
 *   return front item pointer
 */
static    void*    cdeque_static_front(cdeque *thiz) {
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return NULL;
	thiz_data = (cdeque_data *) thiz;
	if (thiz_data->head.next == &(thiz_data->head))
		return NULL;
    return cdeque_node_data(thiz_data->head.next);
}

/*   back: back item pointer
 *   thiz: cdeque pointer
 *   return back item pointer
 */
static    void*    cdeque_static_back(cdeque *thiz) {
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return NULL;
	thiz_data = (cdeque_data *) thiz;
	if (thiz_data->head.prev == &(thiz_data->head))
		return NULL;
    return cdeque_node_data(thiz_data->head.prev);
}

/*   push_front: add front item behind 
 *   thiz: cdeque pointer
 *   val:  item pointer
 */
static    void    cdeque_static_push_front(cdeque *thiz, const void* val) {
	cdeque_node *node = NULL;
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cdeque_data *) thiz;
	node = cdeque_node_alloc(thiz_data->typesize, val);
	cdeque_node_insert(&(thiz_data->head), node);
	++thiz_data->count;
}

/*   push_back: add back item behind 
 *   thiz: cdeque pointer
 *   val:  item pointer
 */
static    void    cdeque_static_push_back(cdeque *thiz, const void* val) {
	cdeque_node *node = NULL;
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cdeque_data *) thiz;
	node = cdeque_node_alloc(thiz_data->typesize, val);
	cdeque_node_insert(thiz_data->head.prev, node);
	++thiz_data->count;
}

/*   pop_front: delete front item 
 *   thiz: cdeque pointer
 */
static    void    cdeque_static_pop_front(cdeque *thiz) {
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cdeque_data *) thiz;
	if (thiz_data->count <= 0)
		return;
	cdeque_node_free(thiz_data->head.next);
    --thiz_data->count;
}

/*   pop_back: delete back item 
 *   thiz: cdeque pointer
 */
static    void    cdeque_static_pop_back(cdeque *thiz) {
	cdeque_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cdeque_data *) thiz;
	if (thiz_data->count <= 0)
		return;
	cdeque_node_free(thiz_data->head.prev);
    --thiz_data->count;
}

/*   copy: copy value from thiz to that
 *   thiz: cdeque pointer
 *   that: cdeque pointer
 */
static    void    cdeque_static_copy(cdeque *thiz, cdeque *that) {
	cdeque_node *node = NULL, *prev = NULL, *next = NULL;
	cdeque_data *thiz_data = NULL, *that_data = NULL;
	if ((thiz == NULL) || (that == NULL)) {
		return;
	}
	thiz_data = (cdeque_data *) thiz;
	that_data = (cdeque_data *) that;

    node = thiz_data->head.next;
    that->clear(that);
    that_data->typesize = thiz_data->typesize;
    while (node != &(thiz_data->head)) {
        prev = that_data->head.prev;
        next = cdeque_node_alloc(thiz_data->typesize, node->data);
    	cdeque_node_insert(prev, next);
    	node = node->next;
    }
    that_data->count = thiz_data->count;
}

/*   equal: compare thiz with that
 *   thiz: cdeque pointer
 *   that: cdeque pointer
 *   return: thiz == that
 */
static    uint8_t    cdeque_static_equal(cdeque *thiz, cdeque *that) {
	cdeque_node *node = NULL, *next = NULL;
	cdeque_data *thiz_data = NULL, *that_data = NULL;
	if ((thiz == NULL) || (that == NULL)) {
		return 0;
	}
	thiz_data = (cdeque_data *) thiz;
	that_data = (cdeque_data *) that;
	if ((thiz_data->typesize != that_data->typesize) || (thiz_data->count != that_data->count)) {
		return 0;
	}

    node = thiz_data->head.next;
    next = that_data->head.next;
    while (node != &(thiz_data->head)) {
    	if (memcmp(node->data, next->data, thiz_data->typesize) != 0)
    		return 0;
    	node = node->next;
    	next = next->next;
    }
    return 1;	
}

/*   cdeque_alloc: malloc cdeque pointer
 *   typesize: cdeque item size
 *   return: cdeque pointer
 */
cdeque* cdeque_alloc(uint64_t typesize) {
	cdeque *thiz = NULL;
	cdeque_data *thiz_data = NULL;
	if (typesize <= 0) {
		return NULL;
	}

	thiz_data = (cdeque_data *)malloc(sizeof(cdeque_data));
	if (thiz_data == NULL) {
		return NULL;
	}

    thiz_data->typesize = typesize;
    thiz_data->count    = 0;
    thiz_data->head.prev = thiz_data->head.next = &(thiz_data->head);
    thiz_data->head.data = NULL;

    thiz = (cdeque *) &(thiz_data->deque);

	thiz->clear = cdeque_static_clear;
	thiz->free  = cdeque_static_free;
	thiz->typesize  = cdeque_static_typesize;
	thiz->size      = cdeque_static_size;
	thiz->empty     = cdeque_static_empty;

	thiz->front  = cdeque_static_front;
	thiz->back   = cdeque_static_back;
	thiz->push_front   = cdeque_static_push_front;
	thiz->push_back    = cdeque_static_push_back;
	thiz->pop_front    = cdeque_static_pop_front;
	thiz->pop_back     = cdeque_static_pop_back;
	thiz->copy   = cdeque_static_copy;
	thiz->equal  = cdeque_static_equal;

    return thiz;
}
