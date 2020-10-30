#include "gameeventhandler.hh"

namespace Game {

GameEventHandler::GameEventHandler()
{

}

GameEventHandler::~GameEventHandler()
{

}

bool GameEventHandler::modifyResources(std::shared_ptr<PlayerBase> player,
                                       ResourceMap resources)
{
    return modifyResources(player, resources, true);
}

bool GameEventHandler::modifyResources(const std::shared_ptr<PlayerBase> &player,
                                       const ResourceMap &resources,
                                       const bool applyToPlayer)
{
    if(player == nullptr) {
        return true;
    }

    std::shared_ptr<Player> actualPlayer = getPlayer(player);

    // Make merged map and test if valid
    std::shared_ptr<Course::ResourceMap> newResources =
            std::make_shared<Course::ResourceMap>
            (Course::mergeResourceMaps(*(actualPlayer->
                                         getResourceMap()), resources));

    if(validResourceMap(newResources)){
        if(applyToPlayer){
            actualPlayer->setResourceMap(*newResources);
        }
        return true;
    }
    else{
        return false;
    }
}

bool GameEventHandler::modifyResource(std::shared_ptr<Course::PlayerBase> player,
                                      Course::BasicResource resource, int amount)
{
    return modifyResource(player, resource, amount, true);
}

bool GameEventHandler::modifyResource(const std::shared_ptr<PlayerBase> &player,
                                      const BasicResource &resource,
                                      int amount,
                                      const bool applyToPlayer)
{
    if(player == nullptr) {
        throw Course::InvalidPointer("Player not found");
    }

    std::shared_ptr<Player> actualPlayer = getPlayer(player);

    ResourceMap changeMap = {{resource, amount}};

    std::shared_ptr<Course::ResourceMap> newResources =
            std::make_shared<Course::ResourceMap>
            (Course::mergeResourceMaps(*(actualPlayer->
                                         getResourceMap()), changeMap));

    if(validResourceMap(newResources)){
        if(applyToPlayer){
            actualPlayer->setResourceMap(*newResources);
        }
        return true;
    }else{
        return false;
    }
}

std::shared_ptr<Player> GameEventHandler::getPlayer(
        const std::shared_ptr<Course::PlayerBase> &player)
{
    std::shared_ptr<Player> foundPlayer;

    for(std::shared_ptr<Player> p : players_){
        if(p->getName() == player->getName()){
            foundPlayer = p;
        }
    }

    return foundPlayer;
}

void GameEventHandler::setPlayers(const std::vector<std::shared_ptr<Player> > &players)
{
    players_ = players;
}

bool GameEventHandler::validResourceMap(const std::shared_ptr<Course::ResourceMap> &resources)
{
    for(auto resource : *resources){
        if(resource.second < 0){
            return false;
            break;
        }
    }
    return true;
}
}
