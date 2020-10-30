#include "cottage.h"

namespace Game {

Cottage::Cottage(const std::shared_ptr<Course::iGameEventHandler> &eventhandler,
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

std::string Cottage::getType() const
{
    return "Lake Cottage";
}

bool Cottage::canBePlacedOnTile(const std::shared_ptr<Course::TileBase> &target) const
{
    if (QString::fromStdString(target->getType()) == LAKE) {
        return true;
    } else {
        return false;
    }
}



}
