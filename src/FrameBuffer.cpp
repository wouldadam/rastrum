#include "rastrum/FrameBuffer.h"

#include <iostream>

#include "stb/stb_image_write.h"

rastrum::FrameBuffer::FrameBuffer(size_t width, size_t height)
    : _width(width), _height(height), _data(width * height) {
}

auto rastrum::FrameBuffer::width() const -> size_t {
  return _width;
}

auto rastrum::FrameBuffer::height() const -> size_t {
  return _height;
}

auto rastrum::FrameBuffer::data() const -> const rastrum::RGBA* {
  return _data.data();
}

void rastrum::FrameBuffer::set(size_t idx, RGBA value) {
  if (idx >= _data.size()) {
    std::cerr << "Attempted to access outside of framebuffer bounds: " << idx << "\n";
    exit(1);
  }
  _data[idx] = value;
}

void rastrum::FrameBuffer::set(Point point, RGBA value) {
  const auto idx = (point.y * _width) + point.x;
  if (idx >= _data.size()) {
    std::cerr << "Attempted to access outside of framebuffer bounds: " << idx << "," << point.x
              << "," << point.y << "\n";
    exit(1);
  }

  _data[idx] = value;
}

void rastrum::FrameBuffer::line(Point start, Point end, RGBA value) {
  // Uses https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

  if (std::abs(end.y - start.y) < std::abs(end.x - start.x)) {
    if (start.x > end.x) {
      lineLow(end, start, value);
    } else {
      lineLow(start, end, value);
    }
  } else {
    if (start.y > end.y) {
      lineHigh(end, start, value);
    } else {
      lineHigh(start, end, value);
    }
  }
}

void rastrum::FrameBuffer::write_bmp(const std::string& filename) const {
  const auto result = stbi_write_bmp(filename.c_str(), _width, _height, 4, _data.data());
  if (result == 0) {
    std::cerr << "Failed to write image: " << result << "\n";
  }
}

void rastrum::FrameBuffer::lineLow(Point start, Point end, RGBA value) {
  const auto deltaX = end.x - start.x;
  auto deltaY = end.y - start.y;

  auto yInc = 1;
  if (deltaY < 0) {
    yInc = -1;
    deltaY = -deltaY;
  }

  const auto deltaDelta = deltaY - deltaX;

  auto D = (2 * deltaY) - deltaX;
  auto y = start.y;

  for (auto x = start.x; x < end.x; ++x) {
    set(Point{x, y}, value);

    if (D > 0) {
      y += yInc;
      D += 2 * deltaDelta;
    } else {
      D += 2 * deltaY;
    }
  }
}

void rastrum::FrameBuffer::lineHigh(Point start, Point end, RGBA value) {
  auto deltaX = end.x - start.x;
  const auto deltaY = end.y - start.y;

  auto xInc = 1;
  if (deltaX < 0) {
    xInc = -1;
    deltaX = -deltaX;
  }

  const auto deltaDelta = deltaX - deltaY;

  auto D = (2 * deltaX) - deltaY;
  auto x = start.x;

  for (auto y = start.y; y < end.y; ++y) {
    set(Point{x, y}, value);

    if (D > 0) {
      x += xInc;
      D += 2 * deltaDelta;
    } else {
      D += 2 * deltaX;
    }
  }
}