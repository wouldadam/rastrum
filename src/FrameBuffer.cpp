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
  _data[idx] = value;
}

void rastrum::FrameBuffer::set(size_t x, size_t y, RGBA value) {
  const auto idx = (y * _width) + x;
  _data[idx] = value;
}

void rastrum::FrameBuffer::write_bmp(const std::string& filename) const {
  const auto result = stbi_write_bmp(filename.c_str(), _width, _height, 4, _data.data());
  if (result == 0) {
    std::cerr << "Failed to write image: " << result << "\n";
  }
}