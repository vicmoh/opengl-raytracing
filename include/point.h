#ifndef POINT_H
#define POINT_H

#include "dynamic_string.h"
#include <math.h>

typedef struct {
  float x, y, z;
  char toString[1024];
} Point;

/**
 * Create a new point object.
 * @param x value of the point.
 * @param y value of the point.
 * @param z value of the point
 * @return Point object.
 */
Point new_PointOf(float x, float y, float z);

/**
 * Create a new point object.
 * @return Point object.
 */
Point new_Point();

/**
 * Free the point object.
 * @param self Point object to be freed.
 */
void free_Point(Point self);

/**
 * Point to string.
 * @param self of the point to create the string.
 * @return string of the point.
 */
String pointToString(Point self);

/**
 * Calculate the vector length.
 * @param self point.
 * @return Float of the vector length.
 */
float calcVecLength(Point self);

/* creates a unit vector */
/* divide a vector by its own length */
/**
 * Create a unit vector.
 * Divide a vector by it's own length.
 * @param self point to be normalized.
 * @return a new point.
 */
Point normalizePoint(Point self);

/* -------------------------------------------------------------------------- */
/*                            Calculation functions                           */
/* -------------------------------------------------------------------------- */

/** 
 * Calculate the two intersection.
 * @param a first point.
 * @param b second point.
 * @return intersection value.
 */
float calcDistForIntersection(Point a, Point b);

/**
 * Calculate the distance of two points.
 * @param a first point.
 * @param b second point.
 * @return a new point.
 */
Point calcDistVector(Point a, Point b);

/**
 * Calculate dot product of two points.
 * @param a first point.
 * @param b second point.
 */
float calcDotProduct(Point a, Point b);

/**
 * Calculate the reflect vector.
 * @param nl is the normal and light vector. Dot producted.
 * @param a first point.
 * @param b second point.
 * @return a new point.
 */
Point calcReflectVector(float nl, Point a, Point b);

#endif
