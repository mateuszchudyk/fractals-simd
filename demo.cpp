#include "fractals-simd.h"

#include <iostream>

fractals::Viewport createViewportBasedOnResolution(int width, int height, float center_x, float center_y, float range_x)
{
    const auto ratio = float(width) / height;

    return fractals::Viewport(
        center_x - range_x,
        center_y + range_x / ratio,
        center_x + range_x,
        center_y - range_x / ratio
    );
}

int main(int argc, char** argv)
{
    // Mandelbrot
    {
        const auto viewport = createViewportBasedOnResolution(
            800, 600,
            -0.667, 0,
            1.8
        );
        auto result = fractals::mandelbrot(viewport, viewport.width() / 800, 50);
        fractals::utils::save_buffer_as_bmp(result, "mandelbrot.bmp", fractals::utils::colormap::hot);
    }

    // Burning ship
    {
        const auto viewport = createViewportBasedOnResolution(
            800, 600,
            -0.4, -0.4,
            2.5
        );
        auto result = fractals::burning_ship(viewport, viewport.width() / 800, 50);
        fractals::utils::save_buffer_as_bmp(result, "burning-ship.bmp", fractals::utils::colormap::hot, fractals::utils::transforms::vertical_flip);
    }

    // Julia set
    {
        const auto viewport = createViewportBasedOnResolution(
            800, 600,
            0, 0,
            1.8
        );
        auto result = fractals::julia_set({-0.1, 0.65}, viewport, viewport.width() / 800, 50);
        fractals::utils::save_buffer_as_bmp(result, "julia-set.bmp", fractals::utils::colormap::hot);
    }

    return 0;
}
