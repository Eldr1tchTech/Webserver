#pragma once

typedef struct darray
{
    int size;
    int type_size;
    void *data;
    int capacity;
} darray;

darray* darray_create(int type_size, int init_cap);

// Increases the capacity by incr_cap
void darray_grow(darray* arr, int incr_cap);

// Decreases the capacity 
void darray_shrink(darray* arr, int decr_cap);

void darray_destroy(darray* arr);

void darray_add(darray* arr, void* e);

void* darray_get(darray* arr, int i);