#ifndef WORLDGENERATORPERLIN_HH
#define WORLDGENERATORPERLIN_HH

#include "tiles/tilebase.h"
#include "tiles/forest.h"
#include "tiles/grassland.h"
#include "interfaces/gameeventhandler.hh"
#include "interfaces/objectmanager.hh"
#include "core/perlinnoise.hh"

#include <vector>
#include <tuple>

namespace Game {

using TileConstructorPointer = std::function<std::shared_ptr<Course::TileBase>(
    Course::Coordinate,
    std::shared_ptr<GameEventHandler>,
    std::shared_ptr<ObjectManager>)>;

/**
 * @brief The WorldGeneratorPerlin class generates the game world and tiles
 * by using the PerlinNoise class that provides so called perlin noise
 * algorithm
 */
class WorldGeneratorPerlin
{
public:
    /**
     * @brief Used to get a reference to the Singleton instance.
     * @return Reference to the Singleton instance.
     * @post Exception guarantee: No-throw
     */
    static WorldGeneratorPerlin& getInstance();

    // Prevent copy and move construction and assignment.
    WorldGeneratorPerlin(const WorldGeneratorPerlin&) = delete;
    WorldGeneratorPerlin& operator=(const WorldGeneratorPerlin&) = delete;
    WorldGeneratorPerlin(WorldGeneratorPerlin&&) = delete;
    WorldGeneratorPerlin& operator=(WorldGeneratorPerlin&&) = delete;

    /**
     * @brief Register a Tile's constructor for use in map generation.
     * @note Do this only once per Tile type or they won't be equally common.
     * Use the Tile's type as the template parameter: addConstructor<Forest>();
     * Use values 0...1
     * @param weight represents the rarity of the Tile, high being common.
     */
    template<typename T>
    void addConstructor(float min, float max)
    {
        TileConstructorPointer ctor = std::make_shared<T, Course::Coordinate,
                std::shared_ptr<GameEventHandler>,
                std::shared_ptr<ObjectManager>>;
        tileConstructors_.push_back(std::make_tuple(min, max, ctor));
    }

    /**
     * @brief Generates Tile-objects and sends them to ObjectManager.
     * @param size_x is the horizontal size of the map area.
     * @param size_y is the vertical size of the map area.
     * @param seed is the seed-value used in the generation.
     * @param objectmanager points to the ObjectManager that receives the
     * generated Tiles.
     * @param eventhandler points to the student's GameEventHandler.
     * @post Exception guarantee: No-throw
     */
    void generateMap(unsigned int size_x,
                     unsigned int size_y,
                     unsigned int seed,
                     const std::shared_ptr<ObjectManager>& objectmanager,
                     const std::shared_ptr<GameEventHandler>& eventhandler);

private:
    /**
     * @brief Default constructor.
     */
    WorldGeneratorPerlin() = default;

    /**
     * @brief Default destructor.
     */
    ~WorldGeneratorPerlin() = default;

    /**
     * @brief Find the Tile ctor matching the value.
     * @param value is the number being matched to a Tile.
     * @return The constructor matching the value.
     */
    TileConstructorPointer findTileByValue(double value);

    /**
     * @brief Find random Tile ctor in the range min...max
     * @param min - Minimum value of the matching Tile
     * @param max - Maximum value of the matching Tile
     * @return The constructor matching the range
     */
    TileConstructorPointer findTileInValueRange(int min, int max) const;

    std::vector<std::tuple<float, float, TileConstructorPointer>> tileConstructors_;
};

}
#endif // WORLDGENERATORPERLIN_HH
