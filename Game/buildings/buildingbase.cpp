#include "buildingbase.h"
#include "../exceptions/ownerconflict.h"
#include "../tiles/tilebase.h"

#include "qdebug.h"


namespace Course {


BuildingBase::BuildingBase(const std::shared_ptr<iGameEventHandler>& eventhandler,
        const std::shared_ptr<iObjectManager> &objectmanager,
        const std::shared_ptr<PlayerBase>& owner,
        const int& tilespaces,
        const ResourceMap& buildcost,
        const ResourceMap& production):
    PlaceableGameObject(eventhandler, objectmanager, owner, tilespaces),
    BUILD_COST(buildcost),
    PRODUCTION_EFFECT(production),
    m_hold(0)
{
}

std::string BuildingBase::getType() const
{
    return "BuildingBase";
}

void BuildingBase::doSpecialAction()
{
    qDebug() << "BuildingBase - doSpecialAction";
}

void BuildingBase::onBuildAction()
{
    qDebug() << "BuildingBase - onBuildAction";
}

ResourceMap BuildingBase::getProduction()
{
    if( m_hold > 0 )
    {
        m_hold -= 1;
        return {};
    }
    return PRODUCTION_EFFECT;
}

void BuildingBase::addHoldMarkers(int amount)
{
    m_hold += amount;
}

int BuildingBase::holdCount() const
{
    return m_hold;
}

bool BuildingBase::canBePlacedOnTile(const std::shared_ptr<TileBase> &target) const
{
    return PlaceableGameObject::canBePlacedOnTile(target) and
            target->hasSpaceForBuildings(spacesInTileCapacity());
}

} // namespace Course
