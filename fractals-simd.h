#ifndef FRACTALS_H
#define FRACTALS_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <complex>
#include <fstream>
#include <array>

namespace fractals {

template <typename Type>
class Point
{
public:
    Point(Type x, Type y) : _x(x), _y(y) {}

    Type x() const { return _x; }
    Type y() const { return _y; }

private:
    Type _x;
    Type _y;
};

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

class Buffer
{
public:
    Buffer(std::size_t width, std::size_t height, std::size_t alignment) : _width(width), _height(height), _alignment(alignment)
    {
        _stride = (width + alignment - 1) / alignment * alignment;
        _memory = static_cast<uint32_t*>(std::aligned_alloc(_alignment * sizeof(uint32_t), _stride * sizeof(uint32_t) * _height));
    }

    ~Buffer()
    {
        std::free(_memory);
    }

    std::size_t width() const { return _width; }
    std::size_t stride() const { return _stride; }
    std::size_t height() const { return _height; }
    std::size_t alignment() const { return _alignment; }

    template <typename T = uint32_t> const T* ptr(std::size_t x, std::size_t y) const { return reinterpret_cast<const T*>(_memory + y * _stride + x); }
    template <typename T = uint32_t> T* ptr(std::size_t x, std::size_t y) { return reinterpret_cast<T*>(_memory + y * _stride + x); }
    template <typename T = uint32_t> const T* line(std::size_t n) const { return ptr<T>(0, n); }
    template <typename T = uint32_t> T* line(std::size_t n) { return ptr<T>(0, n); }
    template <typename T = uint32_t> const T* begin() const { return ptr<T>(0, 0); }
    template <typename T = uint32_t> T* begin() { return ptr<T>(0, 0); }
    template <typename T = uint32_t> const T* end() const { return ptr<T>(_width, _height - 1); }
    template <typename T = uint32_t> T* end() { return ptr<T>(_width, _height - 1); }

private:
    std::size_t _width;
    std::size_t _stride;
    std::size_t _height;
    std::size_t _alignment;
    uint32_t *_memory;
};

Buffer mandelbrot(const Viewport& viewport, float resolution, uint32_t iterations);
Buffer burning_ship(const Viewport& viewport, float resolution, uint32_t iterations);
Buffer julia_set(const std::complex<float>& c, const Viewport& viewport, float resolution, uint32_t iterations);

namespace utils {

using Colormap = uint32_t(*)(unsigned char value);

namespace colormap {

uint32_t gray(unsigned char value);
uint32_t red(unsigned char value);
uint32_t green(unsigned char value);
uint32_t blue(unsigned char value);
uint32_t hot(unsigned char value);

}

using Transform = Point<uint32_t>(*)(const Buffer& buffer, const Point<uint32_t>& xy);

namespace transforms {

Point<uint32_t> identity(const Buffer& buffer, const Point<uint32_t>& xy);
Point<uint32_t> horizontal_flip(const Buffer& buffer, const Point<uint32_t>& xy);
Point<uint32_t> vertical_flip(const Buffer& buffer, const Point<uint32_t>& xy);

}

void save_buffer_as_bmp(const Buffer& buffer, const std::string& filename, const Colormap& colormap = colormap::hot, const Transform& transform = transforms::identity);

}

}

#endif
