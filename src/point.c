#include "point.h"
#include <math.h>
#include "dynamic_string.h"

Point new_PointOf(float x, float y, float z) {
  Point new = {.x = x, .y = y, .z = z};
  sprintf(new.toString, "x: %f, y: %f, x: %f", new.x, new.y, new.z);
  return new;
}

Point new_Point() { return new_PointOf(0, 0, 0); }

String pointToString(Point this) { return $(this.toString); }

float calcVecLength(Point this) {
  return (sqrtf((this.x * this.x) + (this.y * this.y) + (this.z * this.z)));
}

Point normalizePoint(Point this) {
  float len = calcVecLength(this);
  return new_PointOf(this.x / len, this.y / len, this.z / len);
}

/* -------------------------------------------------------------------------- */
/*                            Calculation functions                           */
/* -------------------------------------------------------------------------- */

Point calcDistVector(Point a, Point b) {
  return normalizePoint(new_PointOf(a.x - b.x, a.y - b.y, a.z - b.z));
}

float calcDotProduct(Point a, Point b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Point calcReflectVector(float nl, Point n, Point l) {
  return normalizePoint(
      new_PointOf(2 * nl * n.x - l.x, 2 * nl * n.y - l.y, 2 * nl * n.z - l.z));
}
