#include "perlinnoise.hh"
#include <iostream>

namespace Game {


PerlinNoise::PerlinNoise(unsigned int width, unsigned int height, unsigned int seed)
{
    outputWidth_ = width;
    outputHeight_ = height;

    srand(seed);

    // Fill noiseSeed_ with random 0..1 and perlinNoise_ with zeros
    for(unsigned int i=0; i<outputWidth_*outputHeight_; i++){
        noiseSeed_.push_back((float)rand() / (float)RAND_MAX);
        perlinNoise_.push_back(0);
    }

    generateNoise();
    //smoothSquare(5);
    smoothSquare(4);
    smoothSquare(3);
    smoothSquare(2);
    smoothSquare(1);
    normaliseNoise();
}

double PerlinNoise::getNoiseValue(int x, int y)
{
    return perlinNoise_.at(y*outputWidth_+x);
}

void PerlinNoise::generateNoise()
{
    for(unsigned int x=0; x<outputWidth_; x++){
        for(unsigned int y=0; y<outputHeight_; y++)
        {
            float noise = 0.0f;
            float scale = 1.0f;
            float scaleAcc = 0.0f;

            for(unsigned int o=0; o<octaves_; o++)
            {
                int pitch = outputWidth_ >> o;
                int sampleX1 = (x/pitch)*pitch;
                int sampleY1 = (y/pitch)*pitch;

                int sampleX2 = (sampleX1+pitch) % outputWidth_;
                int sampleY2 = (sampleY1+pitch) % outputHeight_;

                float blendX = (float)(x-sampleX1) / (float)pitch;
                float blendY = (float)(y-sampleY1) / (float)pitch;

                float sampleT = (1.0f - blendX) * noiseSeed_.at(sampleY1*outputWidth_+sampleX1) +
                        blendX * noiseSeed_.at(sampleY1*outputWidth_+ sampleX2);
                float sampleB = (1.0f - blendX) * noiseSeed_.at(sampleY2*outputWidth_+sampleX1) +
                        blendX * noiseSeed_.at(sampleY2*outputWidth_+ sampleX2);
                noise += (blendY * (sampleB-sampleT) + sampleT) * scale;
                scaleAcc += scale;
                scale = scale / bias_;
            }

            perlinNoise_.at(y * outputWidth_ + x) = noise /scaleAcc;
        }
    }
}

void PerlinNoise::normaliseNoise()
{
    // Find min and max values generated
    double noiseMin = 1.0f;
    double noiseMax = 0.0f;
    for (unsigned int x = 0; x < outputWidth_; ++x)
    {
        for (unsigned int y = 0; y < outputHeight_; ++y)
        {
            double n = getNoiseValue(x, y);
            if(n < noiseMin){noiseMin = n;}
            if(n > noiseMax){noiseMax = n;}
        }
    }

	//std::cout << "Noise max: " + std::to_string(noiseMax) << std::endl;
	//std::cout << "Noise min: " + std::to_string(noiseMin) << std::endl;

    // Centralise noise based on min and max
    for(long unsigned int i=0; i<perlinNoise_.size(); i++){
        perlinNoise_.at(i) = (perlinNoise_.at(i)-noiseMin) / (noiseMax-noiseMin);
    }

    // Find new mix and max
    noiseMin = 1.0f;
    noiseMax = 0.0f;
    for (unsigned int x = 0; x < outputWidth_; ++x)
    {
        for (unsigned int y = 0; y < outputHeight_; ++y)
        {
            double n = getNoiseValue(x, y);
            if(n < noiseMin){noiseMin = n;}
            if(n > noiseMax){noiseMax = n;}
        }
    }

	//std::cout << "Noise max: " + std::to_string(noiseMax) << std::endl;
	//std::cout << "Noise min: " + std::to_string(noiseMin) << std::endl;
}

void PerlinNoise::smoothSquare(int range)
{
    std::vector<float> tempNoise;

    // Loop over all "tiles"
    for(unsigned int i=0; i<outputWidth_*outputHeight_; i++)
    {
        float sum = perlinNoise_.at(i);

        // Loop square of size range
        for(int x=-range; x<=range; x++)
        {
            for(int y=-range; y<=range; y++)
            {
                int index = i + x + y*outputWidth_;

                // If outside map
                if(index < 0 || index > static_cast<int>
                        (outputWidth_*outputHeight_-1)){
                    index = i;
                }
                sum += perlinNoise_.at(index);
            }
        }
        //std::cout << sum/((2*range+1)*(2*range+1)) << std::endl;
        tempNoise.push_back(sum/((2*range+1)*(2*range+1)));
    }

    perlinNoise_ = tempNoise;
}
}
