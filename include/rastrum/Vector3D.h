#ifndef RASTRUM_VECTOR3D_H
#define RASTRUM_VECTOR3D_H

namespace rastrum {

/**
 * Represents a 3D vector.
 */
template <typename T>
struct Vector3D {
  T x;
  T y;
  T z;
};

/** An 3D vector using floats. */
typedef Vector3D<float> Vector3DF;

}  // namespace rastrum
#endif