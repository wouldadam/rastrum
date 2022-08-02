/**
 * Example of loading and rendering an .obj model.
 * Accepts the following command line args:
 * -w  Render as a wireframe
 */

#include <cstring>
#include <iostream>
#include <random>

#include "rastrum/FrameBuffer.h"
#include "rastrum/Obj.h"

using namespace rastrum;

constexpr size_t kBufferWidth = 2048;
constexpr size_t kBufferHeight = 2048;
constexpr auto kOutputFile = "image.bmp";

// The direction of the light source
const Vector3DF kLight = Vector3DF{{0, 0, -20}}.normalize();

/**
 * A very simple orthographic projection. Drops the Z axis, interpolates from min to max. */
auto ortho(Vector3DF vert, Vector3DF min, Vector3DF max) -> Vector2DF {
  Vector3DF delta{{max.x() - min.x(), max.y() - min.y(), max.z() - min.z()}};

  float x = (kBufferWidth - 1) * (vert.x() - min.x()) / delta.x();
  float y = (kBufferHeight - 1) * (vert.y() - min.y()) / delta.y();

  // Invert the Y values as obj uses a right-hand coordinate system. */
  y = static_cast<int>(kBufferHeight) - 1 - y;

  return Vector2DF{{x, y}};
}

auto main(int argc, char* argv[]) -> int {
  // Parse the command line options
  bool wireframe = false;
  int min_color = kColMax / 2;
  int max_color = kColMax / 2;
  if (argc > 1) {
    for (int arg_idx = 1; arg_idx < argc; ++arg_idx) {
      if (strcmp(argv[arg_idx], "-w") == 0) {
        std::cout << "WF\n";
        wireframe = true;
      }
      if (strcmp(argv[arg_idx], "-c") == 0) {
        std::cout << "COL\n";
        min_color = 0;
        max_color = kColMax / 2;
      }
    }
  }

  FrameBuffer buffer(kBufferWidth, kBufferHeight);

  std::cout << "Creating " << buffer.width() << "x" << buffer.height() << " image...\n";

  // Load the model
  const auto model = obj::load("../data/centurion_helmet/centurion_helmet.obj");

  const auto& verts = model.vertices();
  const auto& indices = model.vert_indices();

  // Find the min and max values of the verts as we are going the fill the screen with the model
  Vector3DF min = Vector3DF::max();
  Vector3DF max = Vector3DF::min();
  for (const auto& vert : verts) {
    min = rastrum::min(min, vert);
    max = rastrum::max(max, vert);
  }

  // RNG for each poly's color
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(min_color, max_color);

  // Project and draw each triangle
  for (size_t face_idx = 0; face_idx < model.face_count(); ++face_idx) {
    const auto face = model.face(face_idx);

    if (wireframe) {
      buffer.triangle(ortho(face[0], min, max), ortho(face[1], min, max), ortho(face[2], min, max),
                      RGBA{(unsigned char)(dist(rng) * 2), (unsigned char)(dist(rng) * 2),
                           (unsigned char)(dist(rng) * 2), kColMax});
    } else {
      // Use the dot product of the face's normal for some basic shading
      const auto norm = normal(face[0], face[1], face[2]).normalize();
      const auto dot = std::abs(norm.dot(kLight));

      if (dot > 0.0F) {
        const auto intensity = dot + 1.0F;
        buffer.fillTriangle(
            ortho(face[0], min, max), ortho(face[1], min, max), ortho(face[2], min, max),
            RGBA{(unsigned char)(dist(rng) * intensity), (unsigned char)(dist(rng) * intensity),
                 (unsigned char)(dist(rng) * intensity), kColMax});
      }
    }
  }

  // Write to an image
  buffer.writeBmp(kOutputFile);
  std::cout << "Image written to " << kOutputFile << ".\n";
}