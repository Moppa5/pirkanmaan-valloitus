#ifndef IOBJECTMANAGER_H
#define IOBJECTMANAGER_H

#include <memory>
#include <vector>


namespace Course {

class TileBase;
class Coordinate;

#ifndef COURSE_OBJECTID
#define COURSE_OBJECTID
using ObjectId = unsigned int;
#endif

/**
 * @brief The iObjectManager class is an interface which the Course-side
 * code uses to interact with the ObjectManager implemented by the students.
 *
 * @note The interface declares only functions required by the Course-side code.
 * The actual implementation can (and should!) contain more stuff.
 */
class iObjectManager
{
public:
    /**
     * @brief Default destructor.
     */
    virtual ~iObjectManager() = default;

    /**
     * @brief Adds new tiles to the ObjectManager.
     * @param tiles a vector that contains the Tiles to be added.
     * @post The tile-pointers in the vector are stored in the ObjectManager.
     * Exception Guarantee: Basic
     *
     */
    virtual void addTiles(
            const std::vector<std::shared_ptr<TileBase>>& tiles) = 0;

    /**
     * @brief Returns a shared pointer to a Tile that has specified coordinate.
     * @param coordinate Requested Tile's Coordinate
     * @return a pointer to a Tile that has the given coordinate.
     * If no for the coordinate exists, return empty pointer.
     * @post Exception Guarantee: Basic
     */
    virtual std::shared_ptr<TileBase> getTile(
            const Coordinate& coordinate) = 0;

    /**
     * @brief Returns a shared pointer to a Tile that has specified ID
     * @param id Tile's ObjectId.
     * @return a pointer to the Tile that has the given ID
     * If no for the id exists, return empty pointer.
     * @post Exception Guarantee: Basic
     */
    virtual std::shared_ptr<TileBase> getTile(const ObjectId& id) = 0;

    /**
     * @brief Returns a vector of shared pointers to Tiles specified by
     * a vector of Coordinates.
     * @param coordinates a vector of Coordinates for the requested Tiles
     * @return Vector of that contains pointers to Tile's that match
     * the coordinates. The vector is empty if no matches were made.
     * @post Exception Guarantee: Basic
     */
    virtual std::vector<std::shared_ptr<TileBase>> getTiles(
            const std::vector<Coordinate>& coordinates) = 0;


}; // class iObjectManager

} // namespace Course


#endif // IOBJECTMANAGER_H
