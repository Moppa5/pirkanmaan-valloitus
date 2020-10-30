#include "farmer.hh"

namespace Game {

Farmer::Farmer(const std::shared_ptr<GameEventHandler>& eventhandler,
                         const std::shared_ptr<ObjectManager>& objectmanager,
                         const std::shared_ptr<PlayerBase>& owner,
                         const int& tilespaces,
                         const ResourceMap& cost,
                         const ResourceMapDouble& efficiency):
    WorkerBase(
        eventhandler,
        objectmanager,
        owner,
        tilespaces,
        cost,
        efficiency)
{
}

std::string Farmer::getType() const
{
    return "Farmer";
}

bool Farmer::canBePlacedOnTile(const std::shared_ptr<TileBase> &target) const
{
    return target->getOwner() == getOwner() and
            WorkerBase::canBePlacedOnTile(target);
}

void Farmer::doSpecialAction()
{

}

const ResourceMapDouble Farmer::tileWorkAction()
{
    auto player = getOwner();
    auto events = lockEventHandler();
    double satisfaction = 0;
    BasicResource focus = getResourceFocus();

    ResourceMapDouble final_modifier;

    if ( events->modifyResource(player, BasicResource::FOOD, -1) )
    {
        satisfaction = 0.5;
        if ( events->modifyResource(player, BasicResource::MONEY, -1) )
        {
            satisfaction = 1;
        }
    }


    if( focus != BasicResource::NONE )
    {
        final_modifier[focus] =
                WORKER_EFFICIENCY.at(focus) * (satisfaction + 0.25);
    }
    else
    {
        for( auto it = WORKER_EFFICIENCY.begin();
             it != WORKER_EFFICIENCY.end();
             ++it )
        {
            final_modifier[it->first] = it->second * satisfaction;
        }
    }

    return final_modifier;
}


}
