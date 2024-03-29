#ifndef RASTRUM_FRAMEBUFFER_H
#define RASTRUM_FRAMEBUFFER_H

#include <limits>
#include <string>
#include <vector>

#include "rastrum/Vector.h"

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
  void set(size_t idx, RGBA value, float z);

  /** Set a pixel to the specified value based on x/y position. */
  void set(Pixel point, RGBA value, float z);

  /** Draws a line from start to end with the specified color and z-order. */
  void line(Vector3DF start, Vector3DF end, RGBA value);

  /** Draws a wireframe triangle at the 3 specified points. */
  void triangle(Vector3DF a, Vector3DF b, Vector3DF c, RGBA value);

  /** Draws a filled triangle. */
  void fillTriangle(Vector3DF a, Vector3DF b, Vector3DF c, RGBA value);

  /** Write the current buffer as a BMP to the specified file. */
  void writeBmp(const std::string& filename) const;

  /** Write the current buffer to the terminal. */
  void writeConsole() const;

 private:
  /** Line drawing for slopes between 0 and -1. */
  void lineLow(Vector3DF start, Vector3DF end, RGBA value);
  /** Line drawing for positive or negative steep slopes. */
  void lineHigh(Vector3DF start, Vector3DF end, RGBA value);

  /**
   * Edge function, calculates which "side" a point p lies on the edge ab.
   * Positive = right
   * Zero = on the edge
   * Negative = left
   */
  static auto edge(Vector3DF a, Vector3DF b, Vector3DF p) -> float;

  size_t _width;
  size_t _height;
  std::vector<RGBA> _data;
  std::vector<float> _z_buffer;
};

}  // namespace rastrum

#endif