#include "gamemanager.hh"
#include <iostream>

namespace Game {
GameManager::GameManager(std::shared_ptr<Dialog> sd,
                         std::shared_ptr<GameEventHandler> geh,
                         std::shared_ptr<ObjectManager> om,
                         std::shared_ptr<GameScene> gs,
                         QWidget* parent) :
    settingsDialog_(sd),
    gameEventHandler_(geh),
    objectManager_(om),
    gameScene_(gs),
    parent_(parent)
{
    // Add players
    std::vector<QString> players = settingsDialog_->getPlayers();
    for(auto player : players){
        addPlayer(player);
    }

    currentPlayerIndex_ = 0;
    gameEventHandler_->setPlayers(players_);
    totalTurnCount_ = settingsDialog_->getRounds();
    currentTurnNumber_ = 1;

    setMapSize(30,20);
    GenerateWorld();

    gameOver_ = false;
}

std::pair<int, int> GameManager::setMapSize(int width, int height)
{
    if(width >= MIN_MAP_WIDTH){
        mapWidth_ = width;
    }
    if(height >= MIN_MAP_HEIGHT){
        mapHeight_ = height;
    }

    return std::make_pair(mapWidth_, mapHeight_);
}

std::pair<int, int> GameManager::getMapSize()
{
    return std::make_pair(mapWidth_, mapHeight_);
}

void GameManager::addPlayer(QString name,
                            std::vector<std::shared_ptr<Course::GameObject> >
                            objects)
{
    std::shared_ptr<Player> player = std::make_shared<Player>(name.toStdString(),
                                                              objects);
    player->setResourceMap(STARTING_RESOURCEMAP);
    player->setColor(PLAYER_COLORS.at(currentPlayerIndex_));

    players_.push_back(player);
    currentPlayerIndex_ += 1;
}

void GameManager::claimArea(MapItem* tile)
{
    if (tile->getTileObject()->getOwner() != nullptr) {
        // If the tile has owner already, can't claim
        throw Course::OwnerConflict(ALREADY_OWNED_TILE.toStdString());
    }

    if(!gameEventHandler_->modifyResources(players_.at(currentPlayerIndex_),
                                          CLAIM_COST)){
        throw Course::IllegalAction(NOT_ENOUGH_MONEY.toStdString());
    }

    std::shared_ptr<Course::PlayerBase> player = players_.at(currentPlayerIndex_);
    tile->getTileObject()->setOwner(player);

    // Item color based on player's color
    QColor color = players_.at(currentPlayerIndex_)->getColor();
    gameScene_->addClaim(tile, color);
    gameScene_->update(tile->boundingRect());
}

void GameManager::doTurn()
{
    // List of tiles that didn't have enough resources to operate
    std::vector<std::shared_ptr<Course::TileBase>> poorTiles;

    for(auto tile : objectManager_->getTiles()){
        // If not enough resources
        if(!tile->generateResources()){
            poorTiles.push_back(tile);
        }
    }

    // Test if poorTiles have enough resources now after other
    // tiles have generated them
    for(auto tile : poorTiles){
        // Find mapitem of the tile
        MapItem* mapItem = nullptr;
        for(auto m : gameScene_->items()){
            mapItem = static_cast<Game::MapItem*>(m);

            if(mapItem->getTileObject() == tile){
                break;
            }
        }
        if(mapItem != nullptr){
            for(auto w : tile->getWorkers()){
                removeWorkerOnTile(mapItem, w);
            }

            for(auto b : tile->getBuildings()){
                removeBuildingOnTile(mapItem, b);
            }
        }
    }
}

void GameManager::updateOwners()
{
    gameScene_->refreshScene(players_);
}

void GameManager::calculateScores()
{
    playerScores_.clear();
    for(std::shared_ptr<Player> p : players_){
        std::string name = p->getName();
        playerScores_.insert(std::pair<std::string, int>(name, 0));

        // Add resources
        Course::ResourceMap resources = *(p->getResourceMap());
        addResourcesToScore(name, resources);
    }

    // Add buildings and workers
    for(std::shared_ptr<Course::TileBase> tile : objectManager_->getTiles()){
        if(tile->getOwner() != nullptr){
            playerScores_[tile->getOwner()->getName()] += 50;
        }

        for(std::shared_ptr<Course::BuildingBase> building : tile->getBuildings()){
            if(building->getOwner() != nullptr){
                std::string owner = building->getOwner()->getName();
                addResourcesToScore(owner, building->BUILD_COST);
            }
        }
        for(std::shared_ptr<Course::WorkerBase> worker : tile->getWorkers()){
            std::string owner = worker->getOwner()->getName();
            addResourcesToScore(owner, worker->RECRUITMENT_COST);
        }
    }
}

void GameManager::addResourcesToScore(std::string name,
                                      Course::ResourceMap resources)
{
    playerScores_[name] += resources[WOOD];
    playerScores_[name] += resources[FOOD];
    playerScores_[name] += resources[STONE];
    playerScores_[name] += resources[ORE] * 2;
    playerScores_[name] += resources[MONEY] * 2;
}

void GameManager::endTurn()
{
    if(!gameOver_){
        doTurn();
    }
    else{
        return;
    }

    // Increment if last player during this turn
    if(currentPlayerIndex_ == static_cast<int>(players_.size()-1)){
        currentTurnNumber_ += 1;
        currentPlayerIndex_ = 0;

        if(currentTurnNumber_ > totalTurnCount_){
            gameOver_ = true;
            currentTurnNumber_ = totalTurnCount_;
        }
    } else{
        currentPlayerIndex_ += 1;
    }
}

void GameManager::addBuildingOnTile(MapItem* selectedItem, QString building)
{
    std::shared_ptr<Course::TileBase> tile =
            objectManager_->getTile(selectedItem->getTileObject()->getCoordinate());
    if(tile->hasSpaceForBuildings(1) == false){
        throw Course::IllegalAction(TOO_MANY_BUILDINGS.toStdString());
    }
    if(selectedItem->getTileObject()->getOwner() != players_.at(currentPlayerIndex_)){
        throw Course::OwnerConflict(NOT_OWNED_TILE.toStdString());
    }

    // Test if enough resources. Has to be done as last test currently
    Course::ResourceMap cost;

    if(building ==  FARM){
        cost = Course::ConstResourceMaps::FARM_BUILD_COST;
    } else if(building == HQ){
        cost = Course::ConstResourceMaps::HQ_BUILD_COST;
    } else if(building == OUTPOST){
        cost = Course::ConstResourceMaps::OUTPOST_BUILD_COST;
    } else if(building == LAKE_COTTAGE){
        cost = Game::LAKE_COTTAGE_BUILD_COST;
    } else if(building == MINE){
        cost = Game::MINE_BUILD_COST;
    } else if(building == FISHING_BOAT){
        cost = Game::FISHING_BOAT_BUILD_COST;
    }

    // Check if can be built
    std::shared_ptr<Course::BuildingBase> actualBuilding = createBuilding(building);

    if(!actualBuilding->canBePlacedOnTile(tile)){
        throw Course::IllegalAction(CANT_BE_BUILT.toStdString());
    }

    // Check for Ocean
    if(selectedItem->getTileObject()->getType() == OCEAN.toStdString() && building != FISHING_BOAT){
        throw Course::IllegalAction(CANT_BE_BUILT.toStdString());
    }

    // Check for Lake
    if(selectedItem->getTileObject()->getType() == LAKE.toStdString() &&
            !(building == FISHING_BOAT || building == LAKE_COTTAGE)){
        throw Course::IllegalAction(CANT_BE_BUILT.toStdString());
    }

    Course::ResourceMap buildCost = Course::multiplyResourceMap(
                cost, Game::RESOURCEMAP_NEGATIVE);
    if(!gameEventHandler_->modifyResources(players_.at(currentPlayerIndex_),
                                          buildCost)){
        throw Course::IllegalAction(NOT_ENOUGH_RESOURCES.toStdString());
    }

    objectManager_->addBuilding(actualBuilding);
    tile->addBuilding(actualBuilding);
    actualBuilding->onBuildAction();
    updateOwners();

    // Graphics
    selectedItem->setBuildingOnTile(building);
    gameScene_->update(selectedItem->boundingRect());
}

std::shared_ptr<Course::BuildingBase> GameManager::createBuilding(
        QString& type)
{
    std::shared_ptr<Course::BuildingBase> building;

    if(type ==  FARM){
            building = std::make_shared<Course::Farm>(gameEventHandler_, objectManager_,
                                        players_.at(currentPlayerIndex_));
    } else if(type == HQ){
        building = std::make_shared<Course::HeadQuarters>(gameEventHandler_, objectManager_,
                                    players_.at(currentPlayerIndex_));
    } else if(type == OUTPOST){
        building = std::make_shared<Course::Outpost>(gameEventHandler_, objectManager_,
                                    players_.at(currentPlayerIndex_));
    } else if(type == LAKE_COTTAGE){
        building = std::make_shared<Game::Cottage>(gameEventHandler_, objectManager_,
                                    players_.at(currentPlayerIndex_));
    } else if(type == MINE){
        building = std::make_shared<Game::Mine>(gameEventHandler_, objectManager_,
                                    players_.at(currentPlayerIndex_));
    } else if(type == FISHING_BOAT){
        building = std::make_shared<Game::FishingBoat>(gameEventHandler_, objectManager_,
                                    players_.at(currentPlayerIndex_));
    } else{
        return nullptr;
    }

    return building;
}

void GameManager::removeBuildingOnTile(MapItem *selectedItem, std::shared_ptr<BuildingBase> building)
{
    selectedItem->removeBuildingOnTile(QString::fromStdString(building->getType()));
    gameScene_->update(selectedItem->boundingRect());
    objectManager_->removeBuilding(building);
}

void GameManager::addWorkerOnTile(MapItem *selectedItem, QString worker)
{
    if (!selectedItem->claimed() or
            selectedItem->getTileObject()->getOwner() !=
                        players_.at(currentPlayerIndex_)) {
        throw Course::OwnerConflict(NOT_OWNED_TILE.toStdString());
    }

    ResourceMap cost;

    if (worker == WORKER_BASIC) {
        cost = Course::ConstResourceMaps::BW_RECRUITMENT_COST;
    } else if (worker == WORKER_FARMER) {
        cost = Game::FARMER_RECRUITMENT_COST;
    } else if (worker == WORKER_MINER) {
        cost = Game::MINER_RECRUITMENT_COST;
    }
    Course::ResourceMap buildCost = Course::multiplyResourceMap(
                cost, Game::RESOURCEMAP_NEGATIVE);

    if(!gameEventHandler_->modifyResources(players_.at(currentPlayerIndex_),
                                          buildCost)){
        throw Course::IllegalAction(NOT_ENOUGH_RESOURCES.toStdString());
    }

    std::shared_ptr<Course::TileBase> tile =
            objectManager_->getTile(selectedItem->getTileObject()->getCoordinate());

    // Graphics
    selectedItem->setWorkerOnTile(worker);

    // Create worker
    std::shared_ptr<Course::WorkerBase> actualWorker = createWorker(worker);
    objectManager_->addWorker(actualWorker);
    tile->addWorker(actualWorker);
    gameScene_->update(selectedItem->boundingRect());
    gameScene_->tileInfo(QString::fromStdString(selectedItem->getTileObject()->getType()),
                         selectedItem,
                         selectedItem->getItem(),
                         selectedItem->getTileObject()->getType());
}

std::shared_ptr<WorkerBase> GameManager::createWorker(QString &type)
{
    std::shared_ptr<Course::WorkerBase> worker;

    if(type == WORKER_BASIC){
        worker = std::make_shared<Course::BasicWorker>(gameEventHandler_, objectManager_,
                                                      players_.at(currentPlayerIndex_), 1,
                                                      Course::ConstResourceMaps::BW_RECRUITMENT_COST,
                                                       BW_WORKER_EFFICIENCY);
    } else if(type == WORKER_FARMER){
        worker = std::make_shared<Game::Farmer>(gameEventHandler_, objectManager_,
                                                      players_.at(currentPlayerIndex_));
    } else if(type == WORKER_MINER){
        worker = std::make_shared<Game::Miner>(gameEventHandler_, objectManager_,
                                                      players_.at(currentPlayerIndex_));
    } /*else if(type == WORKER_SABOTEUR){
        worker = std::make_shared<Course::BasicWorker>(gameEventHandler_, objectManager_,
                                                      players_.at(currentPlayerIndex_));
    }*/

    return worker;
}

void GameManager::removeWorkerOnTile(MapItem* selectedItem, std::shared_ptr<WorkerBase> worker)
{
    selectedItem->removeWorkerOnTile(QString::fromStdString(worker->getType()));
    gameScene_->update(selectedItem->boundingRect());
    objectManager_->removeWorker(worker);
}

ResourceMap GameManager::calculateResourceProduction(std::shared_ptr<TileBase> tile)
{
    ResourceMapDouble worker_efficiency = RESOURCEMAP_ZERO_DOUBLE;
    ResourceMap total_production;

    std::vector<std::shared_ptr<Course::WorkerBase>> workers = tile->getWorkers();
    std::vector<std::shared_ptr<Course::BuildingBase>> buildings = tile->getBuildings();

    for(std::shared_ptr<Course::WorkerBase> work_it : workers)
    {
        // Copied from basicworker
        auto player = work_it->getOwner();
        double satisfaction = 0;
        BasicResource focus = work_it->getResourceFocus();

        ResourceMapDouble final_modifier;

        if (gameEventHandler_->modifyResource(player, BasicResource::FOOD, -1, false))
        {
            satisfaction = 0.5;
            if (gameEventHandler_->modifyResource(player, BasicResource::MONEY, -1, false))
            {
                satisfaction = 1;
            }
        }

        if( focus != BasicResource::NONE )
        {
            final_modifier[focus] =
                    work_it->WORKER_EFFICIENCY.at(focus) * (satisfaction + 0.25);
        }
        else
        {
            for( auto it = work_it->WORKER_EFFICIENCY.begin();
                 it != work_it->WORKER_EFFICIENCY.end();
                 ++it )
            {
                final_modifier[it->first] = it->second * satisfaction;
            }
        }

        // Copy ends

        ResourceMapDouble current_efficiency = final_modifier;

        worker_efficiency = mergeResourceMapDoubles(worker_efficiency, current_efficiency);
    }

    total_production = multiplyResourceMap(tile->BASE_PRODUCTION, worker_efficiency);

    for(std::shared_ptr<Course::BuildingBase> build_it : buildings)
    {
        ResourceMap current_production = build_it->getProduction();

        total_production = mergeResourceMaps(total_production,
                                             current_production);
    }

    return total_production;
}

int GameManager::getTurnCount()
{
    return totalTurnCount_;
}

bool GameManager::changeTurnCount(int turnCount)
{
    if(turnCount >= MIN_ROUND_COUNT && turnCount >= currentTurnNumber_)
    {
        totalTurnCount_ = turnCount;
        return true;
    }
    else
    {
        return false;
    }
}

int GameManager::getCurrentTurnNumber()
{
    return currentTurnNumber_;
}

std::shared_ptr<Player> GameManager::getCurrentPlayer()
{
    return players_.at(currentPlayerIndex_);
}

std::map<int, std::string> GameManager::getScores()
{
    calculateScores();
    std::map<int, std::string> ordered;
    for(const auto& i : playerScores_){
        ordered.insert(std::pair<int, std::string>(i.second, i.first));
    }

    return ordered;
}

void GameManager::GenerateWorld()
{
    //Course::WorldGenerator& worldGenerator = Course::WorldGenerator::getInstance();
    /*worldGenerator.addConstructor<Course::Forest>(FOREST_RARITY);
    worldGenerator.addConstructor<Course::Grassland>(GRASSLAND_RARITY);
    worldGenerator.addConstructor<Game::Mountain>(MOUNTAIN_RARITY);*/

    WorldGeneratorPerlin& worldGenerator = WorldGeneratorPerlin::getInstance();
    worldGenerator.addConstructor<Game::Ocean>(0, 0.2);
    worldGenerator.addConstructor<Course::Forest>(0.2, 0.6);
    worldGenerator.addConstructor<Game::Lake>(0.4,0.41);
    worldGenerator.addConstructor<Game::Lake>(0.5,0.51);
    worldGenerator.addConstructor<Course::Grassland>(0.5, 0.8);
    worldGenerator.addConstructor<Game::Mountain>(0.8, 1);

    worldGenerator.generateMap(mapWidth_, mapHeight_, settingsDialog_->getSeed(),
                               objectManager_, gameEventHandler_);
}


}
