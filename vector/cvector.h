
#ifndef CVECTOR_H_INCLUDED
#define CVECTOR_H_INCLUDED


#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"{
#endif

struct cvector_t;
typedef struct cvector_t cvector;

// vector
// first                last             end
// |                     |               |
// V                     V               V
// +-------------------------------------+
// |    ... data ...     |               |
// +-------------------------------------+
// |<--------size()----->|
// |<---------------capacity()---------->|
struct cvector_t {
/*   clear: clear data, but not free
 *   thiz: cvector pointer
 */
    void      (*clear)(cvector *thiz);

/*   free: free thiz and data mem
 *   thiz: cvector pointer
 */
    void      (*free)(cvector *thiz);

/*   typesize: get item size
 *   thiz: cvector pointer
 *   return  item size > 0
 */
    uint64_t  (*typesize)(cvector *thiz);

/*   size: get item count
 *   thiz: cvector pointer
 *   return  item count > 0
 */
    uint64_t  (*size)(cvector *thiz);

/*   capacity: get vector max item count
 *   thiz: cvector pointer
 *   return  imax item count > 0
 */
    uint64_t  (*capacity)(cvector *thiz);

/*   empty: item count == 0
 *   thiz: cvector pointer
 *   return  item count == 0
 */
    uint8_t   (*empty)(cvector *thiz);

/*   resize: malloc n * typesize bytes
 *   thiz: cvector pointer
 *   n:    n items  equal val
 *   val:  item pointer
 */
    void      (*resize)(cvector *thiz, uint64_t n, const void* val);

/*   reserve: change capacity items
 *   thiz: cvector pointer
 *   capacity:   max item count
 */
    void      (*reserve)(cvector *thiz, uint64_t capacity);

/*   back: last item pointer
 *   thiz: cvector pointer
 *   return last item pointer
 */
    void*     (*back)(cvector *thiz);

/*   front: first item pointer
 *   thiz: cvector pointer
 *   return first item pointer
 */
    void*     (*front)(cvector *thiz);

/*   at: index item pointer
 *   thiz: cvector pointer
 *   index: item index
 *   return index item pointer
 */
    void*     (*at)(cvector *thiz, uint64_t index);

/*   data: first item pointer
 *   thiz: cvector pointer
 *   return first item pointer
 */
    void*     (*data)(cvector *thiz);

/*   begin: first item pointer
 *   thiz: cvector pointer
 *   return first item pointer
 */
    void*     (*begin)(cvector *thiz);

/*   end: last item pointer + typesize
 *   thiz: cvector pointer
 *   return last item pointer + typesize
 */
    void*     (*end)(cvector *thiz);

/*   rbegin: last item pointer
 *   thiz: cvector pointer
 *   return last item pointer
 */
    void*     (*rbegin)(cvector *thiz);

/*   back: last item pointer
 *   thiz: cvector pointer
 *   return last item pointer
 */
    void*     (*rend)(cvector *thiz);

/*   push_back: add last item behind 
 *   thiz: cvector pointer
 *   val:  item pointer
 */
    void      (*push_back)(cvector *thiz, const void* val);

/*   push_front: add first item before 
 *   thiz: cvector pointer
 *   val:  item pointer
 */
    void      (*push_front)(cvector *thiz, const void* val);

/*   pop_back: delete last item 
 *   thiz: cvector pointer
 */
    void      (*pop_back)(cvector *thiz);

/*   pop_front: delete first item 
 *   thiz: cvector pointer
 */
    void      (*pop_front)(cvector *thiz);

/*   erase: delete from first to last items 
 *   thiz: cvector pointer
 *   first: begin item pointer
 *   last: last item pointer
 */
    void      (*erase)(cvector *thiz, void* first, void* last);

/*   remove: delete position item 
 *   thiz: cvector pointer
 *   position: item pointer
 */
    void      (*remove)(cvector *thiz, void* position);

/*   assign: copy value from first to last items 
 *   thiz: cvector pointer
 *   first: begin item pointer
 *   last: last item pointer
 */
    void      (*assign)(cvector *thiz, void* first, void* last);

/*   fill: copy value  n * typesize val from position begin
 *   thiz: cvector pointer
 *   position: item pointer
 *   n:    n items  equal val
 *   val:  item pointer
 */
    void      (*fill)(cvector *thiz, void* position, uint64_t n, const void* val);

/*   insert: insert values from first to last items  at position
 *   thiz: cvector pointer
 *   position: item pointer
 *   first: begin item pointer
 *   last: last item pointer
 */
    void      (*insert)(cvector *thiz, void* position, void* first, void* last);

/*   reverse: first and last items change 
 *   thiz: cvector pointer
 */
    void      (*reverse)(cvector *thiz);

/*   copy: copy value from thiz to that
 *   thiz: cvector pointer
 *   that: cvector pointer
 */
    void      (*copy)(cvector *thiz, cvector *that);

/*   equal: compare thiz with that
 *   thiz: cvector pointer
 *   that: cvector pointer
 *   return: thiz == that
 */
    uint8_t   (*equal)(cvector *thiz, cvector *that);
};

/*   cvector_alloc: malloc cvector pointer
 *   size:     cvector item count
 *   typesize: cvector item size
 *   return: cvector pointer
 */
cvector* cvector_alloc(uint64_t size, uint64_t typesize);


#ifdef __cplusplus
}
#endif

#endif 