#include "fishingboat.hh"

namespace Game {

FishingBoat::FishingBoat(const std::shared_ptr<Course::iGameEventHandler> &eventhandler,
                         const std::shared_ptr<Course::iObjectManager> &objectmanager,
                         const std::shared_ptr<Course::PlayerBase> &owner,
                         const int &tilespaces, const Course::ResourceMap &buildcost,
                         const Course::ResourceMap &production):

                        BuildingBase(
                            eventhandler,
                            objectmanager,
                            owner,
                            tilespaces,
                            buildcost,
                            production
                            )
{
}

std::string FishingBoat::getType() const
{
    return "Fishing Boat";
}

bool FishingBoat::canBePlacedOnTile(const std::shared_ptr<Course::TileBase> &target) const
{
    QString type = QString::fromStdString(target->getType());
    if (type == OCEAN || type == LAKE) {
        return true;
    } else {
        return false;
    }
}



}
