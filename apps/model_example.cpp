/**
 * Example of loading and rendering an .obj model.
 */

#include <iostream>

#include "rastrum/FrameBuffer.h"
#include "rastrum/Obj.h"

constexpr size_t kBufferWidth = 2048;
constexpr size_t kBufferHeight = 2048;
constexpr auto kOutputFile = "image.bmp";

using namespace rastrum;

/**
 * A very simple orthographic projection. Drops the Z axis, interpolates from min to max. */
auto ortho(Vector3DF vert, Vector3DF min, Vector3DF max) -> Point {
  Vector3DF delta{max.x - min.x, max.y - min.y, max.z - min.z};

  int x = static_cast<int>((kBufferWidth - 1) * (vert.x - min.x) / delta.x);
  int y = static_cast<int>((kBufferHeight - 1) * (vert.y - min.y) / delta.y);

  // Invert the Y values as obj uses a right-hand coordinate system. */
  y = static_cast<int>(kBufferHeight) - 1 - y;

  return Point{x, y};
}

auto main() -> int {
  FrameBuffer buffer(kBufferWidth, kBufferHeight);

  std::cout << "Creating " << buffer.width() << "x" << buffer.height() << " image...\n";

  // Load the model
  const auto model = obj::load("../data/centurion_helmet/centurion_helmet.obj");

  const auto& verts = model.vertices();
  const auto& indices = model.vert_indices();

  // Find the min and max values of the verts as we are going the fill the screen with the model
  Vector3DF min{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                std::numeric_limits<float>::max()};
  Vector3DF max{std::numeric_limits<float>::min(), std::numeric_limits<float>::min(),
                std::numeric_limits<float>::min()};
  for (const auto& vert : verts) {
    min.x = std::min(min.x, vert.x);
    min.y = std::min(min.y, vert.y);
    min.z = std::min(min.z, vert.z);

    max.x = std::max(max.x, vert.x);
    max.y = std::max(max.y, vert.y);
    max.z = std::max(max.z, vert.z);
  }

  // Project and draw each triangle
  for (size_t face_idx = 0; face_idx < model.face_count(); ++face_idx) {
    const auto face = model.face(face_idx);

    buffer.triangle(ortho(face[0], min, max), ortho(face[1], min, max), ortho(face[2], min, max),
                    RGBA{kColMax, 0, 0, kColMax});
  }

  // Write to an image
  buffer.write_bmp(kOutputFile);
  std::cout << "Image written to " << kOutputFile << ".\n";
}