#include "fractals-simd.h"

#include <iostream>

int main(int argc, char** argv)
{
    const fractals::Viewport viewport(-2.5, 1.5, 0.5, -1.5);
    const int32_t iteration_max = 50;

    auto result = fractals::mandelbrot(viewport, 0.003, iteration_max);
    fractals::utils::save_buffer_as_bmp(result, "image.bmp");

    return 0;
}
