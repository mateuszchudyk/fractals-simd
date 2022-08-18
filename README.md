# Fractals SIMD

## Overview

Library containing following set of fractals implemented using SIMD:

| Fractal      | SSE3 | AVX2 |
|--------------|------|------|
| Mandelbrot   | X    | X    |
| Burning ship | X    | X    |
| Julia set    | X    | X    |

## Renders

**Mandelbrot** (iteration limit = 50)

![](images/mandelbrot.png)

**Burning ship** (iteration limit = 50)

![](images/burning-ship.png)

**Julia set, c = -0.1 + 0.65i** (iteration limit = 50)

![](images/julia-set.png)

**Julia set, c = -0.1 + 0.65i** (iteration limit = 10000)

![](images/julia-set-10000.png)
