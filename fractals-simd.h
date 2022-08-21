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

class Resolution
{
public:
    Resolution(uint32_t width, uint32_t height) : _width(width), _height(height) {}

    uint32_t width() const { return _width; }
    uint32_t height() const { return _height; }

private:
    uint32_t _width;
    uint32_t _height;
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
    Buffer(uint32_t width, uint32_t height, uint32_t alignment) : _width(width), _height(height), _alignment(alignment)
    {
        _stride = (width + alignment - 1) / alignment * alignment;
        _memory = static_cast<uint32_t*>(std::aligned_alloc(_alignment * sizeof(uint32_t), _stride * sizeof(uint32_t) * _height));
    }

    ~Buffer()
    {
        std::free(_memory);
    }

    uint32_t width() const { return _width; }
    uint32_t stride() const { return _stride; }
    uint32_t height() const { return _height; }
    uint32_t alignment() const { return _alignment; }

    template <typename T = uint32_t> const T* ptr(uint32_t x, uint32_t y) const { return reinterpret_cast<const T*>(_memory + y * _stride + x); }
    template <typename T = uint32_t> T* ptr(uint32_t x, uint32_t y) { return reinterpret_cast<T*>(_memory + y * _stride + x); }
    template <typename T = uint32_t> const T* line(uint32_t n) const { return ptr<T>(0, n); }
    template <typename T = uint32_t> T* line(uint32_t n) { return ptr<T>(0, n); }
    template <typename T = uint32_t> const T* begin() const { return ptr<T>(0, 0); }
    template <typename T = uint32_t> T* begin() { return ptr<T>(0, 0); }
    template <typename T = uint32_t> const T* end() const { return ptr<T>(_width, _height - 1); }
    template <typename T = uint32_t> T* end() { return ptr<T>(_width, _height - 1); }

private:
    uint32_t _width;
    uint32_t _stride;
    uint32_t _height;
    uint32_t _alignment;
    uint32_t *_memory;
};

Buffer mandelbrot(const Resolution& resolution, const Viewport& viewport, uint32_t iterations);
Buffer burning_ship(const Resolution& resolution, const Viewport& viewport, uint32_t iterations);
Buffer julia_set(const Resolution& resolution, const Viewport& viewport, uint32_t iterations, const std::complex<float>& c);

namespace utils {

using Colormap = uint32_t(*)(unsigned char value);

namespace colormap {

uint32_t gray(unsigned char value);
uint32_t red(unsigned char value);
uint32_t green(unsigned char value);
uint32_t blue(unsigned char value);
uint32_t hot(unsigned char value);
uint32_t jet(unsigned char value);

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
