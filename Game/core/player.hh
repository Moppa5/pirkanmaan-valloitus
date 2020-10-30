#ifndef PLAYER_HH
#define PLAYER_HH
#include <QColor>
#include "core/playerbase.h"
#include "core/gameobject.h"
#include "core/resourcemaps.h"

#include "exceptions/invalidpointer.h"

namespace Game {

/**
 * @brief The Player class is Course PlayerBase based and implements
 * game player. It also keeps track of a single player's resources.
 */
class Player : public Course::PlayerBase
{
public:
    /**
     * @brief Constructor for the class
     * @param name A std::string for player's name
     * @param objects (optional) A std::vector of shared-pointers to
     * GameObjects.
     */
    Player(const std::string& name,
           const std::vector<std::shared_ptr<Course::GameObject> > objects={});

    /**
     * @brief Gets player's resources
     * @post Exception guarantee: Strong
     * @return Player's resourcemap
     */
    std::shared_ptr<Course::ResourceMap> getResourceMap();

    /**
     * @brief Set resource map for player
     * @param map to be assigned for player
     */
    void setResourceMap(const Course::ResourceMap &map);

    /**
     * @brief Sets resource for player
     * @param resource to modify
     * @param count amount of resource
     */
    void setResource(const Course::BasicResource &resource, int count);
    /**
     * @brief Get player's color
     * @return QColor of player's color
     */
    QColor getColor();

    /**
     * @brief Set color for player
     * @param color for player
     */
    void setColor(const QColor &color);

private:
    std::shared_ptr<Course::ResourceMap> resources_ = nullptr;
    QColor color_ = nullptr;
};
}
#endif // PLAYER_HH
