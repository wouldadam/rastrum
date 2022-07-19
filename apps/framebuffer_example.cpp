/**
 * Example of using a FrameBuffer to write an image file.
 */

#include <iostream>

#include "rastrum/FrameBuffer.h"

constexpr size_t kBufferWidth = 1920;
constexpr size_t kBufferHeight = 1080;
constexpr auto kLinesPerSegment = 20;
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
      buffer.set(Point{w, h}, col);
    }
  }

  // Draw a red line straight across the middle
  for (auto w = 0; w < kBufferWidth; ++w) {
    buffer.set((kBufferHeight / 2 * kBufferWidth) + w, {rastrum::kColMax, 0, 0, rastrum::kColMax});
  }

  // Draw lines from the center to all sides of the frame
  const Point center{kBufferWidth / 2, kBufferHeight / 2};

  for (auto x = 0; x <= kBufferWidth; x += kBufferWidth / kLinesPerSegment) {
    buffer.line(center, Point{x, 0}, rastrum::RGBA{0, 0, rastrum::kColMax, rastrum::kColMax});

    buffer.line(center, Point{x, kBufferHeight - 1},
                rastrum::RGBA{rastrum::kColMax, 0, rastrum::kColMax, rastrum::kColMax});
  }

  for (auto y = 0; y <= kBufferHeight; y += kBufferHeight / kLinesPerSegment) {
    buffer.line(center, Point{0, y}, rastrum::RGBA{0, rastrum::kColMax, 0, rastrum::kColMax});

    buffer.line(center, Point{kBufferWidth - 1, y},
                rastrum::RGBA{rastrum::kColMax, rastrum::kColMax, 0, rastrum::kColMax});
  }

  // Write to an image
  buffer.write_bmp(kOutputFile);
  std::cout << "Image written to " << kOutputFile << ".\n";
}