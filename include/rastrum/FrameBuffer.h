#ifndef RASTRUM_FRAMEBUFFER_H
#define RASTRUM_FRAMEBUFFER_H

#include <limits>
#include <string>
#include <vector>

namespace rastrum {

#pragma pack()
/**
 * Represents an RGB-alpha color.
 */
struct RGBA {
  unsigned char r = 0;
  unsigned char g = 0;
  unsigned char b = 0;
  unsigned char a = 255;
};

/** Max value in an RGBA. */
constexpr auto kColMax = std::numeric_limits<unsigned char>::max();

/**
 * Represents a buffer of screen data.
 */
class FrameBuffer {
 public:
  /** Creates a buffer with a specified width and height in pixels. */
  FrameBuffer(size_t width, size_t height);

  auto width() const -> size_t;
  auto height() const -> size_t;
  
  /** Gets the raw data for the buffer. Data is indexed left to right, top to bottom. */
  auto data() const -> const rastrum::RGBA*;

  /** Set a pixel to the specified value based on linear position. */
  void set(size_t idx, RGBA value);

  /** Set a pixel to the specified value based on x/y position. */
  void set(size_t x, size_t y, RGBA value);

  /** Write the current buffer as a BMP to the specified file. */
  void write_bmp(const std::string& filename) const;

 private:
  size_t _width;
  size_t _height;
  std::vector<RGBA> _data;
};

}  // namespace rastrum

#endif