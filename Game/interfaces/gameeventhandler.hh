#ifndef GAMEEVENTHANDLER_HH
#define GAMEEVENTHANDLER_HH

#include <interfaces/igameeventhandler.h>
#include <core/player.hh>
#include <core/resourcemaps.h>
#include <constants/resourcemaps2.h>

#include "exceptions/invalidpointer.h"

namespace Game {

/**
 * @brief The GameEventHandler class is Course side based implementation for
 * modifying player's resources, setting up players and fetching them
 */
class GameEventHandler : public Course::iGameEventHandler
{
public:
    GameEventHandler();

    ~GameEventHandler();

    bool modifyResources(std::shared_ptr<PlayerBase> player,
                         ResourceMap resources);

    /**
     * @brief Modify Player's resources. Can be used to both sum or subtract.
     * @param player Pointer to the Player whose resources are being modified.
     * @param resources ResourceMap containing change amounts.
     * @param applyToPlayer - Is the change applied to the player.
     *  Primaly used for testing if player has enough resources
     * @post Exception guarantee: Strong
     * @exception InvalidPointer - missing player
     * @return
     * True - Modification was succesful. \n
     * False - Modification failed. \n
     */
    bool modifyResources(const std::shared_ptr<Course::PlayerBase> &player,
                         const Course::ResourceMap &resources,const bool applyToPlayer);

    bool modifyResource(std::shared_ptr<Course::PlayerBase> player,
                        Course::BasicResource resource, int amount);

    /**
     * @brief Modify Player's resource. Can be used to both sum or subtract.
     * @param player - Pointer to the Player whose resource is being modified.
     * @param resource - Defines the modified resource.
     * @param amount - Defines the amount of change.
     * @param applyToPlayer - Is the change applied to the player.
     *  Primaly used for testing if player has enough resources
     * @post Exception guarantee: Strong
     * @exception InvalidPointer - missing player
     * @return
     * True - Modification was succesful. \n
     * False - Modification failed. \n
     */
    bool modifyResource(const std::shared_ptr<Course::PlayerBase> &player,
                        const Course::BasicResource &resource, int amount, const bool applyToPlayer);

    /**
     * @brief Sets player pointers to this object
     * @param Vector of players
     * @post Exception guarantee: No-throw
     */
    void setPlayers(const std::vector<std::shared_ptr<Player>> &players);

private:
    /**
     * @brief Get Player using PlayerBase
     * @param PlayerBase pointer
     * @return Player pointer
     */
    std::shared_ptr<Player> getPlayer(const std::shared_ptr<Course::PlayerBase> &player);

    /**
     * @brief Checks if the recourcemap is all positive
     * @param ResourceMap to be tested
     * @return True if all >=0, False otherwise
     */
    bool validResourceMap(const std::shared_ptr<Course::ResourceMap> &resources);

    std::vector<std::shared_ptr<Player>> players_;
};
}

#endif // GAMEEVENTHANDLER_HH
