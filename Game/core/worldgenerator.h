#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <functional>
#include <map>
#include <memory>

#include "interfaces/igameeventhandler.h"
#include "interfaces/iobjectmanager.h"
#include "tiles/tilebase.h"


namespace Course {

using TileConstructorPointer = std::function<std::shared_ptr<TileBase>(
    Coordinate,
    std::shared_ptr<iGameEventHandler>,
    std::shared_ptr<iObjectManager>)>;

/**
 * @brief The WorldGenerator class is a singleton for generating tiles
 * for the game.
 *
 * Students use this to create new Tile-objects that form the gameworld. \n
 * Usage:
 * 1. Use WorldGenerator::getInstance() to get a reference to the singleton.
 * 2. Call addConstructor with each Tile type you created.
 * 3. Generate the map through the reference.
 */
class WorldGenerator
{
public:
    /**
     * @brief Used to get a reference to the Singleton instance.
     * @return Reference to the Singleton instance.
     * @post Exception guarantee: No-throw
     */
    static WorldGenerator& getInstance();

    // Prevent copy and move construction and assignment.
    WorldGenerator(const WorldGenerator&) = delete;
    WorldGenerator& operator=(const WorldGenerator&) = delete;
    WorldGenerator(WorldGenerator&&) = delete;
    WorldGenerator& operator=(WorldGenerator&&) = delete;

    /**
     * @brief Register a Tile's constructor for use in map generation.
     * @note Do this only once per Tile type or they won't be equally common.
     * Use the Tile's type as the template parameter: addConstructor<Forest>();
     * @param weight represents the rarity of the Tile, high being common.
     */
    template<typename T>
    void addConstructor(unsigned int weight)
    {
        TileConstructorPointer ctor = std::make_shared<T, Coordinate,
                std::shared_ptr<iGameEventHandler>,
                std::shared_ptr<iObjectManager> >;
        m_ctors.insert(std::make_pair(weight, ctor));
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
                     const std::shared_ptr<iObjectManager>& objectmanager,
                     const std::shared_ptr<iGameEventHandler>& eventhandler);

private:
    /**
     * @brief Default constructor.
     */
    WorldGenerator() = default;

    /**
     * @brief Default destructor.
     */
    ~WorldGenerator() = default;

    /**
     * @brief Find the Tile ctor matching the random number.
     * @param random is the random number being matched to a Tile.
     * @return The constructor matching the random number.
     */
    TileConstructorPointer findRandCtor(int random) const;

    // For mapping constructors.
    std::multimap<unsigned int, TileConstructorPointer> m_ctors;

}; // class WorldGenerator

} // namespace Course


#endif // WORLDGENERATOR_H
