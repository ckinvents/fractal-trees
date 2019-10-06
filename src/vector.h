#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct _Vector {
    int x;
    int y;
} Vector;

void Vector_setCoords(Vector* vec, int newX, int newY);
void Vector_setPolar(Vector* vec, double angle, double length);
void Vector_addVector(Vector* vec1, Vector* vec2, Vector* result);
double Vector_getAngle(Vector* vec);
double Vector_getLength(Vector* vec);
double Vector_getAngleDif(Vector* vec1, Vector* vec2);

#endif //VECTOR_H