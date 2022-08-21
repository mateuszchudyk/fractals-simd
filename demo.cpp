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
    auto resolution4k = fractals::Resolution(3840, 2160);
    // Mandelbrot
    {
        auto viewport = createViewport(resolution4k, {-0.667, 0}, 800);
        auto result = fractals::mandelbrot(resolution4k, viewport, 50);
        fractals::utils::save_buffer_as_bmp(result, "mandelbrot.bmp", fractals::utils::colormap::hot);
    }

    // Burning ship
    {
        auto viewport = createViewport(resolution4k, {-0.4, -0.4}, 700);
        auto result = fractals::burning_ship(resolution4k, viewport, 50);
        fractals::utils::save_buffer_as_bmp(result, "burning-ship.bmp", fractals::utils::colormap::hot, fractals::utils::transforms::vertical_flip);
    }

    // Julia set
    {
        auto viewport = createViewport(resolution4k, {0, 0}, 800);
        auto result = fractals::julia_set(resolution4k, viewport, 50, {-0.1, 0.65});
        fractals::utils::save_buffer_as_bmp(result, "julia-set.bmp", fractals::utils::colormap::hot);
    }

    // Colormaps
    // {
    //     auto resolution = fractals::Resolution(200, 150);
    //     auto viewport = createViewport(resolution, {-0.667, 0}, 55.f);
    //     auto result = fractals::mandelbrot(resolution, viewport, 50);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_gray.bmp", fractals::utils::colormap::gray);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_red.bmp", fractals::utils::colormap::red);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_green.bmp", fractals::utils::colormap::green);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_blue.bmp", fractals::utils::colormap::blue);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_yellow.bmp", fractals::utils::colormap::yellow);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_cyan.bmp", fractals::utils::colormap::cyan);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_magenta.bmp", fractals::utils::colormap::magenta);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_hot.bmp", fractals::utils::colormap::hot);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_jet.bmp", fractals::utils::colormap::jet);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_cividis.bmp", fractals::utils::colormap::cividis);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_twilight.bmp", fractals::utils::colormap::twilight);
    //     fractals::utils::save_buffer_as_bmp(result, "colormap_turbo.bmp", fractals::utils::colormap::turbo);
    // }

    return 0;
}
