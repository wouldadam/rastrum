#include "rastrum/FrameBuffer.h"

#include <cmath>
#include <iostream>

#include "stb/stb_image_write.h"
#include "terminal.h"

rastrum::FrameBuffer::FrameBuffer(size_t width, size_t height)
    : _width(width), _height(height), _data(width * height), _z_buffer(width * height) {
  std::fill(_z_buffer.begin(), _z_buffer.end(), std::numeric_limits<float>::lowest());
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

void rastrum::FrameBuffer::set(size_t idx, RGBA value, float z) {
  if (idx >= _data.size()) {
    std::cerr << "Attempted to access outside of framebuffer bounds: " << idx << "\n";
    exit(1);
  }

  if (z >= _z_buffer[idx]) {
    _data[idx] = value;
    _z_buffer[idx] = z;
  }
}

void rastrum::FrameBuffer::set(Pixel point, RGBA value, float z) {
  const auto idx = (point.y() * _width) + point.x();
  if (idx >= _data.size()) {
    std::cerr << "Attempted to access outside of framebuffer bounds: " << idx << "," << point
              << "\n";
    exit(1);
  }

  if (z >= _z_buffer[idx]) {
    _data[idx] = value;
    _z_buffer[idx] = z;
  }
}

void rastrum::FrameBuffer::line(Vector3DF start, Vector3DF end, RGBA value) {
  // Uses https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

  if (std::abs(end.y() - start.y()) < std::abs(end.x() - start.x())) {
    if (start.x() > end.x()) {
      lineLow(end, start, value);
    } else {
      lineLow(start, end, value);
    }
  } else {
    if (start.y() > end.y()) {
      lineHigh(end, start, value);
    } else {
      lineHigh(start, end, value);
    }
  }
}

void rastrum::FrameBuffer::triangle(Vector3DF a, Vector3DF b, Vector3DF c, RGBA value) {
  line(a, b, value);
  line(b, c, value);
  line(c, a, value);
}

void rastrum::FrameBuffer::fillTriangle(Vector3DF a, Vector3DF b, Vector3DF c, RGBA value) {
  // Calculate the bounding box so we don't have to test every pixel
  const Pixel min = rastrum::min(rastrum::min(a, b), c).floor().as<int>().resize<2>();
  const Pixel max = rastrum::max(rastrum::max(a, b), c).ceil().as<int>().resize<2>();

  const float area = edge(a, b, c);

  // Check each point in the bounding box to see if it is in the triangle
  for (int x = min.x(); x < max.x(); ++x) {
    for (int y = min.y(); y < max.y(); ++y) {
      Vector3DF p{{static_cast<float>(x), static_cast<float>(y), 0}};
      const float ab_edge = edge(a, b, p);
      const float bc_edge = edge(b, c, p);
      const float ca_edge = edge(c, a, p);

      const bool inside = (ab_edge >= 0) && (bc_edge >= 0) && (ca_edge >= 0);
      if (inside) {
        // Calculate the z position for this point
        const float ab_bary = ab_edge / area;
        const float bc_bary = bc_edge / area;
        const float ca_bary = ca_edge / area;
        const float z = (a.z() * bc_bary) + (b.z() * ca_bary) + (c.z() * ab_bary);

        set(p.resize<2>().as<int>(), value, z);
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

void rastrum::FrameBuffer::writeConsole() const {
  constexpr auto kBlockChar = "\u2588";

  terminal::clear();

  for (size_t y = 0; y < _height; ++y) {
    for (size_t x = 0; x < _width; ++x) {
      terminal::setColor(_data[(y * _width) + x]);

      // Intentionally written twice as terminal segments are often
      // roughly twice as tall as they are wide.
      std::cout << kBlockChar << kBlockChar;
    }

    std::cout << "\n";
  }

  terminal::reset();
}

void rastrum::FrameBuffer::lineLow(Vector3DF start, Vector3DF end, RGBA value) {
  const auto start_pixel = start.as<int>();
  const auto end_pixel = end.as<int>();
  auto delta = end_pixel - start_pixel;

  auto yInc = 1;
  if (delta.y() < 0) {
    yInc = -1;
    delta.y(-delta.y());
  }

  const auto deltaDelta = delta.y() - delta.x();

  auto D = (2 * delta.y()) - delta.x();
  auto y = start_pixel.y();

  for (auto x = start_pixel.x(); x < end_pixel.x(); ++x) {
    const float z = start.z() + static_cast<float>(delta.z() * delta.x());
    set(Pixel{{(int)x, (int)y}}, value, z);

    if (D > 0) {
      y += yInc;
      D += 2 * deltaDelta;
    } else {
      D += 2 * delta.y();
    }
  }
}

void rastrum::FrameBuffer::lineHigh(Vector3DF start, Vector3DF end, RGBA value) {
  const auto start_pixel = start.as<int>();
  const auto end_pixel = end.as<int>();
  auto delta = end_pixel - start_pixel;

  auto xInc = 1;
  if (delta.x() < 0) {
    xInc = -1;
    delta.x(-delta.x());
  }

  const auto deltaDelta = delta.x() - delta.y();

  auto D = (2 * delta.x()) - delta.y();
  auto x = start_pixel.x();

  for (auto y = start_pixel.y(); y < end_pixel.y(); ++y) {
    const float z = start.z() + static_cast<float>(delta.y() * delta.y());
    set(Pixel{{(int)x, (int)y}}, value, z);

    if (D > 0) {
      x += xInc;
      D += 2 * deltaDelta;
    } else {
      D += 2 * delta.x();
    }
  }
}

auto rastrum::FrameBuffer::edge(Vector3DF a, Vector3DF b, Vector3DF p) -> float {
  // See https://dl.acm.org/doi/10.1145/54852.378457
  const auto val = ((p.x() - a.x()) * (b.y() - a.y()) - (p.y() - a.y()) * (b.x() - a.x()));
  return val;
}