#ifndef PLACEABLEGAMEOBJECT_H
#define PLACEABLEGAMEOBJECT_H

#include "gameobject.h"


namespace Course {

class TileBase;

/**
 * @brief The PlaceableGameObject class represents GameObjects that can be
 * placed on Tile Objects.
 *
 * Handles placement on a Tile and throws exception if the placement would
 * violate PlaceableGameObjects own placement rules.
 *
 * @note One new overridable method: canBePlacedOnTile - Can be used to
 * re-specify placement-rules.
 */
class PlaceableGameObject : public GameObject
{
public:
    const int TILESPACES;

    /**
     * @brief Disabled parameterless constructor.
     */
    PlaceableGameObject() = delete;

    /**
     * @brief Constructor for the class.
     *
     * @param eventhandler points to the student's GameEventHandler.
     * @param objectmanager points to the studtent's ObjectManager
     * @param owner points to the owning player.
     * @param tilespace indicates the amount of tilespace the object would
     * take when placed on a tile.
     */
    explicit PlaceableGameObject(
            const std::shared_ptr<iGameEventHandler>& eventhandler,
            const std::shared_ptr<iObjectManager>& objectmanager,
            const std::shared_ptr<PlayerBase>& owner,
            const int& tilespace = 1
            );

    /**
     * @brief Default destructor.
     */
    virtual ~PlaceableGameObject() = default;

    /**
     * @copydoc GameObject::getType()
     */
    virtual std::string getType() const override;

    /**
     * @brief How many spaces does the GameObject take from a Tile's capacity.
     * @return Amount of spaces that is being taken.
     * @post Exception guarantee: No-throw
     */
    virtual int spacesInTileCapacity() const;

    /**
     * @brief Check if the object's own placement rules allow placement on the
     * specified tile.
     * @param target is a pointer to the tile that is being targeted.
     * @return \n
     * True - Object has same owner as the tile.\n
     * False - If condition doesn't match.\n
     * @post Exception guarantee: Basic
     * @note Override to change default behaviour
     */
    virtual bool canBePlacedOnTile(const std::shared_ptr<TileBase>& target) const;

    /**
     * @brief Set the PlaceableGameObject's location.
     * @param tile points to the Tile where the object is placed.
     * @post Exception guarantee: Strong
     * @note nullptr can be used to clear the location.
     * @exception
     * IllegalAction - If canBePlacedOnTile returns False.
     */
    virtual void setLocationTile(const std::shared_ptr<TileBase>& tile) final;

    /**
     * @brief Returns a shared_ptr to current location-tile.
     * @return
     * @post Exception guarantee: No-throw
     */
    virtual std::shared_ptr<TileBase> currentLocationTile() const final;

private:
    std::weak_ptr<TileBase> m_location;

}; // class PlaceableGameObject

} // namespace Course


#endif // PLACEABLEGAMEOBJECT_H
