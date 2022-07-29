#ifndef RASTRUM_VECTOR2D_H
#define RASTRUM_VECTOR2D_H

namespace rastrum {

/**
 * Represents a 2D vector.
 */
template <typename T>
struct Vector2D {
  T x;
  T y;
};

/** An 2D vector using floats. */
typedef Vector2D<float> Vector2DF;

/** An integer 2D vector. */
typedef Vector2D<int> Pixel;

}  // namespace rastrum

#endif