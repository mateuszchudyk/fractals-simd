#include "fractals-simd.h"

#include <immintrin.h>
#include <cmath>

#define ABI_AVX2 __attribute__((target ("avx2")))

using namespace fractals;

using vf = __m256;
using vi = __m256i;

struct vcomplex
{
    vf real;
    vf imag;
};

template <typename Init, typename Kernel>
static ABI_AVX2 void loop(Buffer& result,
                          const Viewport& viewport,
                          uint32_t iterations,
                          Init init,
                          Kernel kernel)
{
    const vf epsilon = _mm256_set1_ps(4.0f);
    const auto dx = float(viewport.width()) / result.width();
    const auto dy = float(viewport.height()) / result.height();

    vcomplex input;
    vcomplex prev;
    for (uint32_t y = 0; y < result.height(); ++y)
    {
        auto* result_ptr = result.line<vi>(y);

        input.imag = _mm256_set1_ps(viewport.top() - y * dy);
        for (uint32_t x = 0; x < result.width(); x += sizeof(vi) / sizeof(uint32_t))
        {
            input.real = _mm256_set_ps(
                viewport.left() + (x + 7) * dx,
                viewport.left() + (x + 6) * dx,
                viewport.left() + (x + 5) * dx,
                viewport.left() + (x + 4) * dx,
                viewport.left() + (x + 3) * dx,
                viewport.left() + (x + 2) * dx,
                viewport.left() + (x + 1) * dx,
                viewport.left() + (x + 0) * dx
            );

            auto result = _mm256_set1_epi32(iterations);
            auto result_mask = _mm256_set1_epi32(0);

            prev = init(input);
            for (uint32_t iteration = 0; iteration < iterations; ++iteration)
            {
                prev = kernel(prev, input);

                auto rrii = _mm256_add_ps(_mm256_mul_ps(prev.real, prev.real), _mm256_mul_ps(prev.imag, prev.imag));
                auto mask = _mm256_andnot_si256(result_mask, _mm256_castps_si256(_mm256_cmp_ps(rrii, epsilon, _CMP_GT_OS)));
                auto counter = _mm256_set1_epi32(iteration);

                result = _mm256_or_si256(_mm256_andnot_si256(mask, result), _mm256_and_si256(counter, mask));
                result_mask = _mm256_or_si256(result_mask, mask);
            }
            *result_ptr++ = result;
        }
    }
};

void fractals::mandelbrot(Buffer& result, const Viewport& viewport, uint32_t iterations)
{
    loop(result, viewport, iterations,
        [](const vcomplex& input) ABI_AVX2
        {
            return vcomplex{
                _mm256_set1_ps(0.0f),
                _mm256_set1_ps(0.0f)
            };
        },
        [](const vcomplex& prev, const vcomplex& input) ABI_AVX2
        {
            return vcomplex{
                _mm256_add_ps(_mm256_sub_ps(_mm256_mul_ps(prev.real, prev.real), _mm256_mul_ps(prev.imag, prev.imag)), input.real),
                _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(2), _mm256_mul_ps(prev.real, prev.imag)), input.imag)
            };
        }
    );
}

void fractals::burning_ship(Buffer& result, const Viewport& viewport, uint32_t iterations)
{
    loop(result, viewport, iterations,
        [](const vcomplex& input) ABI_AVX2
        {
            return vcomplex{
                _mm256_set1_ps(0.0f),
                _mm256_set1_ps(0.0f)
            };
        },
        [](const vcomplex& prev, const vcomplex& input) ABI_AVX2
        {
            vcomplex prev_abs{
                _mm256_and_ps(prev.real, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff))),
                _mm256_and_ps(prev.imag, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff)))
            };

            return vcomplex{
                _mm256_add_ps(_mm256_sub_ps(_mm256_mul_ps(prev_abs.real, prev_abs.real), _mm256_mul_ps(prev_abs.imag, prev_abs.imag)), input.real),
                _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(2), _mm256_mul_ps(prev_abs.real, prev_abs.imag)), input.imag)
            };
        }
    );
}

void fractals::julia_set(Buffer& result, const Viewport& viewport, uint32_t iterations, const std::complex<float>& c)
{
    loop(result, viewport, iterations,
        [](const vcomplex& input) ABI_AVX2
        {
            return input;
        },
        [c](const vcomplex& prev, const vcomplex&) ABI_AVX2
        {
            return vcomplex{
                _mm256_add_ps(_mm256_sub_ps(_mm256_mul_ps(prev.real, prev.real), _mm256_mul_ps(prev.imag, prev.imag)), _mm256_set1_ps(c.real())),
                _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(2), _mm256_mul_ps(prev.real, prev.imag)), _mm256_set1_ps(c.imag()))
            };
        }
    );
}
