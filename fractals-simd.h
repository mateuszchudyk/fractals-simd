#ifndef FRACTALS_H
#define FRACTALS_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <complex>

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

template <typename Type = unsigned char>
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

}

#endif
