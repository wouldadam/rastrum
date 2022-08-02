#ifndef RASTRUM_MODEL_H
#define RASTRUM_MODEL_H

#include <array>
#include <vector>

#include "rastrum/Vector.h"

namespace rastrum {

constexpr int kModelFaceSize = 3;

/**
 * Represents a loaded model.
 * A model is represented by:
 * - Vertices - a list of all the vertices used in the model
 * - Vertex indices - Groups of kModelFaceSize indices into the vertices
 *   that describe a face of the model.
 */
class Model {
 public:
  Model(std::vector<Vector3DF> vertices, std::vector<size_t> vert_indices);

  /** Gets the raw verts for the model. */
  auto vertices() const -> const std::vector<Vector3DF>&;

  /** Gets the raw indices for the model. */
  auto vert_indices() const -> const std::vector<size_t>&;

  /** Indicates how many faces are in the model. */
  auto face_count() const -> size_t;

  /** Gets the verts for the specified face. */
  auto face(size_t idx) const -> std::array<Vector3DF, kModelFaceSize>;

 private:
  std::vector<Vector3DF> _vertices;
  std::vector<size_t> _vert_indices;
};

}  // namespace rastrum

#endif