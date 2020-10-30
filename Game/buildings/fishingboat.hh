#ifndef FISHINGBOAT_H
#define FISHINGBOAT_H

#include "buildings/buildingbase.h"
#include "tiles/tilebase.h"
#include "constants/constants.hh"
#include "constants/resourcemaps2.h"

namespace Game {

/**
 * @brief The FishingBoat class is Course BuildingBase based and implements
 * Fishing Boat / ship to be placed on Ocean tiles in the game
 */
class FishingBoat : public Course::BuildingBase
{
public:
    const Course::ResourceMap BUILD_COST;
    const Course:: ResourceMap PRODUCTION_EFFECT;

    /**
     * @brief Disabled parameterless constructor.
     */
    FishingBoat() = delete;

    /**
     * @brief Constructor for the class.
     *
     * @param eventhandler points to the student's GameEventHandler.
     * @param owner points to the owning player.
     * @param descriptions contains descriptions and flavor texts.
     * @param tile points to the tile upon which the building is constructed.
     * @param hold is the initial amount of hold-markers.
     *
     * @post Exception guarantee: No guarantee.
     * @exception OwnerConflict - if the building conflicts with tile's
     * ownership.
     */
    explicit FishingBoat(
            const std::shared_ptr<Course::iGameEventHandler>& eventhandler,
            const std::shared_ptr<Course::iObjectManager>& objectmanager,
            const std::shared_ptr<Course::PlayerBase>& owner,
            const int& tilespaces = 1,
            const Course::ResourceMap& buildcost = FISHING_BOAT_BUILD_COST,
            const Course::ResourceMap& production = FISHING_BOAT_PRODUCTION
            );

    /**
     * @brief Default destructor.
     */
    virtual ~FishingBoat() = default;

    virtual std::string getType() const override;

    /**
     * @brief Returns boolean based on wheter the building can or can't be
     * placed on a Tile-object.
     * @param target is a pointer to the target Tile.
     * @return
     * True - Base class' method return true and Tile has space for building.
     * False - If both conditions are not met.
     * @note Override to modify placementrules for derived classes.
     * @post Exception guarantee: Basic
     */
    virtual bool canBePlacedOnTile(
            const std::shared_ptr<Course::TileBase> &target) const override;


};

}

#endif // FISHINGBOAT_H
