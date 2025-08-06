#include "darray.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

darray* darray_create(int type_size, int init_cap) {
    darray* arr = malloc(sizeof(darray));
    if (!arr) {
        printf("darray_create - Failed to allocate memory for darray.\n");
        return NULL;
    }
    
    arr->type_size = type_size;
    if (init_cap < 0) {
        printf("darray_create - Initial capacity cannot be less than 0.\n");
        init_cap = 0;
    }
    
    arr->capacity = init_cap;
    if (init_cap > 0) {
        arr->data = calloc(arr->capacity, arr->type_size);
        if (!arr->data) {
            printf("darray_create - Failed to allocate memory for data.\n");
            free(arr);
            return NULL;
        }
    } else {
        arr->data = NULL;
    }
    arr->size = 0;
    
    // FIX: Return the created array
    return arr;
}

void darray_grow(darray* arr, int incr_cap) {
    int n_cap = arr->capacity + incr_cap;
    if (n_cap == 0) {
        return;
    }
    
    void* new_data = realloc(arr->data, arr->type_size * n_cap);
    if (!new_data && n_cap > 0) {
        printf("darray_grow - Failed to reallocate memory.\n");
        return;
    }
    
    arr->data = new_data;
    // FIX: Update capacity
    arr->capacity = n_cap;
}

void darray_shrink(darray* arr, int decr_cap) {
    int n_cap = arr->capacity - decr_cap;
    if (n_cap < 0) {
        printf("darray_shrink - Tried to decrement capacity below 0. Proceeding with 0.\n");
        n_cap = 0;
    }
    
    if (n_cap == 0) {
        free(arr->data);
        arr->data = NULL;
    } else {
        void* new_data = realloc(arr->data, arr->type_size * n_cap);
        if (!new_data) {
            printf("darray_shrink - Failed to reallocate memory.\n");
            return;
        }
        arr->data = new_data;
    }
    
    arr->capacity = n_cap;
    
    if (arr->size > arr->capacity) {
        arr->size = arr->capacity;
    }
}

void darray_destroy(darray* arr) {
    if (arr) {
        free(arr->data);
        arr->data = NULL;
        free(arr);
    }
}

void darray_add(darray* arr, void* e) {
    if (!arr || !e) {
        printf("darray_add - Invalid parameters.\n");
        return;
    }
    
    if (arr->size >= arr->capacity) {
        // Double the capacity (or start with 1 if capacity is 0)
        int grow_amount = arr->capacity > 0 ? arr->capacity : 1;
        darray_grow(arr, grow_amount);
    }
    
    // Calculate the position where the new element should be added
    void* dest = (char*)arr->data + (arr->size * arr->type_size);
    
    // Copy the element data
    memcpy(dest, e, arr->type_size);
    
    // Increment the size
    arr->size++;
}

void* darray_get(darray* arr, int i) {
    if (!arr) {
        printf("darray_get - Array is NULL.\n");
        return NULL;
    }
    
    // FIX: Should be >= not > for boundary check
    if (i >= arr->size || i < 0) {
        printf("darray_get - Attempted to access an out of bounds index.\n");
        return NULL;
    }
    
    // Calculate and return the address of the element at index i
    return (char*)arr->data + (i * arr->type_size);
}