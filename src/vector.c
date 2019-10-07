#include "vector.h"
#include <math.h>

/**
 * Vector
 * 
 * Handles basic cartesian
 * coordinate vectors for
 * reprentation/computation of
 * branches for tree struct
 * 
 * Connor 2019
 **/

// Sets coords of vector
void Vector_setCoords(Vector* vec, int newX, int newY)
{
    vec->x = newX;
    vec->y = newY;
}

// Sets coords from polar data
void Vector_setPolar(Vector* vec, double angle, double length)
{
    vec->x = (int)(cos(angle) * length);
    vec->y = (int)(sin(angle) * length);
}

// Adds two vectors and stores result in a third
void Vector_addVector(Vector* vec1, Vector* vec2, Vector* result)
{
    int newX = vec1->x + vec2->x;
    int newY = vec1->y + vec2->y;
    Vector_setCoords(result, newX, newY);
}

// Returns angle of vector from horizontal
double Vector_getAngle(Vector* vec)
{
    return atan2((double)vec->y, (double)vec->x);
}

// Returns length of vector
double Vector_getLength(Vector* vec)
{
    double newX = vec->x;
    double newY = vec->y;
    return sqrt(newX * newX + newY * newY);
}

double Vector_getAngleDif(Vector* vec1, Vector* vec2)
{
    return Vector_getAngle(vec1) - Vector_getAngle(vec2);
}
