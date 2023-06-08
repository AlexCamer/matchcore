#include <string.h>
#include "utility/alloc.h"
#include "utility/macros.h"
#include "containers/vector.h"

#define VECTOR_DEFAULT_CAPACITY 64

void
Vector_Construct(struct Vector *vector, usize elementSize) {
    vector->data = Calloc(elementSize, VECTOR_DEFAULT_CAPACITY);
    vector->size = 0;
    vector->capacity = VECTOR_DEFAULT_CAPACITY;
    vector->elementSize = elementSize;
}

void
Vector_Destruct(struct Vector *vector) {
    Free(vector->data);
}

void
Vector_Push(struct Vector *vector, void *element) {
    usize elementSize = vector->elementSize;
    if (vector->size == vector->capacity) {
        vector->capacity = MUL2(vector->capacity);
        vector->data = Realloc(vector->data, vector->capacity * elementSize);
    }
    void *destination = vector->data + vector->size++ * elementSize;
    memcpy(destination, element, elementSize);
}

void
Vector_Pop(struct Vector *vector) {
    vector->size--;
}

void *
Vector_Get(struct Vector *vector, usize index) {
    return vector->data + index * vector->elementSize;
}
