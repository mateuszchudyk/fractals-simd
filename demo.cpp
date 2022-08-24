#include "fractals-simd.h"

#include <iostream>
#include <regex>

fractals::Viewport createViewport(const fractals::Resolution& resolution, const fractals::Point<float>& center, float scale)
{
    const auto ratio = float(resolution.width()) / resolution.height();

    return fractals::Viewport(
        center.x() - resolution.width() / 2 / scale,
        center.y() + resolution.height() / 2 / scale,
        center.x() + resolution.width() / 2 / scale,
        center.y() - resolution.height() / 2 / scale
    );
}

void usage()
{
    std::cout << "./demo fractal resolution [--center C] [--scale Z] [--iterations I] [--param P] [--output O]\n"
                 "\n"
                 "Parameters:\n"
                 "  fractal         - mandelbrot/burninng-ship/julia-set\n"
                 "  resolution      - in form WIDTHxHEIGHT e.g. 800x600\n"
                 "  --center        - complex number without spaces e.g. -0.4-0.4i (default = depends on the fractal)\n"
                 "  --scale         - how many pixels is equal to 1 (floating point) (default = depends on the fractal)\n"
                 "  --iterations    - number of iterations (default = 50)\n"
                 "  --param         - extra parameter used in julia set (default = depends on the fractal)\n"
                 "  --output        - output filename (default = \"output.bmp\")\n"
                 "  --colormap      - colormap, see README.md\n"
                 "\n"
                 "Example:\n"
                 "  ./demo mandelbrot 1920x1080 --center -0.667 --scale 400 --iterations 50\n"
              << std::endl;
}

fractals::Resolution parseResolution(const std::string& str)
{
    static auto pattern = std::regex("([0-9]+)x([0-9]+)");

    std::cmatch matches;
    if (std::regex_match(str.c_str(), matches, pattern) && matches.size() == 3)
    {
        return {uint32_t(std::stoi(matches[1].str())), uint32_t(std::stoi(matches[2].str()))};
    }

    return {0, 0};
}

fractals::Point<float> parseComplex(const std::string& str)
{
    static const auto pattern = std::regex("([+-]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][+-]?\\d+)?(?![iI.\\d]))?([+-]?(?:(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][+-]?\\d+)?)?[iI])?");

    std::cmatch matches;
    if (std::regex_match(str.c_str(), matches, pattern))
    {
        float x = 0;
        float y = 0;
        if (matches.size() >= 2 && matches[1].str() != "")
        {
            x = std::stof(matches[1].str());
        }
        if (matches.size() >= 3 && matches[2].str() != "")
        {
            y = std::stof(matches[2].str());
        }
        return {x, y};
    }

    return {0, 0};
}

fractals::utils::Colormap parseColormap(const std::string& str)
{
#define COLORMAP_CHOOSE(name) if (str == #name) { return fractals::utils::colormap::name; }
    COLORMAP_CHOOSE(gray)
    COLORMAP_CHOOSE(red)
    COLORMAP_CHOOSE(green)
    COLORMAP_CHOOSE(blue)
    COLORMAP_CHOOSE(yellow)
    COLORMAP_CHOOSE(cyan)
    COLORMAP_CHOOSE(magenta)
    COLORMAP_CHOOSE(hot)
    COLORMAP_CHOOSE(jet)
    COLORMAP_CHOOSE(cividis)
    COLORMAP_CHOOSE(twilight)
    COLORMAP_CHOOSE(turbo)
#undef COLORMAP_CHOOSE
    return nullptr;
}

fractals::Point<float> defaultCenter(const std::string& fractal)
{
#define CENTER_CHOOSE(name, x, y) if (fractal == name) { return {x, y}; }
    CENTER_CHOOSE("mandelbrot", -0.667, 0)
    CENTER_CHOOSE("burning-ship", -0.4, -0.4)
    CENTER_CHOOSE("julia-set", 0, 0)
#undef CENTER_CHOOSE
    return {0, 0};
}

float defaultScale(const std::string& fractal, const fractals::Resolution& resolution)
{
    float factor = resolution.width() / 1920.0f;
#define SCALE_CHOOSE(name, scale) if (fractal == name) { return scale * factor; }
    SCALE_CHOOSE("mandelbrot", 400)
    SCALE_CHOOSE("burning-ship", 350)
    SCALE_CHOOSE("julia-set", 400)
#undef SCALE_CHOOSE
    return 0;
}

uint32_t defaultIterations(const std::string& fractal)
{
    return 50;
}

fractals::Point<float> defaultParam(const std::string& fractal)
{
#define PARAM_CHOOSE(name,  x, y) if (fractal == name) { return {x, y}; }
    PARAM_CHOOSE("julia-set", -0.1, 0.65)
#undef PARAM_CHOOSE
    return {0, 0};
}

int main(int argc, char** argv)
{
    if (argc < 3 || argc % 2 == 0)
    {
        usage();
        return 1;
    }

    std::string fractal = argv[1];
    if (fractal != "mandelbrot" && fractal != "burning-ship" && fractal != "julia-set")
    {
        usage();
        return 2;
    }

    fractals::Resolution resolution = parseResolution(argv[2]);
    fractals::Point<float> center = defaultCenter(fractal);
    float scale = defaultScale(fractal, resolution);
    uint32_t iterations = defaultIterations(fractal);
    fractals::Point<float> param = defaultParam(fractal);
    std::string output = "output.bmp";
    fractals::utils::Colormap colormap = fractals::utils::colormap::hot;

    for (int i = 3; i < argc; i += 2)
    {
        if (strcmp(argv[i], "--center") == 0 && i+1 < argc)
        {
            center = parseComplex(argv[i+1]);
        }
        else if (strcmp(argv[i], "--scale") == 0  && i+1 < argc)
        {
            scale = std::stof(argv[i+1]);
        }
        else if (strcmp(argv[i], "--iterations") == 0  && i+1 < argc)
        {
            iterations = std::stoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--param") == 0 && i+1 < argc)
        {
            param = parseComplex(argv[i+1]);
        }
        else if (strcmp(argv[i], "--output") == 0 && i+1 < argc)
        {
            output = argv[i+1];
        }
        else if (strcmp(argv[i], "--colormap") == 0 && i+1 < argc)
        {
            colormap = parseColormap(argv[i+1]);
        }
        else
        {
            usage();
            return i;
        }
    }

    auto viewport = createViewport(resolution, center, scale);
    if (fractal == "mandelbrot")
    {
        auto result = fractals::mandelbrot(resolution, viewport, iterations);
        fractals::utils::save_buffer_as_bmp(result, output, colormap);
    }
    else if (fractal == "burning-ship")
    {
        auto result = fractals::burning_ship(resolution, viewport, iterations);
        fractals::utils::save_buffer_as_bmp(result, output, colormap, fractals::utils::transforms::vertical_flip);
    }
    else if (fractal == "julia-set")
    {
        auto result = fractals::julia_set(resolution, viewport, iterations, {param.x(), param.y()});
        fractals::utils::save_buffer_as_bmp(result, output, colormap);
    }

    return 0;
}
