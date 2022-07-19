/**
 * Example of using a FrameBuffer to write an image file.
 */

#include <iostream>

#include "rastrum/FrameBuffer.h"

constexpr size_t kBufferWidth = 1920;
constexpr size_t kBufferHeight = 1080;
constexpr auto kOutputFile = "image.bmp";

auto main() -> int {
  rastrum::FrameBuffer buffer(kBufferWidth, kBufferHeight);

  std::cout << "Creating " << buffer.width() << "x" << buffer.height() << " image...\n";

  // Fill the buffer with a pattern of colors
  for (auto w = 0; w < kBufferWidth; ++w) {
    for (auto h = 0; h < kBufferHeight; ++h) {
      rastrum::RGBA col{static_cast<unsigned char>(w % rastrum::kColMax),
                        static_cast<unsigned char>(h % rastrum::kColMax),
                        static_cast<unsigned char>(w + h % rastrum::kColMax), rastrum::kColMax};
      buffer.set(w, h, col);
    }
  }

  // Draw a red line straight across the middle
  for (auto w = 0; w < kBufferWidth; ++w) {
    buffer.set((kBufferHeight / 2 * kBufferWidth) + w, {rastrum::kColMax, 0, 0, rastrum::kColMax});
  }

  // Write to an image
  buffer.write_bmp(kOutputFile);
  std::cout << "Image written to " << kOutputFile << ".\n";
}