#include <string.h>
#include <stdlib.h>
#include "cqueue.h"


struct cqueue_node_t;
typedef struct  cqueue_node_t  cqueue_node;

struct cqueue_node_t {
    cqueue_node   *next;
    void          *data;
};


/*   alloc: alloc new node
 *   data: item data pointer
 *   return:  return node pointer
 */
static cqueue_node* cqueue_node_alloc(uint64_t typesize, const void *data) {
	cqueue_node* node = malloc(sizeof(cqueue_node));
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
static cqueue_node* cqueue_node_insert(cqueue_node* prev, cqueue_node* node) {
	cqueue_node *next = prev->next;
	prev->next      = node;
	node->next        = next;
	return prev;
}

/*   erase: erase node
 *   prev: node pointer , node erase behind prev
 *   node: node pointer
 *   return:  return node pointer
 */
static cqueue_node* cqueue_node_erase(cqueue_node* prev, cqueue_node* node) {
	cqueue_node *next = node->next;
	prev->next = next;
	node->next = node;
	return node;
}

/*   free: free node
 *   prev: node pointer , node free behind prev
 *   node: node pointer
 */
static void        cqueue_node_free(cqueue_node* prev, cqueue_node *node) {
	cqueue_node *next = node->next;
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
static void*       cqueue_node_data(cqueue_node* node) {
	return node->data;
}

/*   next: get next node pointer
 *   node: node pointer
 *   return: return next node pointer
 */
static cqueue_node* cqueue_node_next(cqueue_node* node) {
	return node->next;
}



struct cqueue_data_t {
	cqueue                      queue;
    cqueue_node                 head;
    cqueue_node                 tail;
    uint64_t                    count;
    uint64_t                    typesize;
};

typedef struct cqueue_data_t  cqueue_data;

/*   clear: clear data, but not free
 *   thiz: cqueue pointer
 */
static    void    cqueue_static_clear(cqueue *thiz) {
	cqueue_node *node = NULL, *next = NULL, *prev = NULL;
	cqueue_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cqueue_data *) thiz;
	if (thiz_data->count <= 0)
		return;
	prev = &(thiz_data->head);
	node = thiz_data->head.next;
	while(node != &(thiz_data->head)) {
		next = node->next;
        cqueue_node_free(prev, node);
        node = next;
	}
	thiz_data->head.next = &(thiz_data->head);
	thiz_data->tail.next = &(thiz_data->head);
    thiz_data->count = 0;
}

/*   free: free thiz and data mem
 *   thiz: cqueue pointer
 */
static    void    cqueue_static_free(cqueue *thiz) {
	cqueue_node *node = NULL, *next = NULL, *prev = NULL;
	cqueue_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cqueue_data *) thiz;
	if (thiz_data->count <= 0) {
		free(thiz_data);
		return;
	}
	prev = &(thiz_data->head);
	node = thiz_data->head.next;
	while(node != &(thiz_data->head)) {
		next = node->next;
        cqueue_node_free(prev, node);
        node = next;
	}
	thiz_data->head.next = &(thiz_data->head);
	thiz_data->tail.next = &(thiz_data->head);
    thiz_data->count = 0;
	free(thiz_data);
}

/*   typesize: get item size
 *   thiz: cqueue pointer
 *   return  item size > 0
 */
static uint64_t    cqueue_static_typesize(cqueue *thiz) {
	cqueue_data *thiz_data = NULL;
	if (thiz == NULL) 
		return 0;
	thiz_data = (cqueue_data *) thiz;
	return thiz_data->typesize;
}

/*   size: get item count
 *   thiz: cqueue pointer
 *   return  item count > 0
 */
static uint64_t    cqueue_static_size(cqueue *thiz) {
	cqueue_data *thiz_data = NULL;
	if (thiz == NULL) 
		return 0;
	thiz_data = (cqueue_data *) thiz;
	return thiz_data->count;
}

/*   empty: item count == 0
 *   thiz: cqueue pointer
 *   return  item count == 0
 */
static uint8_t    cqueue_static_empty(cqueue *thiz) {
	cqueue_data *thiz_data = NULL;
	if (thiz == NULL) 
		return 0;
	thiz_data = (cqueue_data *) thiz;
	if (thiz_data->count == 0)
		return 1;
	return 0;
}

/*   top: top item pointer
 *   thiz: cqueue pointer
 *   return top item pointer
 */
static    void*    cqueue_static_front(cqueue *thiz) {
	cqueue_data *thiz_data = NULL;
	if (thiz == NULL) 
		return NULL;
	thiz_data = (cqueue_data *) thiz;
	if (thiz_data->head.next == &(thiz_data->head))
		return NULL;
    return cqueue_node_data(thiz_data->head.next);
}

/*   push: add top item behind 
 *   thiz: cqueue pointer
 *   val:  item pointer
 */
static    void    cqueue_static_push(cqueue *thiz, const void* val) {
	cqueue_node *node = NULL;
	cqueue_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cqueue_data *) thiz;
	node = cqueue_node_alloc(thiz_data->typesize, val);
	cqueue_node_insert(thiz_data->tail.next, node);
	thiz_data->tail.next = node;
	++thiz_data->count;
}

/*   pop: delete top item 
 *   thiz: cqueue pointer
 */
static    void    cqueue_static_pop(cqueue *thiz) {
	cqueue_data *thiz_data = NULL;
	if (thiz == NULL) 
		return;
	thiz_data = (cqueue_data *) thiz;
	if (thiz_data->count <= 0)
		return;
	cqueue_node_free(&(thiz_data->head), thiz_data->head.next);
    --thiz_data->count;
    if (thiz_data->count > 0)
    	return;
    thiz_data->tail.next = &(thiz_data->head);
}

/*   copy: copy value from thiz to that
 *   thiz: cqueue pointer
 *   that: cqueue pointer
 */
static    void    cqueue_static_copy(cqueue *thiz, cqueue *that) {
	cqueue_node *node = NULL, *prev = NULL, *next = NULL;
	cqueue_data *thiz_data = NULL, *that_data = NULL;
	if ((thiz == NULL) || (that == NULL)) {
		return;
	}
	thiz_data = (cqueue_data *) thiz;
	that_data = (cqueue_data *) that;

    node = thiz_data->head.next;
    that->clear(that);
    that_data->typesize = thiz_data->typesize;
    while (node != &(thiz_data->head)) {
        prev = that_data->tail.next;
        next = cqueue_node_alloc(thiz_data->typesize, node->data);
    	cqueue_node_insert(prev, next);
    	node = node->next;
    	that_data->tail.next = next;
    }
    that_data->count = thiz_data->count;
}

/*   equal: compare thiz with that
 *   thiz: cqueue pointer
 *   that: cqueue pointer
 *   return: thiz == that
 */
static    uint8_t    cqueue_static_equal(cqueue *thiz, cqueue *that) {
	cqueue_node *node = NULL, *next = NULL;
	cqueue_data *thiz_data = NULL, *that_data = NULL;
	if ((thiz == NULL) || (that == NULL)) {
		return 0;
	}
	thiz_data = (cqueue_data *) thiz;
	that_data = (cqueue_data *) that;
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

/*   cqueue_alloc: malloc cqueue pointer
 *   typesize: cqueue item size
 *   return: cqueue pointer
 */
cqueue* cqueue_alloc(uint64_t typesize) {
	cqueue *thiz = NULL;
	cqueue_data *thiz_data = NULL;
	if (typesize <= 0) {
		return NULL;
	}

	thiz_data = (cqueue_data *)malloc(sizeof(cqueue_data));
	if (thiz_data == NULL) {
		return NULL;
	}

    thiz_data->typesize = typesize;
    thiz_data->count    = 0;
    thiz_data->head.next = &(thiz_data->head);
    thiz_data->head.data = NULL;
    thiz_data->tail.next = &(thiz_data->head);
    thiz_data->tail.data = NULL;

    thiz = (cqueue *) &(thiz_data->queue);

	thiz->clear = cqueue_static_clear;
	thiz->free  = cqueue_static_free;
	thiz->typesize  = cqueue_static_typesize;
	thiz->size      = cqueue_static_size;
	thiz->empty     = cqueue_static_empty;

	thiz->front  = cqueue_static_front;
	thiz->push   = cqueue_static_push;
	thiz->pop    = cqueue_static_pop;
	thiz->copy   = cqueue_static_copy;
	thiz->equal  = cqueue_static_equal;

    return thiz;
}
