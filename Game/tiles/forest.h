#ifndef FOREST_H
#define FOREST_H

#include "tilebase.h"


namespace Course {

/**
 * @brief The Forest class represents Forest in the gameworld.
 *
 * Forest has BasicProduction: \n
 * * Money = 1
 * * Food = 3
 * * Wood = 5
 * * Stone = 1
 * * Ore = 0
 *
 * Building in the forest takes time. So buildings get extra hold-marker.
 *
 * Tile supports 2 buildings.
 */
class Forest : public TileBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Forest() = delete;

    /**
     * @brief Constructor for the class.
     *
     * @param location is the Coordinate where the Tile is located in the game.
     * @param eventhandler points to the student's GameEventHandler.
     */
    Forest(const Coordinate& location,
           const std::shared_ptr<iGameEventHandler>& eventhandler,
           const std::shared_ptr<iObjectManager>& objectmanager,
           const unsigned int& max_build = 2,
           const unsigned int& max_work = 3,
           const ResourceMap& production = ConstResourceMaps::FOREST_BP);

    /**
     * @brief Default destructor.
     */
    virtual ~Forest() = default;

    /**
     * @copydoc GameObject::getType()
     */
    virtual std::string getType() const override;

    /**
     * @brief Adds a new building-object to the tile. Building in forest adds
     * one hold-marker to the building.
     *
     * Phases: \n
     * 1. Check that there is space for the building. \n
     * 2. Call parent's addBuilding \n
     * 3. Add a HoldMarker for the building. \n
     *
     * @post Exception guarantee: Strong
     * @exception OwnerConflict - If the tile's ownership prevents placing the
     * \b building.
     * @exception NoSpace - If the tile doesn't have enough space for
     * the \b building.
     */
    void addBuilding(const std::shared_ptr<BuildingBase>& building) override;

}; // class Forest

} // namespace Course


#endif // FOREST_H
