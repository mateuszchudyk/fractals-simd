#include "fractals-simd.h"

#include <iostream>

int main(int argc, char** argv)
{
    const fractals::utils::Viewport viewport(-2.5, 1.5, 0.5, -1.5);
    const int32_t iteration_max = 50;

    auto result = fractals::mandelbrot(viewport, 0.003, iteration_max);

    std::cout << "[";
    for (std::size_t y = 0; y < result.height(); ++y)
    {
        std::cout << (y > 0 ? ",\n" : "") << "[";
        for (std::size_t x = 0; x < result.width(); ++x)
            std::cout << (x > 0 ? "," : "") << *result.ptr(x, y);
        std::cout << "]";
    }
    std::cout << "]" << std::endl;

    return 0;
}
