/**
 * Example of using a FrameBuffer to write an image file.
 */

#include <iostream>

#include "rastrum/FrameBuffer.h"

constexpr size_t kBufferWidth = 1920;
constexpr size_t kBufferHeight = 1080;
constexpr auto kLinesPerSegment = 20;
constexpr auto kOutputFile = "image.bmp";

using namespace rastrum;

auto main() -> int {
  FrameBuffer buffer(kBufferWidth, kBufferHeight);

  std::cout << "Creating " << buffer.width() << "x" << buffer.height() << " image...\n";

  // Fill the buffer with a pattern of colors
  for (auto w = 0; w < kBufferWidth; ++w) {
    for (auto h = 0; h < kBufferHeight; ++h) {
      RGBA col{static_cast<unsigned char>(w % kColMax), static_cast<unsigned char>(h % kColMax),
               static_cast<unsigned char>(w + h % kColMax), kColMax};
      buffer.set(Pixel{{w, h}}, col, 0);
    }
  }

  // Draw a red line straight across the middle
  for (auto w = 0; w < kBufferWidth; ++w) {
    buffer.set((kBufferHeight / 2 * kBufferWidth) + w, {kColMax, 0, 0, kColMax}, 1);
  }

  // Draw lines from the center to all sides of the frame
  const Vector3DF center{{kBufferWidth / 2.0, kBufferHeight / 2.0, 1}};

  for (auto x = 0; x < kBufferWidth; x += kBufferWidth / kLinesPerSegment) {
    buffer.line(center, Vector3DF{{(float)x, 0, 1}}, RGBA{0, 0, kColMax, kColMax});

    buffer.line(center, Vector3DF{{(float)x, kBufferHeight - 1, 1}},
                RGBA{kColMax, 0, kColMax, kColMax});
  }

  for (auto y = 0; y < kBufferHeight; y += kBufferHeight / kLinesPerSegment) {
    buffer.line(center, Vector3DF{{0, (float)y, 1}}, RGBA{0, kColMax, 0, kColMax});

    buffer.line(center, Vector3DF{{kBufferWidth - 1, (float)y, 1}},
                RGBA{kColMax, kColMax, 0, kColMax});
  }

  // Write to an image
  buffer.writeBmp(kOutputFile);
  std::cout << "Image written to " << kOutputFile << ".\n";
}