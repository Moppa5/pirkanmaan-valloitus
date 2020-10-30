#include "placeablegameobject.h"
#include "tiles/tilebase.h"

#include "exceptions/ownerconflict.h"

namespace Course {

PlaceableGameObject::PlaceableGameObject(
        const std::shared_ptr<iGameEventHandler>& eventhandler,
        const std::shared_ptr<iObjectManager>& objectmanager,
        const std::shared_ptr<PlayerBase>& owner,
        const int& tilespace
        ):
    GameObject(owner, eventhandler, objectmanager),
    TILESPACES(tilespace),
    m_location({})
{
}

std::string PlaceableGameObject::getType() const
{
    return "PlaceableGameObject";
}

int PlaceableGameObject::spacesInTileCapacity() const
{
    return TILESPACES;
}

bool PlaceableGameObject::canBePlacedOnTile(
        const std::shared_ptr<TileBase>& target) const
{
    if( target->getOwner() == nullptr or getOwner() == nullptr )
    {
        return true;
    }

    return hasSameOwnerAs(target);
}

void PlaceableGameObject::setLocationTile(const std::shared_ptr<TileBase>& tile)
{
    if( tile )
    {
        if( not canBePlacedOnTile(tile) )
        {
            throw IllegalAction("Illegal action for " + getType());
        }
        setCoordinate(tile->getCoordinate());
    }
    else
    {
        unsetCoordinate();
    }
    m_location = tile;
}

std::shared_ptr<TileBase> PlaceableGameObject::currentLocationTile() const
{
    return m_location.lock();
}


} // namespace Course
