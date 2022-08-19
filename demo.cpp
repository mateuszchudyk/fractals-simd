#include "fractals-simd.h"

#include <iostream>

fractals::Viewport createViewport(const fractals::Buffer& buffer, const fractals::Point<float>& center, float zoom)
{
    const auto ratio = float(buffer.width()) / buffer.height();

    return fractals::Viewport(
        center.x() - buffer.width() / 2 / zoom,
        center.y() + buffer.height() / 2 / zoom,
        center.x() + buffer.width() / 2 / zoom,
        center.y() - buffer.height() / 2 / zoom
    );
}

int main(int argc, char** argv)
{
    // Mandelbrot
    {
        auto result = fractals::Buffer(800, 600);
        auto viewport = createViewport(result, {-0.667, 0}, 222.f);
        fractals::mandelbrot(result, viewport, 50);
        fractals::utils::save_buffer_as_bmp(result, "mandelbrot.bmp", fractals::utils::colormap::hot);
    }

    // Burning ship
    {
        auto result = fractals::Buffer(800, 600);
        auto viewport = createViewport(result, {-0.4, -0.4}, 160);
        fractals::burning_ship(result, viewport, 50);
        fractals::utils::save_buffer_as_bmp(result, "burning-ship.bmp", fractals::utils::colormap::hot, fractals::utils::transforms::vertical_flip);
    }

    // Julia set
    {
        auto result = fractals::Buffer(800, 600);
        auto viewport = createViewport(result, {0, 0}, 222.f);
        fractals::julia_set(result, viewport, 50, {-0.1, 0.65});
        fractals::utils::save_buffer_as_bmp(result, "julia-set.bmp", fractals::utils::colormap::hot);
    }

    return 0;
}
