#include "worldgenerator.h"

#include "tiles/forest.h"
#include "tiles/grassland.h"

#include <vector>


namespace Course {

WorldGenerator& WorldGenerator::getInstance()
{
    static WorldGenerator instance;
    return instance;
}

void WorldGenerator::generateMap(
        unsigned int size_x,
        unsigned int size_y,
        unsigned int seed,
        const std::shared_ptr<iObjectManager>& objectmanager,
        const std::shared_ptr<iGameEventHandler>& eventhandler)
{
    unsigned int total_weight = 0;
    for (const auto& ctor : m_ctors)
    {
        total_weight += ctor.first;
    }

    srand(seed);
    std::vector<std::shared_ptr<TileBase>> tiles;
    for (unsigned int x = 0; x < size_x; ++x)
    {
        for (unsigned int y = 0; y < size_y; ++y)
        {
            auto ctor = findRandCtor(rand() % total_weight);
            tiles.push_back(ctor(Coordinate(x, y), eventhandler, objectmanager));
        }
    }

    objectmanager->addTiles(tiles);
}

TileConstructorPointer WorldGenerator::findRandCtor(int random) const
{
    while (random >= 0)
    {
        for (const auto& ctor : m_ctors)
        {
            random -= ctor.first;
            if (random < 0)
            {
                return ctor.second;
            }
        }
    }
    return {};
}

} // namespace Course
