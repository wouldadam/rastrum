#ifndef RASTRUM_VECTOR2D_H
#define RASTRUM_VECTOR2D_H

/**
 * Represents a 2D vector.
 */
template<typename T>
struct Vector2D {
  T x;
  T y;
};

/** An integer 2D vector. */
typedef Vector2D<int> Point;

#endif