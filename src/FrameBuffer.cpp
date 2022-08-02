#include "rastrum/FrameBuffer.h"

#include <cmath>
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

void rastrum::FrameBuffer::set(Pixel point, RGBA value) {
  const auto idx = (point.y() * _width) + point.x();
  if (idx >= _data.size()) {
    std::cerr << "Attempted to access outside of framebuffer bounds: " << idx << "," << point
              << "\n";
    exit(1);
  }

  _data[idx] = value;
}

void rastrum::FrameBuffer::line(Vector2DF start, Vector2DF end, RGBA value) {
  // Uses https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

  Pixel start_pixel = start.round().as<int>();
  Pixel end_pixel = end.round().as<int>();

  if (std::abs(end.y() - start.y()) < std::abs(end.x() - start.x())) {
    if (start.x() > end.x()) {
      lineLow(end_pixel, start_pixel, value);
    } else {
      lineLow(start_pixel, end_pixel, value);
    }
  } else {
    if (start.y() > end.y()) {
      lineHigh(end_pixel, start_pixel, value);
    } else {
      lineHigh(start_pixel, end_pixel, value);
    }
  }
}

void rastrum::FrameBuffer::triangle(Vector2DF a, Vector2DF b, Vector2DF c, RGBA value) {
  line(a, b, value);
  line(b, c, value);
  line(c, a, value);
}

void rastrum::FrameBuffer::fillTriangle(Vector2DF a, Vector2DF b, Vector2DF c, RGBA value) {
  // Calculate the bounding box so we don't have to test every pixel
  const Pixel min = rastrum::min(rastrum::min(a, b), c).floor().as<int>();
  const Pixel max = rastrum::max(rastrum::max(a, b), c).ceil().as<int>();

  // Check each point in the bounding box to see if it is in the triangle
  for (int x = min.x(); x < max.x(); ++x) {
    for (int y = min.y(); y < max.y(); ++y) {
      Vector2DF p{{static_cast<float>(x), static_cast<float>(y)}};
      bool inside = edge(a, b, p) <= 0;
      inside &= edge(b, c, p) <= 0;
      inside &= edge(c, a, p) <= 0;

      if (inside) {
        set(p.as<int>(), value);
      }
    }
  }
}

void rastrum::FrameBuffer::writeBmp(const std::string& filename) const {
  const auto result = stbi_write_bmp(filename.c_str(), _width, _height, 4, _data.data());
  if (result == 0) {
    std::cerr << "Failed to write image: " << result << "\n";
  }
}

void rastrum::FrameBuffer::lineLow(Pixel start, Pixel end, RGBA value) {
  auto delta = end - start;

  auto yInc = 1;
  if (delta.y() < 0) {
    yInc = -1;
    delta.y(-delta.y());
  }

  const auto deltaDelta = delta.y() - delta.x();

  auto D = (2 * delta.y()) - delta.x();
  auto y = start.y();

  for (auto x = start.x(); x < end.x(); ++x) {
    set(Pixel{{x, y}}, value);

    if (D > 0) {
      y += yInc;
      D += 2 * deltaDelta;
    } else {
      D += 2 * delta.y();
    }
  }
}

void rastrum::FrameBuffer::lineHigh(Pixel start, Pixel end, RGBA value) {
  auto delta = end - start;

  auto xInc = 1;
  if (delta.x() < 0) {
    xInc = -1;
    delta.x(-delta.x());
  }

  const auto deltaDelta = delta.x() - delta.y();

  auto D = (2 * delta.x()) - delta.y();
  auto x = start.x();

  for (auto y = start.y(); y < end.y(); ++y) {
    set(Pixel{{x, y}}, value);

    if (D > 0) {
      x += xInc;
      D += 2 * deltaDelta;
    } else {
      D += 2 * delta.x();
    }
  }
}

auto rastrum::FrameBuffer::edge(Vector2DF a, Vector2DF b, Vector2DF p) -> float {
  // See https://dl.acm.org/doi/10.1145/54852.378457
  const auto val = ((p.x() - a.x()) * (b.y() - a.y()) - (p.y() - a.y()) * (b.x() - a.x()));
  return val;
}