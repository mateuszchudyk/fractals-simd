#include "fractals-simd.h"

#include <immintrin.h>
#include <cmath>

#define ABI_AVX2 __attribute__((target ("avx2")))

using namespace fractals;

using vf = __m256;
using vi = __m256i;

template <typename Kernel>
static ABI_AVX2 utils::Buffer<int32_t> loop(const utils::Viewport& viewport,
                                            float resolution,
                                            int32_t iterations,
                                            Kernel kernel)
{
    const vf epsilon = _mm256_set1_ps(4.0f);

    auto result_buffer = utils::Buffer<int32_t>(
        std::round(viewport.width() / resolution + 1),
        std::round(viewport.height() / resolution + 1),
        sizeof(vi) / sizeof(int32_t));

    for (std::size_t y = 0; y < result_buffer.height(); ++y)
    {
        auto imag_in = _mm256_set1_ps(viewport.top() - y * resolution);

        auto* result_ptr = result_buffer.line<vi>(y);
        for (std::size_t x = 0; x < result_buffer.width(); x += sizeof(vi) / sizeof(int32_t))
        {
            auto result = _mm256_set1_epi32(iterations);
            auto result_mask = _mm256_set1_epi32(0);

            auto real_in = _mm256_set_ps(
                viewport.left() + (x + 7) * resolution,
                viewport.left() + (x + 6) * resolution,
                viewport.left() + (x + 5) * resolution,
                viewport.left() + (x + 4) * resolution,
                viewport.left() + (x + 3) * resolution,
                viewport.left() + (x + 2) * resolution,
                viewport.left() + (x + 1) * resolution,
                viewport.left() + (x + 0) * resolution
            );
            auto real_prev = _mm256_set1_ps(0.0f);
            auto imag_prev = _mm256_set1_ps(0.0f);

            for (int32_t iteration = 0; iteration < iterations; ++iteration)
            {
                auto current = kernel(real_prev, imag_prev, real_in, imag_in);
                real_prev = current.first;
                imag_prev = current.second;
                auto rrii = _mm256_add_ps(_mm256_mul_ps(real_prev, real_prev), _mm256_mul_ps(imag_prev, imag_prev));
                auto mask = _mm256_andnot_si256(result_mask, _mm256_castps_si256(_mm256_cmp_ps(rrii, epsilon, _CMP_GT_OS)));
                auto counter = _mm256_set1_epi32(iteration);

                result = _mm256_or_si256(_mm256_andnot_si256(mask, result), _mm256_and_si256(counter, mask));
                result_mask = _mm256_or_si256(result_mask, mask);
            }
            *result_ptr++ = result;
        }
    }

    return result_buffer;
};

utils::Buffer<int32_t> fractals::mandelbrot(const utils::Viewport& viewport, float resolution, int32_t iterations)
{
    return loop(viewport, resolution, iterations,
        [](vf real_prev, vf imag_prev, vf real_in, vf imag_in) ABI_AVX2
        {
            return std::make_pair(
                _mm256_add_ps(_mm256_sub_ps(_mm256_mul_ps(real_prev, real_prev), _mm256_mul_ps(imag_prev, imag_prev)), real_in),
                _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(2), _mm256_mul_ps(real_prev, imag_prev)), imag_in)
            );
        }
    );
}

utils::Buffer<int32_t> fractals::burning_ship(const utils::Viewport& viewport, float resolution, int32_t iterations)
{
    return loop(viewport, resolution, iterations,
        [](vf real_prev, vf imag_prev, vf real_in, vf imag_in) ABI_AVX2
        {
            auto real_prev_abs = _mm256_and_ps(real_prev, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff)));
            auto imag_prev_abs =_mm256_and_ps(imag_prev, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff)));

            return std::make_pair(
                _mm256_add_ps(_mm256_sub_ps(_mm256_mul_ps(real_prev_abs, real_prev_abs), _mm256_mul_ps(imag_prev_abs, imag_prev_abs)), real_in),
                _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(2), _mm256_mul_ps(real_prev_abs, imag_prev_abs)), imag_in)
            );
        }
    );
}
