# Fractals SIMD

## Overview

Library containing following set of fractals implemented using SIMD:

|              | SSE3 | AVX2 |
|--------------|------|------|
| Mandelbrot   | +    | +    |
| Burning ship | +    | +    |
| Julia set    | +    | +    |

**Built-in colormaps:**

red, green, blue, yellow, cyan, magenta, gray, hot, jet, cividis, twilight, turbo

![](images/colormaps/colormap_red.png)
![](images/colormaps/colormap_green.png)
![](images/colormaps/colormap_blue.png)

![](images/colormaps/colormap_yellow.png)
![](images/colormaps/colormap_cyan.png)
![](images/colormaps/colormap_magenta.png)

![](images/colormaps/colormap_gray.png)
![](images/colormaps/colormap_hot.png)
![](images/colormaps/colormap_jet.png)

![](images/colormaps/colormap_cividis.png)
![](images/colormaps/colormap_twilight.png)
![](images/colormaps/colormap_turbo.png)

## Renders

**Mandelbrot** (50 iterations)

![](images/mandelbrot.png)

**Burning ship** (50 iterations)

![](images/burning-ship.png)

**Julia set, c = -0.1 + 0.65i** (50 iterations)

![](images/julia-set.png)

**Julia set, c = -0.1 + 0.65i** (10 000 iterations)

![](images/julia-set-10000.png)
