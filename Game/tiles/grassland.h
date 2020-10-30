#ifndef GRASSLAND_H
#define GRASSLAND_H

#include "tilebase.h"


namespace Course {

/**
 * @brief The Grassland class represents Grassland in the gameworld.
 *
 * Grassland has BasicProduction: \n
 * * Money = 2
 * * Food = 5
 * * Wood = 1
 * * Stone = 1
 * * Ore = 0
 *
 * Functionality follows mainly the parent class' functionality.
 *
 * Tile supports 3 buildings.
 */
class Grassland : public TileBase
{
public:
    static const unsigned int MAX_BUILDINGS;
    static const unsigned int MAX_WORKERS;
    static const ResourceMap BASE_PRODUCTION;
    /**
     * @brief Disabled parameterless constructor.
     */
    Grassland() = delete;

    /**
     * @brief Constructor for the class.
     *
     * @param location is the Coordinate where the Tile is located in the game.
     * @param eventhandler points to the student's GameEventHandler.
     */
    Grassland(const Coordinate& location,
              const std::shared_ptr<iGameEventHandler>& eventhandler,
              const std::shared_ptr<iObjectManager>& objectmanager,
              const unsigned int& max_build = 3,
              const unsigned int& max_work = 3,
              const ResourceMap& production = ConstResourceMaps::GRASSLAND_BP);

    /**
     * @brief Default destructor.
     */
    virtual ~Grassland() = default;

    /**
     * @copydoc GameObject::getType()
     */
    virtual std::string getType() const override;

}; // class Grassland

} // namespace Course


#endif // GRASSLAND_H
