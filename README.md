# Fractals SIMD

## Overview

Library containing following set of fractals implemented using SIMD:

|              | SSE3 | AVX2 |
|--------------|------|------|
| Mandelbrot   | +    | +    |
| Burning ship | +    | +    |
| Julia set    | +    | +    |

**Supported colormaps:**

Red, green, blue, gray, hot, jet:

![](images/colormaps/red.png)
![](images/colormaps/green.png)
![](images/colormaps/blue.png)

![](images/colormaps/gray.png)
![](images/colormaps/hot.png)
![](images/colormaps/jet.png)

## Renders

**Mandelbrot** (iteration limit = 50)

![](images/mandelbrot.png)

**Burning ship** (iteration limit = 50)

![](images/burning-ship.png)

**Julia set, c = -0.1 + 0.65i** (iteration limit = 50)

![](images/julia-set.png)

**Julia set, c = -0.1 + 0.65i** (iteration limit = 10000)

![](images/julia-set-10000.png)

