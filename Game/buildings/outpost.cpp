#include "outpost.h"
#include "interfaces/iobjectmanager.h"
#include "tiles/tilebase.h"

namespace Course {

Outpost::Outpost(
        const std::shared_ptr<iGameEventHandler>& eventhandler,
        const std::shared_ptr<iObjectManager>& objectmanager,
        const std::shared_ptr<PlayerBase>& owner,
        const int& tilespaces,
        const ResourceMap& buildcost,
        const ResourceMap& production
        ):
    BuildingBase(eventhandler,
                 objectmanager,
                 owner,
                 tilespaces,
                 buildcost,
                 production)
{
}

std::string Outpost::getType() const
{
    return "Outpost";
}

void Outpost::onBuildAction()
{
    std::vector< std::shared_ptr<TileBase> > neighbours =
            lockObjectManager()->getTiles(getCoordinatePtr()->neighbours(1));

    for(auto it = neighbours.begin(); it != neighbours.end(); ++it)
    {
        // If the Tile doesn't have owner, set it's owner to buildings owner.
        if( not (*it)->getOwner() )
        {
            (*it)->setOwner(getOwner());
        }
    }
    lockObjectManager()->getTile(getCoordinate())->setOwner(getOwner());
}

ResourceMap Outpost::getProduction()
{
    // Outpost has only negative production effect.
    // Ze hold markerz, zey do nothing.
    return PRODUCTION_EFFECT;
}

} // namespace Course
