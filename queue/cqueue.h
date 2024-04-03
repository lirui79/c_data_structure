
#ifndef CQUEUE_H_INCLUDED
#define CQUEUE_H_INCLUDED


#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"{
#endif


struct cqueue_t;
typedef struct cqueue_t cqueue;

// queue
struct cqueue_t {
/*   clear: clear data, but not free
 *   thiz: cqueue pointer
 */
    void      (*clear)(cqueue *thiz);

/*   free: free thiz and data mem
 *   thiz: cqueue pointer
 */
    void      (*free)(cqueue *thiz);

/*   typesize: get item size
 *   thiz: cqueue pointer
 *   return  item size > 0
 */
    uint64_t  (*typesize)(cqueue *thiz);

/*   size: get item count
 *   thiz: cqueue pointer
 *   return  item count > 0
 */
    uint64_t  (*size)(cqueue *thiz);

/*   empty: item count == 0
 *   thiz: cqueue pointer
 *   return  item count == 0
 */
    uint8_t   (*empty)(cqueue *thiz);

/*   front: front item pointer
 *   thiz: cqueue pointer
 *   return front item pointer
 */
    void*     (*front)(cqueue *thiz);

/*   push: add last item behind 
 *   thiz: cqueue pointer
 *   val:  item pointer
 */
    void      (*push)(cqueue *thiz, const void* val);

/*   pop: delete front item 
 *   thiz: cqueue pointer
 */
    void      (*pop)(cqueue *thiz);

/*   copy: copy value from thiz to that
 *   thiz: cqueue pointer
 *   that: cqueue pointer
 */
    void      (*copy)(cqueue *thiz, cqueue *that);

/*   equal: compare thiz with that
 *   thiz: cqueue pointer
 *   that: cqueue pointer
 *   return: thiz == that
 */
    uint8_t   (*equal)(cqueue *thiz, cqueue *that);
};

/*   cqueue_alloc: malloc cqueue pointer
 *   typesize: cqueue item size
 *   return: cqueue pointer
 */
cqueue* cqueue_alloc(uint64_t typesize);


#ifdef __cplusplus
}
#endif

#endif 