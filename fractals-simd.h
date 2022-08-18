#ifndef FRACTALS_H
#define FRACTALS_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <complex>
#include <fstream>
#include <array>

namespace fractals {

class Viewport
{
public:
    Viewport(float left, float top, float right, float bottom) : _left(left), _top(top), _right(right), _bottom(bottom) {}

    float left() const { return _left; }
    float top() const { return _top; }
    float right() const { return _right; }
    float bottom() const { return _bottom; }

    float width() const { return _right - _left; }
    float height() const { return _top - _bottom; }

private:
    float _left;
    float _top;
    float _right;
    float _bottom;
};

template <typename Type = uint8_t>
class Buffer
{
public:
    Buffer(std::size_t width, std::size_t height, std::size_t alignment) : _width(width), _height(height), _alignment(alignment)
    {
        _stride = (width + alignment - 1) / alignment * alignment;
        _memory = static_cast<Type*>(std::aligned_alloc(_alignment * sizeof(Type), _stride * sizeof(Type) * _height));
    }

    ~Buffer()
    {
        std::free(_memory);
    }

    std::size_t width() const { return _width; }
    std::size_t stride() const { return _stride; }
    std::size_t height() const { return _height; }
    std::size_t alignment() const { return _alignment; }

    template <typename T = Type> const T* ptr(std::size_t x, std::size_t y) const { return reinterpret_cast<const T*>(_memory + y * _stride + x); }
    template <typename T = Type> T* ptr(std::size_t x, std::size_t y) { return reinterpret_cast<T*>(_memory + y * _stride + x); }
    template <typename T = Type> const T* line(std::size_t n) const { return ptr<T>(0, n); }
    template <typename T = Type> T* line(std::size_t n) { return ptr<T>(0, n); }
    template <typename T = Type> const T* begin() const { return ptr<T>(0, 0); }
    template <typename T = Type> T* begin() { return ptr<T>(0, 0); }
    template <typename T = Type> const T* end() const { return ptr<T>(_width, _height - 1); }
    template <typename T = Type> T* end() { return ptr<T>(_width, _height - 1); }

private:
    std::size_t _width;
    std::size_t _stride;
    std::size_t _height;
    std::size_t _alignment;
    Type *_memory;
};

Buffer<int32_t> mandelbrot(const Viewport& viewport, float resolution, int32_t iterations);
Buffer<int32_t> burning_ship(const Viewport& viewport, float resolution, int32_t iterations);
Buffer<int32_t> julia_set(const std::complex<float>& c, const Viewport& viewport, float resolution, int32_t iterations);

namespace utils {

namespace colormap {

using Colormap = int32_t(*)(unsigned char value);

int32_t gray(unsigned char value);
int32_t red(unsigned char value);
int32_t green(unsigned char value);
int32_t blue(unsigned char value);
int32_t hot(unsigned char value);

}

template <typename Type>
void save_buffer_as_bmp(const Buffer<Type>& buffer, const std::string& filename, const colormap::Colormap& colormap = colormap::hot)
{
    std::ofstream file(filename, std::ios::binary);

    int filesize = (14 + 40) + 4 * buffer.width() * buffer.height();

    uint8_t header[14] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *reinterpret_cast<int32_t*>(&header[2]) = filesize;
    *reinterpret_cast<int32_t*>(&header[6]) = 0;
    *reinterpret_cast<int32_t*>(&header[10]) = 54;

    uint8_t info_header[40] = {0};
    *reinterpret_cast<int32_t*>(&info_header[0]) = sizeof(info_header);
    *reinterpret_cast<int32_t*>(&info_header[4]) = buffer.width();
    *reinterpret_cast<int32_t*>(&info_header[8]) = buffer.height();
    *reinterpret_cast<int16_t*>(&info_header[12]) = 1;
    *reinterpret_cast<int16_t*>(&info_header[14]) = 32;

    file.write(reinterpret_cast<char*>(header), sizeof(header));
    file.write(reinterpret_cast<char*>(info_header), sizeof(info_header));

    // Save data
    Type max_value = 0;
    for (int y = 0; y < buffer.height(); ++y)
    {
        for (int x = 0; x < buffer.width(); ++x)
        {
            max_value = std::max(max_value, *buffer.ptr(x, y));
        }
    }

    auto zero = char(0);
    for (int y = buffer.height() - 1; y >= 0; --y)
    {
        for (int x = 0; x < buffer.width(); ++x)
        {
            int32_t value = (max_value > 0 ? colormap(std::round(255.0f * *buffer.ptr(x, y) / max_value)) : 0);
            file.write(reinterpret_cast<char*>(&value), 4);
        }
    }

    file.close();
}

}

}

#endif
