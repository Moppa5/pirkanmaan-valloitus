#ifndef PERLINNOISE_HH
#define PERLINNOISE_HH

#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

namespace Game {

/**
 * @brief The PerlinNoise class produces so called perlin noise for
 * WorldGenerator. Generated Game tiles look more realistic and not just
 * randomized.
 */
class PerlinNoise
{
public:
    /**
     * @brief Constructor for class
     * @param width - Map width
     * @param height - Map height
     * @param seed - Seed used for random generation
     * @post Exception guarantee: No-throw
     */
    PerlinNoise(unsigned int width, unsigned int height, unsigned int seed);

    /**
     * @brief Get noise value on certain integer point
     * @param x - X coordinate
     * @param y - Y coordinate
     * @post Exception guarantee: No-throw
     * @return Noise value with double float precision
     */
    double getNoiseValue(int x, int y);

private:
    /**
     * @brief Generates the noise map
     * @post Exception guarantee: No-throw
     */
    void generateNoise();

    /**
     * @brief Normalises the noise to range 0...1
     * @post Exception guarantee: No-throw
     */
    void normaliseNoise();

    /**
     * @brief Noise smoothing with square mask
     * @param range - half the square diameter minus one
     * @post Exception guarantee: No-throw
     * @note All ranges produce a square with odd diameter
     */
    void smoothSquare(int range);

    unsigned int outputWidth_;
    unsigned int outputHeight_;
    std::vector<float> noiseSeed_;
    std::vector<float> perlinNoise_;

    unsigned int octaves_ = 5;
    float bias_ = 1;
};
}

#endif // PERLINNOISE_HH
