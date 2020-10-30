#include "forest.h"


namespace Course {

Forest::Forest(const Coordinate& location,
               const std::shared_ptr<iGameEventHandler>& eventhandler,
               const std::shared_ptr<iObjectManager>& objectmanager,
               const unsigned int& max_build,
               const unsigned int& max_work,
               const ResourceMap& production):
    TileBase(location,
             eventhandler,
             objectmanager,
             max_build,
             max_work,
             production)
{
}

std::string Forest::getType() const
{
    return "Forest";
}

void Forest::addBuilding(const std::shared_ptr<BuildingBase>& building)
{
    TileBase::addBuilding(building);
    building->addHoldMarkers(1);
}

} // namespace Course
