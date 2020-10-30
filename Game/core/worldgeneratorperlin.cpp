#include "worldgeneratorperlin.hh"
#include <iostream>

namespace Game {

WorldGeneratorPerlin& WorldGeneratorPerlin::getInstance()
{
    static WorldGeneratorPerlin instance;
    return instance;
}

void WorldGeneratorPerlin::generateMap(
        unsigned int size_x,
        unsigned int size_y,
        unsigned int seed,
        const std::shared_ptr<ObjectManager>& objectmanager,
        const std::shared_ptr<GameEventHandler>& eventhandler)
{
    srand(seed);
    std::vector<std::shared_ptr<Course::TileBase>> tiles;

    // Get perlin noise
    PerlinNoise noise(size_x, size_y, seed);

    // Loop over map for noise and make tiles
    for (unsigned int x = 0; x < size_x; ++x)
    {
        for (unsigned int y = 0; y < size_y; ++y)
        {
            double weight = noise.getNoiseValue(x, y);
            std::cout << weight << std::endl;
            auto ctor = findTileByValue(weight);

            // Override default productions
            std::shared_ptr<Course::TileBase> tile = ctor(
                        Course::Coordinate(x, y), eventhandler, objectmanager);

            // Overriding resourcemaps
            if(tile->getType() == "Forest"){
                tile = std::make_shared<Course::Forest>(
                            Course::Coordinate(x, y), eventhandler, objectmanager, 2, 3, FOREST_BP);
            }
            if(tile->getType() == "Grassland"){
                tile = std::make_shared<Course::Grassland>(
                            Course::Coordinate(x, y), eventhandler, objectmanager, 2, 3, GRASSLAND_BP);
            }

            tiles.push_back(tile);
        }
    }

    objectmanager->addTiles(tiles);
}

TileConstructorPointer WorldGeneratorPerlin::findTileByValue(double value)
{
    //std::cout << value << std::endl;
    std::vector<std::tuple<float, float, TileConstructorPointer>> validTiles;

    for(const auto& ctor : tileConstructors_)
    {
        // Check that value is between min and max
        if(std::get<0>(ctor) <= value && std::get<1>(ctor) >= value)
        {
            validTiles.push_back(ctor);
        }
    }

    if(validTiles.size() != 0){
        int randIndex = ((double)rand() / RAND_MAX) * validTiles.size();
        return std::get<2>(validTiles.at(randIndex));
    }

    // If no tiles matched fall back to first
    return std::get<2>(tileConstructors_.at(0));
}
}

