#ifndef GAMEMANAGER_HH
#define GAMEMANAGER_HH

#include "constants/constants.hh"
#include "core/resourcemaps.h"
#include "constants/resourcemaps2.h"

#include "dialog/dialog.h"
#include "interfaces/gameeventhandler.hh"
#include "interfaces/objectmanager.hh"
#include "graphics/gamescene.hh"

#include "tiles/forest.h"
#include "tiles/grassland.h"
#include "tiles/lake.h"
#include "tiles/mountain.h"
#include "tiles/ocean.hh"

#include "buildings/farm.h"
#include "buildings/cottage.h"
#include "buildings/headquarters.h"
#include "buildings/mine.h"
#include "buildings/outpost.h"
#include "buildings/fishingboat.hh"

#include "workers/basicworker.h"
#include "workers/farmer.hh"
#include "workers/miner.hh"

#include "core/gameobject.h"
#include "core/player.hh"
#include "core/resourcemaps.h"
#include "core/worldgenerator.h"
#include "core/worldgeneratorperlin.hh"

#include "exceptions/illegalaction.h"
#include "exceptions/ownerconflict.h"

#include <vector>
#include <QString>

namespace Game {

/**
 * @brief The GameManager class is an interface between Game GUI,
 * ObjectManager and GameEventHandler. The class also keeps track of player
 * and game turns / rounds.
 */
class GameManager

{
public:
    GameManager(std::shared_ptr<Dialog> sd,
                std::shared_ptr<GameEventHandler> geh,
                std::shared_ptr<ObjectManager> om,
                std::shared_ptr<GameScene> gs,
                QWidget* parent);

    // returns set map size for handling rejected size
    /**
     * @brief Sets the map size in tiles
     * @param wdith - Width of the map in tiles
     * @param height - Height of the map in tiles
     * @return Map size as a pair
     * @note Automatically applies min and max bounds.
     *  Return can be used to check if the size was valid
     */
    std::pair<int,int> setMapSize(int width, int height);

    /**
     * @brief Gets the map size in tiles
     * @return Map size as a pair
     */
    std::pair<int,int> getMapSize();

    /**
     * @brief claimArea
     * @param tile - Selected tile
     * @pre Must be valid MapItem
     * @post Exception guarantee: Strong
     * @exceptions OwnerConflict - already owned
     * @exceptions IllegalAction - not enough money
     */
    void claimArea(MapItem* tile);

    // Buildings
    /**
     * @brief Adds building to selected tile
     * @param SelectedItem - selected MapItem
     * @param building - Building name as a string, see constants.hh
     * @pre Must be valid MapItem
     * @post Exception guarantee: Strong
     * @exceptions OwnerConflict - Not owned by the current player
     * @exceptions IllegalAction - Cannot be placed / not enough resources
     */
    void addBuildingOnTile(MapItem* selectedItem, QString building);
    /**
     * @brief removeBuildingOnTile
     * @param selectedItem - Selected MapItem
     * @param building - Pointer to the building to be removed
     * @pre Valid MapItem
     * @post Exception guarantee: Strong
     * @note Exceptions raise from called other classes
     */
    void removeBuildingOnTile(MapItem* selectedItem, std::shared_ptr<BuildingBase> building);

    // Workers
    /**
     * @brief addWorkerOnTile
     * @param selectedItem - Selected MapItem
     * @param worker - Worker name as a string, see constants.hh
     * @pre Must be valid MapItem
     * @post Exception guarantee: Strong
     * @exceptions OwnerConflict - Not owned by the current player
     * @exceptions IllegalAction - Not enough resources
     * @note - Does not check if the tile has space for the worker
     */
    void addWorkerOnTile(MapItem* selectedItem, QString worker);
    /**
     * @brief removeWorkerOnTile
     * @param selectedItem - Selected MapItem
     * @param worker - Pointer to the worker to be removed
     * @pre Valid MapItem
     * @post Exception guarantee: Strong
     * @note Exceptions raise from called other classes
     */
    void removeWorkerOnTile(MapItem* selectedItem, std::shared_ptr<Course::WorkerBase> worker);

    /**
     * @brief calculateResourceProduction
     * @param tile - Selected tile
     * @pre Must be valid tile
     * @post Exception guarantee: No-throw
     * @return Resource production of the tile
     */
    Course::ResourceMap calculateResourceProduction(
            std::shared_ptr<Course::TileBase> tile);

    /**
     * @brief Gets the total turn count of the game
     * @post Exception guarantee: No-throw
     * @return Turn count
     */
    int getTurnCount();

    /**
     * @brief Changes the game turn count
     * @param turnCount - New turn count
     * @post Exception guarantee: No-throw
     * @return True if new turn count is valid, higher than current or min
     * @note Used only when changing turn count during the game
     */
    bool changeTurnCount(int turnCount);

    /**
     * @brief Gets current game turn number
     * @post Exception guarantee: No-throw
     * @return Turn number
     */
    int getCurrentTurnNumber();

    /**
     * @brief Ends the turn and changes to the next player
     * @post Exception guarantee: No-throw
     */
    void endTurn();

    /**
     * @brief Gets the current player object
     * @post Exception guarantee: No-throw
     * @return Player object
     */
    std::shared_ptr<Player> getCurrentPlayer();

    /**
     * @brief Fetches the game scores and orders them
     * @return Ordered score data
     */
    std::map<int, std::string> getScores();

    bool gameOver_ = true;

private:
    /**
     * @brief Adds player to the game
     * @param name - Name of the player
     * @param objects - GameObjects that belong to the player
     * @post Exception guarantee: No-throw
     */
    void addPlayer(std::pair<QString, QColor> name, std::vector<std::shared_ptr<Course::GameObject>> objects={});

    /**
     * @brief Creates Building object for addBuildingOnTile
     * @param type - Building name as string
     * @pre Valid building type
     * @post Exception guarantee: No-throw
     * @return Building object
     */
    std::shared_ptr<Course::BuildingBase> createBuilding(QString& type);

    /**
     * @brief Creates Worker object for addWorkerOnTile
     * @param type - Worker name as string
     * @pre Valid worker type
     * @post Exception guarantee: No-throw
     * @return Worker object
     */
    std::shared_ptr<Course::WorkerBase> createWorker(QString& type);

    /**
     * @brief Generates the world
     * @post Exception guarantee: No-throw
     */
    void GenerateWorld();

    /**
     * @brief Does necessary turn actions
     * @post Exception guarantee: No-throw
     */
    void doTurn();
    /**
     * @brief Updates all the tiles on the map to correct owners
     * @post Exception guarantee: No-throw
     * @note Used for buildings that claim tiles
     */
    void updateOwners();

    /**
     * @brief Calculates the score of the game
     */
    void calculateScores();

    /**
     * @brief Calculates scores based on player resources
     * @param name of the player
     * @param resources of the player
     */
    void addResourcesToScore(std::string name,
                             Course::ResourceMap resources);

    std::shared_ptr<Dialog> settingsDialog_ = nullptr;
    std::shared_ptr<GameEventHandler> gameEventHandler_ = nullptr;
    std::shared_ptr<ObjectManager> objectManager_ = nullptr;
    std::shared_ptr<GameScene> gameScene_ = nullptr;
    QWidget* parent_ = nullptr;

    int totalTurnCount_ = 0;
    int currentTurnNumber_ = 0;

    std::vector<std::shared_ptr<Player>> players_;
    int currentPlayerIndex_ = 0;

    std::map<std::string, int> playerScores_;

    int mapWidth_;
    int mapHeight_;
};
}

#endif // GAMEMANAGER_HH
