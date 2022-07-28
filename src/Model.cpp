#include "rastrum/Model.h"

#include <iostream>

rastrum::Model::Model(std::vector<Vector3DF> vertices, std::vector<size_t> vert_indices)
    : _vertices(std::move(vertices)), _vert_indices(std::move(vert_indices)) {
  if (_vert_indices.size() % kModelFaceSize != 0) {
    std::cerr << "Receive invalid model indices count\n";
    exit(1);
  }
}

auto rastrum::Model::vertices() const -> const std::vector<rastrum::Vector3DF>& {
  return _vertices;
}

auto rastrum::Model::vert_indices() const -> const std::vector<size_t>& {
  return _vert_indices;
}

auto rastrum::Model::face_count() const -> size_t {
  return _vert_indices.size() / kModelFaceSize;
}

auto rastrum::Model::face(size_t idx) const
    -> std::array<rastrum::Vector3DF, rastrum::kModelFaceSize> {
  if (idx >= face_count()) {
    std::cerr << "Attempted to access OOB model face.\n";
    exit(1);
  }

  const auto base = idx * kModelFaceSize;
  return {_vertices[_vert_indices[base]], _vertices[_vert_indices[base + 1]],
          _vertices[_vert_indices[base + 2]]};
}