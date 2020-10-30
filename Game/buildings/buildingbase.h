#ifndef BUILDINGBASE_H
#define BUILDINGBASE_H

#include "core/placeablegameobject.h"
#include "core/basicresources.h"


namespace Course {

class TileBase;

/**
 * @brief The BuildingBase class is a base-class for
 * different buildings in the game.
 *
 * * Can increase base-production for a Tile.
 * * Can call functions from GameEventHandler.
 *
 * Buildings can have hold-markers that prevent normal operation.
 */
class BuildingBase : public PlaceableGameObject
{
public:

    const ResourceMap BUILD_COST;
    const ResourceMap PRODUCTION_EFFECT;

    /**
     * @brief Disabled parameterless constructor.
     */
    BuildingBase() = delete;

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
    explicit BuildingBase(
            const std::shared_ptr<iGameEventHandler>& eventhandler,
            const std::shared_ptr<iObjectManager>& objectmanager,
            const std::shared_ptr<PlayerBase>& owner,
            const int& tilespaces = 1,
            const ResourceMap& buildcost = {},
            const ResourceMap& production = {}
            );

    /**
     * @brief Default destructor.
     */
    virtual ~BuildingBase() = default;

    /**
     * @copydoc GameObject::getType()
     */
    virtual std::string getType() const override;

    /**
     * @brief Performs building's default action.
     */
    virtual void doSpecialAction();

    /**
     * @brief Performs building's possible action after construction.
     */
    virtual void onBuildAction();

    /**
     * @brief Return's building's production as a ResourceMap.\n
     *
     * @note
     * Used by TileBase to get buildings production effect
     * on Tile's production.
     *
     */
    virtual ResourceMap getProduction();

    /**
     * @brief Adds the amount to hold-markers.
     * @note Negative amounts can be used for substraction.
     * @param amount the amount being added.
     * @post Exception guarantee: No-throw
     */
    virtual void addHoldMarkers(int amount) final;

    /**
     * @brief Returns the amount of hold-markers.
     * @return value in m_hold.
     * @post Exception guarantee: No-throw
     */
    virtual int holdCount() const final;

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
            const std::shared_ptr<TileBase> &target) const override;

private:
    int m_hold;

}; // class BuildingBase

} // namespace Course


#endif // BUILDINGBASE_H
