#include <string.h>
#include <stdlib.h>
#include "cvector.h"

// vector
// first                last             end
// |                     |               |
// V                     V               V
// +-------------------------------------+
// |    ... data ...     |               |
// +-------------------------------------+
// |<--------size()----->|
// |<---------------capacity()---------->|
struct cvector_data_t {
	cvector                    vector;
    void*                      first;
    void*                      last;
    void*                      final;
    uint64_t                   typesize;
};

typedef struct cvector_data_t  cvector_data;


/*   clear: clear data, but not free
 *   thiz: cvector pointer
 */
static    void    cvector_static_clear(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
	thiz->last = thiz->first;
}

/*   free: free thiz and data mem
 *   thiz: cvector pointer
 */
static    void    cvector_static_free(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
	if (thiz->first != NULL)
		free(thiz->first);
	free(thiz);
}

/*   typesize: get item size
 *   thiz: cvector pointer
 *   return  item size > 0
 */
static uint64_t    cvector_static_typesize(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return 0;
	thiz = (cvector_data*) _thiz;
	return thiz->typesize;
}

/*   size: get item count
 *   thiz: cvector pointer
 *   return  item count > 0
 */
static uint64_t    cvector_static_size(cvector *_thiz) {
	uint64_t size = 0;
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return 0;
	thiz = (cvector_data*) _thiz;
	if (thiz->typesize <= 0)
		return 0;
	size = (thiz->last - thiz->first) / thiz->typesize;
	return size;
}

/*   capacity: get vector max item count
 *   thiz: cvector pointer
 *   return  imax item count > 0
 */
static  uint64_t    cvector_static_capacity(cvector *_thiz) {
	uint64_t size = 0;
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return 0;
	thiz = (cvector_data*) _thiz;
	if (thiz->typesize <= 0)
		return 0;
	size = (thiz->final - thiz->first) / thiz->typesize;
	return size;
}

/*   empty: item count == 0
 *   thiz: cvector pointer
 *   return  item count == 0
 */
static uint8_t    cvector_static_empty(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return 0;
	thiz = (cvector_data*) _thiz;
	if (thiz->typesize <= 0)
		return 0;
	if (thiz->first == thiz->last)
		return 1;
	return 0;
}

/*   resize: malloc n * typesize bytes
 *   thiz: cvector pointer
 *   n:    n items  equal val
 *   val:  item pointer
 */
static    void    cvector_static_resize(cvector *_thiz, uint64_t n, const void* val) {
	cvector_data *thiz = NULL;
	if ((_thiz == NULL) || (n <= 0) || (val == NULL))
		return;

	thiz = (cvector_data*) _thiz;
    if (thiz->first == NULL) {
        thiz->first = malloc(n * thiz->typesize);
        thiz->final = thiz->first + n * thiz->typesize;
    } else {
        if (n > _thiz->capacity(_thiz)) {
            free(thiz->first);
            thiz->first = malloc(n * thiz->typesize);
            thiz->final  = thiz->first + n * thiz->typesize;
        }
    }

    thiz->last = thiz->first;
    for (uint64_t i = 0; i < n; ++i, thiz->last += thiz->typesize) {
        memcpy(thiz->last, val, thiz->typesize);
    }	
}

/*   reserve: change capacity items
 *   thiz: cvector pointer
 *   capacity:   max item count
 */
static    void    cvector_static_reserve(cvector *_thiz, uint64_t capacity) {
	void* data = NULL;
	uint64_t max_size = 0;
	cvector_data *thiz = NULL;
	if ((_thiz == NULL) || (capacity <= 0))
		return;
	thiz = (cvector_data*) _thiz;
    capacity = capacity * thiz->typesize;
    max_size = thiz->final - thiz->first;
    if (max_size == capacity)
        return;
    data = malloc(capacity);
    max_size = thiz->last - thiz->first;
    if (max_size > capacity)
        max_size = capacity;
    if (max_size > 0)
        memcpy(data, thiz->first, max_size);
    free(thiz->first);
    thiz->first = data;
    thiz->last  = data + max_size;
    thiz->final   = data + capacity;
}

/*   back: last item pointer
 *   thiz: cvector pointer
 *   return last item pointer
 */
static    void*    cvector_static_back(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cvector_data*) _thiz;
	if (thiz->first == thiz->last)
		return NULL;
    return (thiz->last - thiz->typesize);
}

/*   front: first item pointer
 *   thiz: cvector pointer
 *   return first item pointer
 */
static    void*    cvector_static_front(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cvector_data*) _thiz;
	if (thiz->first == thiz->last)
		return NULL;
	return thiz->first;
}

/*   at: index item pointer
 *   thiz: cvector pointer
 *   index: item index
 *   return index item pointer
 */
static    void*    cvector_static_at(cvector *_thiz, uint64_t index) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cvector_data*) _thiz;
    if (index >= _thiz->size(_thiz))
        return NULL;
    return (thiz->first + index * thiz->typesize);
}

/*   data: first item pointer
 *   thiz: cvector pointer
 *   return first item pointer
 */
static    void*    cvector_static_data(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cvector_data*) _thiz;
	return thiz->first;	
}

/*   begin: first item pointer
 *   thiz: cvector pointer
 *   return first item pointer
 */
static    void*    cvector_static_begin(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cvector_data*) _thiz;
	return thiz->first;
}

/*   end: last item pointer + typesize
 *   thiz: cvector pointer
 *   return last item pointer + typesize
 */
static    void*    cvector_static_end(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cvector_data*) _thiz;
	return thiz->last;
}

/*   rbegin: last item pointer
 *   thiz: cvector pointer
 *   return last item pointer
 */
static    void*    cvector_static_rbegin(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cvector_data*) _thiz;
	if (thiz->first == thiz->last)
		return NULL;
    return (thiz->last - thiz->typesize);
}

/*   back: last item pointer
 *   thiz: cvector pointer
 *   return last item pointer
 */
static    void*    cvector_static_rend(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return NULL;
	thiz = (cvector_data*) _thiz;
	if (thiz->first == thiz->last)
		return NULL;
    return (thiz->first - thiz->typesize);
}

/*   push_back: add last item behind 
 *   thiz: cvector pointer
 *   val:  item pointer
 */
static    void    cvector_static_push_back(cvector *_thiz, const void* val) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
	_thiz->fill(_thiz, thiz->last, 1, val);
}

/*   push_front: add first item before 
 *   thiz: cvector pointer
 *   val:  item pointer
 */
static    void    cvector_static_push_front(cvector *_thiz, const void* val) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
	_thiz->fill(_thiz, thiz->first, 1, val);
}

/*   pop_back: delete last item 
 *   thiz: cvector pointer
 */
static    void    cvector_static_pop_back(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
	if (thiz->first == thiz->last)
		return;
    thiz->last = thiz->last - thiz->typesize;
}

/*   pop_front: delete first item 
 *   thiz: cvector pointer
 */
static    void    cvector_static_pop_front(cvector *_thiz) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
    _thiz->erase(_thiz, thiz->first, thiz->first + thiz->typesize);		
}

/*   erase: delete from first to last items 
 *   thiz: cvector pointer
 *   first: begin item pointer
 *   last: last item pointer
 */
static    void    cvector_static_erase(cvector *_thiz, void* first, void* last) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
    if (first == NULL || last == NULL || first >= last)
        return;
    if (first < thiz->first)
        first = thiz->first;
    if (last > thiz->last)
        last = thiz->last;
    for (;last < thiz->last; first = first + thiz->typesize, last = last + thiz->typesize)
        memcpy(first, last, thiz->typesize);
    thiz->last = first;
}

/*   remove: delete position item 
 *   thiz: cvector pointer
 *   position: item pointer
 */
static    void    cvector_static_remove(cvector *_thiz, void* position) {
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
    return _thiz->erase(_thiz, position, position + thiz->typesize);	
}

/*   assign: copy value from first to last items 
 *   thiz: cvector pointer
 *   first: begin item pointer
 *   last: last item pointer
 */
static    void    cvector_static_assign(cvector *_thiz, void* first, void* last) {
	uint64_t size;
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
    if (first == NULL || last == NULL || first >= last)
        return;
    size = (last - first);

    if (thiz->first == NULL) {
        thiz->first = malloc(size);
        thiz->final  = thiz->first + size;
    } else {
        if (size > (thiz->final - thiz->first)) {
            free(thiz->first);
            thiz->first = malloc(size);
            thiz->final  = thiz->first + size;
        }
    }

    thiz->last = thiz->first + size;
    memcpy(thiz->first, first, size);
}

/*   fill: copy value  n * typesize val from position begin
 *   thiz: cvector pointer
 *   position: item pointer
 *   n:    n items  equal val
 *   val:  item pointer
 */
static    void    cvector_static_fill(cvector *_thiz, void* position, uint64_t n, const void* val) {
	uint64_t newsize, size, capacity, step = 0;
	void* ptr = NULL;
	cvector_data *thiz = NULL;
	if ((_thiz == NULL) || (val == NULL) || (n <= 0) || (position == NULL))
		return;
	thiz = (cvector_data*) _thiz;
    
    newsize  = n * thiz->typesize;
    size     = thiz->last - thiz->first;
    capacity = thiz->final  - thiz->first;

    if (newsize + size > capacity) {
        uint64_t pos = position - thiz->first;
        ptr = malloc(2 * (newsize + size));
        if (pos > 0)
            memcpy(ptr, thiz->first, pos);
        for (step = 0; step < newsize; step = step + thiz->typesize)
            memcpy(ptr + pos + step, val, thiz->typesize);
        if (size > pos)
            memcpy(ptr + pos + newsize, position, size - pos);
        free(thiz->first);
        thiz->first = ptr;
        thiz->last  = ptr + newsize + size;
        thiz->final   = ptr + 2 * (newsize + size);
        return;
    }
    ptr = thiz->last - thiz->typesize;
    for ( ;ptr >= position; ptr = ptr - thiz->typesize) {
        memcpy(ptr + newsize, ptr, thiz->typesize);
    }

    for (step = 0; step < newsize; step = step + thiz->typesize)
        memcpy(position + step, val, thiz->typesize);
    thiz->last = thiz->last + newsize;
}

/*   insert: insert values from first to last items  at position
 *   thiz: cvector pointer
 *   position: item pointer
 *   first: begin item pointer
 *   last: last item pointer
 */
static    void    cvector_static_insert(cvector *_thiz, void* position, void* first, void* last) {
	uint64_t newsize, size, capacity;
	void* ptr = NULL;
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
    if (position == NULL || first == NULL || last == NULL)
        return;
    if (position < thiz->first || position > thiz->last || first >= last)
        return;
    
    newsize  = last - first;
    size     = thiz->last - thiz->first;
    capacity = thiz->final  - thiz->first;

    if (newsize + size > capacity) {
        uint64_t pos = position - thiz->first;
        ptr = malloc(2 * (newsize + size));
        if (pos > 0)
            memcpy(ptr, thiz->first, pos);
        memcpy(ptr + pos, first, newsize);
        if (size > pos)
            memcpy(ptr + pos + newsize, position, size - pos);
        free(thiz->first);
        thiz->first = ptr;
        thiz->last  = ptr + newsize + size;
        thiz->final   = ptr + 2 * (newsize + size);
        return;
    }
    ptr = thiz->last - thiz->typesize;
    for ( ;ptr >= position; ptr = ptr - thiz->typesize) {
        memcpy(ptr + newsize, ptr, thiz->typesize);
    }
    memcpy(position, first, newsize);
    thiz->last = thiz->last + newsize;
}

/*   reverse: first and last items change 
 *   thiz: cvector pointer
 */
static    void    cvector_static_reverse(cvector *_thiz) {
	void *first, *last, *val;
	cvector_data *thiz = NULL;
	if (_thiz == NULL) 
		return;
	thiz = (cvector_data*) _thiz;
    if (thiz->first == thiz->last)
        return;

    first = thiz->first;
    last = thiz->last - thiz->typesize;
    val  = malloc(thiz->typesize);
    for (; first < last; first = first + thiz->typesize, last = last - thiz->typesize) {
        memcpy(val,   first, thiz->typesize);
        memcpy(first, last,  thiz->typesize);
        memcpy(last,  val,   thiz->typesize);
    }
    free(val);
}

/*   copy: copy value from thiz to that
 *   thiz: cvector pointer
 *   that: cvector pointer
 */
static    void    cvector_static_copy(cvector *_thiz, cvector *_that) {
	cvector_data *thiz = NULL, *that = NULL;
	if ((_thiz == NULL) || (_that == NULL)) {
		return;
	}
	thiz = (cvector_data*) _thiz;
	that = (cvector_data*) _that;
	_that->assign(_that, thiz->first, thiz->last);
	that->typesize = thiz->typesize;
}

/*   equal: compare thiz with that
 *   thiz: cvector pointer
 *   that: cvector pointer
 *   return: thiz == that
 */
static    uint8_t    cvector_static_equal(cvector *_thiz, cvector *_that) {
	cvector_data *thiz = NULL, *that = NULL;
	if ((_thiz == NULL) || (_that == NULL)) {
		return 0;
	}
	thiz = (cvector_data*) _thiz;
	that = (cvector_data*) _that;
	if (thiz->typesize != that->typesize) {
		return 0;
	}

	if ((thiz->first == NULL) || (that->first == NULL)) {
		return 0;
	}

    if (_thiz->size(_thiz) != _that->size(_that)) {
    	return 0;
    }

    if (memcmp(thiz->first, that->first, _thiz->size(_thiz)) != 0) {
    	return 0;
    }
    return 1;	
}

/*   cvector_alloc: malloc cvector pointer
 *   size:     cvector item count
 *   typesize: cvector item size
 *   return: cvector pointer
 */
cvector* cvector_alloc(uint64_t size, uint64_t typesize) {
	cvector *thiz = NULL;
	cvector_data *thiz_data = NULL;
	if ((size <= 0) || (typesize <= 0)) {
		return NULL;
	}

	thiz_data = (cvector_data *)malloc(sizeof(cvector_data));
	if (thiz_data == NULL) {
		return NULL;
	}

	thiz_data->first = malloc(size * typesize);
	if (thiz_data->first == NULL) {
		free(thiz_data);
		return NULL;
	}

    thiz_data->typesize = typesize;
    thiz_data->last = thiz_data->first;
    thiz_data->final  = thiz_data->first + size * typesize;

	thiz = (cvector*) &(thiz_data->vector);

	thiz->clear = cvector_static_clear;
	thiz->free  = cvector_static_free;
	thiz->typesize  = cvector_static_typesize;
	thiz->size  = cvector_static_size;
	thiz->capacity  = cvector_static_capacity;
	thiz->empty  = cvector_static_empty;
	thiz->resize  = cvector_static_resize;
	thiz->reserve  = cvector_static_reserve;
	thiz->back  = cvector_static_back;
	thiz->front  = cvector_static_front;
	thiz->at  = cvector_static_at;
	thiz->data  = cvector_static_data;
	thiz->begin  = cvector_static_begin;
	thiz->end  = cvector_static_end;
	thiz->rbegin  = cvector_static_rbegin;
	thiz->rend  = cvector_static_rend;
	thiz->push_back  = cvector_static_push_back;
	thiz->push_front  = cvector_static_push_front;
	thiz->pop_back  = cvector_static_pop_back;
	thiz->pop_front  = cvector_static_pop_front;
	thiz->erase  = cvector_static_erase;
	thiz->remove  = cvector_static_remove;
	thiz->assign  = cvector_static_assign;
	thiz->fill  = cvector_static_fill;
	thiz->insert  = cvector_static_insert;
	thiz->reverse  = cvector_static_reverse;
	thiz->copy  = cvector_static_copy;
	thiz->equal  = cvector_static_equal;

    return thiz;
}
