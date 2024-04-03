
#ifndef CSTACK_H_INCLUDED
#define CSTACK_H_INCLUDED


#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"{
#endif



struct cstack_t;
typedef struct cstack_t cstack;

// stack

struct cstack_t {
/*   clear: clear data, but not free
 *   thiz: cstack pointer
 */
    void      (*clear)(cstack *thiz);

/*   free: free thiz and data mem
 *   thiz: cstack pointer
 */
    void      (*free)(cstack *thiz);

/*   typesize: get item size
 *   thiz: cstack pointer
 *   return  item size > 0
 */
    uint64_t  (*typesize)(cstack *thiz);

/*   size: get item count
 *   thiz: cstack pointer
 *   return  item count > 0
 */
    uint64_t  (*size)(cstack *thiz);

/*   empty: item count == 0
 *   thiz: cstack pointer
 *   return  item count == 0
 */
    uint8_t   (*empty)(cstack *thiz);

/*   top: top item pointer
 *   thiz: cstack pointer
 *   return top item pointer
 */
    void*     (*top)(cstack *thiz);

/*   push: add top item behind 
 *   thiz: cstack pointer
 *   val:  item pointer
 */
    void      (*push)(cstack *thiz, const void* val);

/*   pop: delete top item 
 *   thiz: cstack pointer
 */
    void      (*pop)(cstack *thiz);

/*   copy: copy value from thiz to that
 *   thiz: cstack pointer
 *   that: cstack pointer
 */
    void      (*copy)(cstack *thiz, cstack *that);

/*   equal: compare thiz with that
 *   thiz: cstack pointer
 *   that: cstack pointer
 *   return: thiz == that
 */
    uint8_t   (*equal)(cstack *thiz, cstack *that);

};

/*   cstack_alloc: malloc cstack pointer
 *   typesize: cstack item size
 *   return: cstack pointer
 */
cstack* cstack_alloc(uint64_t typesize);


#ifdef __cplusplus
}
#endif

#endif 