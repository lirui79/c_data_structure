
#ifndef CDEQUE_H_INCLUDED
#define CDEQUE_H_INCLUDED


#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"{
#endif


struct cdeque_t;
typedef struct cdeque_t cdeque;

// queue
struct cdeque_t {
/*   clear: clear data, but not free
 *   thiz: cdeque pointer
 */
    void      (*clear)(cdeque *thiz);

/*   free: free thiz and data mem
 *   thiz: cdeque pointer
 */
    void      (*free)(cdeque *thiz);

/*   typesize: get item size
 *   thiz: cdeque pointer
 *   return  item size > 0
 */
    uint64_t  (*typesize)(cdeque *thiz);

/*   size: get item count
 *   thiz: cdeque pointer
 *   return  item count > 0
 */
    uint64_t  (*size)(cdeque *thiz);

/*   empty: item count == 0
 *   thiz: cdeque pointer
 *   return  item count == 0
 */
    uint8_t   (*empty)(cdeque *thiz);

/*   front: front item pointer
 *   thiz: cdeque pointer
 *   return front item pointer
 */
    void*     (*front)(cdeque *thiz);

/*   back: back item pointer
 *   thiz: cdeque pointer
 *   return back item pointer
 */
    void*     (*back)(cdeque *thiz);

/*   push_front: add front item behind 
 *   thiz: cdeque pointer
 *   val:  item pointer
 */
    void      (*push_front)(cdeque *thiz, const void* val);

/*   push_back: add back item behind 
 *   thiz: cdeque pointer
 *   val:  item pointer
 */
    void      (*push_back)(cdeque *thiz, const void* val);

/*   pop_front: delete front item 
 *   thiz: cdeque pointer
 */
    void      (*pop_front)(cdeque *thiz);

/*   pop_back: delete back item 
 *   thiz: cdeque pointer
 */
    void      (*pop_back)(cdeque *thiz);

/*   copy: copy value from thiz to that
 *   thiz: cdeque pointer
 *   that: cdeque pointer
 */
    void      (*copy)(cdeque *thiz, cdeque *that);

/*   equal: compare thiz with that
 *   thiz: cdeque pointer
 *   that: cdeque pointer
 *   return: thiz == that
 */
    uint8_t   (*equal)(cdeque *thiz, cdeque *that);
};

/*   cdeque_alloc: malloc cdeque pointer
 *   typesize: cdeque item size
 *   return: cdeque pointer
 */
cdeque* cdeque_alloc(uint64_t typesize);


#ifdef __cplusplus
}
#endif

#endif 