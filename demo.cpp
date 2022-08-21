#include "fractals-simd.h"

#include <iostream>

fractals::Viewport createViewport(const fractals::Resolution& resolution, const fractals::Point<float>& center, float zoom)
{
    const auto ratio = float(resolution.width()) / resolution.height();

    return fractals::Viewport(
        center.x() - resolution.width() / 2 / zoom,
        center.y() + resolution.height() / 2 / zoom,
        center.x() + resolution.width() / 2 / zoom,
        center.y() - resolution.height() / 2 / zoom
    );
}

int main(int argc, char** argv)
{
    // // Mandelbrot
    // {
    //     auto resolution = fractals::Resolution(800, 600);
    //     auto viewport = createViewport(resolution, {-0.667, 0}, 222.f);
    //     auto result = fractals::mandelbrot(resolution, viewport, 50);
    //     fractals::utils::save_buffer_as_bmp(result, "mandelbrot.bmp", fractals::utils::colormap::hot);
    // }

    // // Burning ship
    // {
    //     auto resolution = fractals::Resolution(800, 600);
    //     auto viewport = createViewport(resolution, {-0.4, -0.4}, 160);
    //     auto result = fractals::burning_ship(resolution, viewport, 50);
    //     fractals::utils::save_buffer_as_bmp(result, "burning-ship.bmp", fractals::utils::colormap::hot, fractals::utils::transforms::vertical_flip);
    // }

    // // Julia set
    // {
    //     auto resolution = fractals::Resolution(800, 600);
    //     auto viewport = createViewport(resolution, {0, 0}, 222.f);
    //     auto result = fractals::julia_set(resolution, viewport, 50, {-0.1, 0.65});
    //     fractals::utils::save_buffer_as_bmp(result, "julia-set.bmp", fractals::utils::colormap::hot);
    // }

    // Colormaps
    {
        auto resolution = fractals::Resolution(200, 150);
        auto viewport = createViewport(resolution, {-0.667, 0}, 55.f);
        auto result = fractals::mandelbrot(resolution, viewport, 50);
        fractals::utils::save_buffer_as_bmp(result, "gray.bmp", fractals::utils::colormap::gray);
        fractals::utils::save_buffer_as_bmp(result, "red.bmp", fractals::utils::colormap::red);
        fractals::utils::save_buffer_as_bmp(result, "green.bmp", fractals::utils::colormap::green);
        fractals::utils::save_buffer_as_bmp(result, "blue.bmp", fractals::utils::colormap::blue);
        fractals::utils::save_buffer_as_bmp(result, "yellow.bmp", fractals::utils::colormap::yellow);
        fractals::utils::save_buffer_as_bmp(result, "cyan.bmp", fractals::utils::colormap::cyan);
        fractals::utils::save_buffer_as_bmp(result, "magenta.bmp", fractals::utils::colormap::magenta);
        fractals::utils::save_buffer_as_bmp(result, "hot.bmp", fractals::utils::colormap::hot);
        fractals::utils::save_buffer_as_bmp(result, "jet.bmp", fractals::utils::colormap::jet);
        fractals::utils::save_buffer_as_bmp(result, "cividis.bmp", fractals::utils::colormap::cividis);
        fractals::utils::save_buffer_as_bmp(result, "twilight.bmp", fractals::utils::colormap::twilight);
        fractals::utils::save_buffer_as_bmp(result, "turbo.bmp", fractals::utils::colormap::turbo);
    }

    return 0;
}
