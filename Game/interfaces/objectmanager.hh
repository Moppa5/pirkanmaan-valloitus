#ifndef OBJECTMANAGER_HH
#define OBJECTMANAGER_HH

#include "interfaces/iobjectmanager.h"
#include "core/gameobject.h"
#include "graphics/mapitem.hh"
#include "tiles/tilebase.h"

#include "exceptions/keyerror.h"

#include <vector>
#include <memory>

namespace Game {

/**
 * @brief The ObjectManager class is Course side based implementation for
 * saving the gameobjects. It has the game tiles, buildings and workers.
 */
class ObjectManager : public Course::iObjectManager
{
public:
    /**
     * @brief Empty constructor for test cases
     * Had to be created since it was unsuccessful to create GameScene
     * to the UnitTests
     */
    ObjectManager();

    ~ObjectManager();

    /**
     * @brief Get all tiles on the given coordinates
     * @param coordinates - Coordinates of the tiles
     * @post Exception guarantee: No-throw
     * @return Vector of matching tiles
     */
    std::vector<std::shared_ptr<Course::TileBase>>
        getTiles(const std::vector<Course::Coordinate> &coordinates);

    /**
     * @brief Get all tiles
     * @post Exception guarantee: No-throw
     * @return Vector of all tiles
     */
    std::vector<std::shared_ptr<Course::TileBase>> getTiles();

    /**
     * @brief Get tile with given ID
     * @param id - ID of the tile
     * @post Exception guarantee: No-throw
     * @return Matching tile if found
     */
    std::shared_ptr<Course::TileBase> getTile(const Course::ObjectId &id);
    /**
     * @brief Get tile on given coordinate
     * @param coordinate - Coordinate of the tile
     * @post Exception guarantee: No-throw
     * @return Matching tile if found
     */
    std::shared_ptr<Course::TileBase> getTile(
            const Course::Coordinate &coordinate);

    /**
     * @brief Add tiles
     * @param tiles - Vector of tiles
     * @pre Valid tiles
     * @post Exception guarantee: No-throw
     */
    void addTiles(const std::vector<std::shared_ptr<Course::TileBase> > &tiles);

    /**
     * @brief Add building
     * @param building - Building object
     * @pre Valid building
     * @post Exception guarantee: No-throw
     */
    void addBuilding(const std::shared_ptr<Course::BuildingBase> &building);

    /**
     * @brief Remove building
     * @param building - Building object
     * @post Exception guarantee: Strong
     * @exceptions KeyError - Building not found
     */
    void removeBuilding(const std::shared_ptr<Course::BuildingBase> &building);

    /**
     * @brief Add worker
     * @param worker - Worker object
     * @pre Valid worker
     * @post Exception guarantee: No-throw
     */
    void addWorker(const std::shared_ptr<Course::WorkerBase> &worker);

    /**
     * @brief Remove worker
     * @param worker - Worker object
     * @post Exception guarantee: Strong
     * @exceptions KeyError - Worker not found
     */
    void removeWorker(const std::shared_ptr<Course::WorkerBase> &worker);

private:
    std::vector<std::shared_ptr<Course::TileBase>> tiles_;
    std::vector<std::shared_ptr<Course::BuildingBase>> buildings_;
    std::vector<std::shared_ptr<Course::WorkerBase>> workers_;
};
}
#endif // OBJECTMANAGER_HH
