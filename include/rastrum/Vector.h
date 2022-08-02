#ifndef RASTRUM_VECTOR_H
#define RASTRUM_VECTOR_H

#include <array>
#include <cmath>
#include <ostream>

namespace rastrum {

/**
 * An n-dimension math vector class
 */
template <typename T, size_t D>
class Vector {
 public:
  using type = T;

  Vector() = default;
  Vector(std::array<T, D> source) : _coords(source) {
  }

  /** Create a Vector with all coords set to the min for the type. */
  static auto min() -> Vector<T, D> {
    std::array<T, D> source;
    source.fill(std::numeric_limits<T>::min());
    return Vector<T, D>(source);
  }

  /** Create a Vector with all coords set to the max for the type. */
  static auto max() -> Vector<T, D> {
    std::array<T, D> source;
    source.fill(std::numeric_limits<T>::max());
    return Vector<T, D>(source);
  }

  // Getters/setters for conventional names of the 1st 4 dimensions

  template <size_t _D = D, std::enable_if_t<(_D > 0), int> = 0>
  auto x() const -> T {
    return (*this)[0];
  }

  template <size_t _D = D, std::enable_if_t<(_D > 1), int> = 0>
  auto y() const -> T {
    return (*this)[1];
  }

  template <size_t _D = D, std::enable_if_t<(_D > 2), int> = 0>
  auto z() const -> T {
    return (*this)[2];
  }

  template <size_t _D = D, std::enable_if_t<(_D > 3), int> = 0>
  auto w() const -> T {
    return (*this)[3];
  }

  template <size_t _D = D, std::enable_if_t<(_D > 0), int> = 0>
  void x(T val) {
    (*this)[0] = val;
  }

  template <size_t _D = D, std::enable_if_t<(_D > 1), int> = 0>
  void y(T val) {
    (*this)[1] = val;
  }

  template <size_t _D = D, std::enable_if_t<(_D > 2), int> = 0>
  void z(T val) {
    (*this)[2] = val;
  }

  template <size_t _D = D, std::enable_if_t<(_D > 3), int> = 0>
  void w(T val) {
    (*this)[3] = val;
  }

  // Array-like accessors

  auto operator[](size_t idx) -> T& {
    return _coords[idx];
  }

  auto operator[](size_t idx) const -> T {
    return _coords[idx];
  }

  /**
   * Changes the type of the Vector (truncates)
   */
  template <typename A>
  auto as() const -> Vector<A, D> {
    std::array<A, D> coords;
    std::transform(_coords.begin(), _coords.end(), coords.begin(),
                   [](const auto& ele) -> A { return static_cast<A>(ele); });

    return Vector<A, D>{coords};
  }

  /** Returns a new vector with each coord being ceil(coord). */
  auto ceil() const -> Vector<T, D> {
    std::array<T, D> coords;
    std::transform(_coords.begin(), _coords.end(), coords.begin(),
                   [](const auto& ele) -> T { return static_cast<T>(std::ceil(ele)); });

    return Vector<T, D>{coords};
  }

  /** Returns a new vector with each coord being floor(coord). */
  auto floor() const -> Vector<T, D> {
    std::array<T, D> coords;
    std::transform(_coords.begin(), _coords.end(), coords.begin(),
                   [](const auto& ele) -> T { return static_cast<T>(std::floor(ele)); });

    return Vector<T, D>{coords};
  }

  /** Returns a new vector with each coord being round(coord). */
  auto round() const -> Vector<T, D> {
    std::array<T, D> coords;
    std::transform(_coords.begin(), _coords.end(), coords.begin(),
                   [](const auto& ele) -> T { return static_cast<T>(ele); });

    return Vector<T, D>{coords};
  }

  /** Calculate the dot product of this vector and another vector. */
  auto dot(const Vector<T, D>& v2) const -> T {
    T dot_product = 0;

    for (size_t idx = 0; idx < D; ++idx) {
      dot_product += (*this)[idx] * v2[idx];
    }

    return dot_product;
  }

  /** Calculate the cross product of this vector and another vector. */
  template <size_t _D = D, std::enable_if_t<(_D == 3), int> = 0>
  auto cross(const Vector<T, D>& v2) const -> Vector<T, D> {
    Vector<T, D> cross_product;
    ;

    cross_product.x((this->y() * v2.z()) - (this->z() * v2.y()));
    cross_product.y((this->z() * v2.x()) - (this->x() * v2.z()));
    cross_product.z((this->x() * v2.y()) - (this->y() * v2.x()));

    return cross_product;
  }

  /** Calculate the magnitude of the vector. */
  auto length() const -> T {
    T length = 0;

    for (size_t idx = 0; idx < D; ++idx) {
      length += (*this)[idx] * (*this)[idx];
    }

    length = std::sqrt(length);
    return length;
  }

  /** Normalise the vector (magnitude of 1) */
  auto normalize() const -> Vector<T, D> {
    const auto l = length();

    if (l > 0) {
      return (*this) / l;
    }

    return *this;
  }

 private:
  std::array<T, D> _coords;
};

/** Pretty prints a vector to a stream. */
template <typename T, size_t D>
auto operator<<(std::ostream& out, const Vector<T, D>& vec) -> std::ostream& {
  out << "Vec<" << D << ">(";

  for (size_t idx = 0; idx < D; ++idx) {
    out << vec[idx];

    if (idx != D - 1) {
      out << ",";
    }
  }

  out << ")";

  return out;
}

/** Returns the min of each coord of a Vector. */
template <typename T, size_t D>
auto min(const Vector<T, D>& lhs, const Vector<T, D>& rhs) -> Vector<T, D> {
  Vector<T, D> res;

  for (size_t idx = 0; idx < D; ++idx) {
    res[idx] = std::min(lhs[idx], rhs[idx]);
  }

  return res;
}

/** Returns the max of each coord of a Vector. */
template <typename T, size_t D>
auto max(const Vector<T, D>& lhs, const Vector<T, D>& rhs) -> Vector<T, D> {
  Vector<T, D> res;

  for (size_t idx = 0; idx < D; ++idx) {
    res[idx] = std::max(lhs[idx], rhs[idx]);
  }

  return res;
}

/** Vector subtraction. */
template <typename T, size_t D>
auto operator-(const Vector<T, D>& lhs, const Vector<T, D>& rhs) -> Vector<T, D> {
  Vector<T, D> res = lhs;

  for (size_t idx = 0; idx < D; ++idx) {
    res[idx] -= rhs[idx];
  }

  return res;
}

/** Vector division by a scalar. */
template <typename T, size_t D, typename S>
auto operator/(const Vector<T, D>& lhs, S& rhs) -> Vector<T, D> {
  Vector<T, D> res = lhs;

  for (size_t idx = 0; idx < D; ++idx) {
    res[idx] /= rhs;
  }

  return res;
}

/** Calculate the normal for a face described by 3 vectors. */
template <typename T, size_t D>
auto normal(const Vector<T, D>& v1, const Vector<T, D>& v2, const Vector<T, D>& v3)
    -> Vector<T, D> {
  const auto u = v2 - v1;
  const auto v = v3 - v1;
  const auto norm = u.cross(v);
  return norm;
}

/** An 2D vector using floats. */
typedef Vector<float, 2> Vector2DF;

/** An integer 2D vector. */
typedef Vector<int, 2> Pixel;

/** An 3D vector using floats. */
typedef Vector<float, 3> Vector3DF;

}  // namespace rastrum
#endif