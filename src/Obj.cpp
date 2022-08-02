#include "rastrum/Obj.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include "rastrum/Vector.h"

namespace {
/**
 * Splits a string_view into a vector of string_views.
 * It seems you actually need to be using C++23 and the trunk of gcc to do anything useful
 * with std::views::split.
 */
auto split(std::string_view strv, std::string_view delims) -> std::vector<std::string_view> {
  std::vector<std::string_view> output;
  size_t first = 0;

  while (first < strv.size()) {
    const auto second = strv.find_first_of(delims, first);

    if (first != second) {
      output.emplace_back(strv.substr(first, second - first));
    }

    if (second == std::string_view::npos) {
      break;
    }

    first = second + 1;
  }

  return output;
}
}  // namespace

auto rastrum::obj::load(const std::string& filename) -> rastrum::Model {
  std::vector<Vector3DF> vertices;
  std::vector<size_t> vert_indices;
  size_t max_index = 0;

  std::ifstream file(filename);
  if (!file.good()) {
    std::cerr << "Failed to open: " << filename << "\n";
  }

  std::string line;
  while (std::getline(file, line)) {
    auto line_view = std::string_view(line);

    if (line_view.starts_with("#")) {
      // Comment - ignore
    } else if (line_view.starts_with("v ")) {
      // Vertex - we ignore everything after X Y Z
      line_view.remove_prefix(2);

      const auto segments = split(line_view, " ");
      if (segments.size() < 3) {
        std::cerr << "Failed to parse vertex line(not enough fields): " << line << "\n";
        exit(1);
      }

      std::array<float, 3> vert{0, 0, 0};
      for (size_t field_idx = 0; field_idx < 3; ++field_idx) {
        try {
          vert[field_idx] = std::stof(segments[field_idx].data());
        } catch (std::invalid_argument&) {
          std::cerr << "Failed to parse vertex line (bad vertex): " << line << "\n";
          exit(1);
        }
      }

      vertices.push_back(Vector3DF{{vert[0], vert[1], vert[2]}});
    } else if (line_view.starts_with("f ")) {
      // Face - we only support triangles
      line_view.remove_prefix(2);

      const auto segments = split(line_view, " ");
      if (segments.size() != 3) {
        std::cerr << "Tried to load model with " << segments.size()
                  << "sided face (Only 3 sided faces are supported).\n";
        exit(1);
      }

      for (const auto element : segments) {
        const size_t index = std::stoul(element.data()) - 1;  // Obj indices are 1 based
        max_index = std::max(max_index, index);
        vert_indices.push_back(index);
      }
    }
  }

  if (max_index >= vertices.size()) {
    std::cerr << "Obj contains indices to verts that don't exist: "
              << "max_index: " << max_index << ", "
              << "vertices.size(): " << vertices.size() << "\n";
    exit(1);
  }

  return Model(vertices, vert_indices);
}