# Fractals SIMD

## Overview

Library containing following set of fractals implemented using SIMD:

|              | SSE3 | AVX2 |
|--------------|------|------|
| Mandelbrot   | +    | +    |
| Burning ship | +    | +    |
| Julia set    | +    | +    |

**Built-in colormaps:**

red, green, blue, yellow, cyan, magenta, gray, hot, jet

![](images/colormaps/red.png)
![](images/colormaps/green.png)
![](images/colormaps/blue.png)

![](images/colormaps/yellow.png)
![](images/colormaps/cyan.png)
![](images/colormaps/magenta.png)

![](images/colormaps/gray.png)
![](images/colormaps/hot.png)
![](images/colormaps/jet.png)

## Renders

**Mandelbrot** (50 iterations)

![](images/mandelbrot.png)

**Burning ship** (50 iterations)

![](images/burning-ship.png)

**Julia set, c = -0.1 + 0.65i** (50 iterations)

![](images/julia-set.png)

**Julia set, c = -0.1 + 0.65i** (10000 iterations)

![](images/julia-set-10000.png)

