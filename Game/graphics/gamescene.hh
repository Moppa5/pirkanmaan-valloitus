#ifndef GAMESCENE_HH
#define GAMESCENE_HH

#include "core/gameobject.h"
#include "constants/constants.hh"
#include "graphics/mapitem.hh"
#include <math.h>
#include "core/playerbase.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QDebug>
#include <memory>

namespace Game {

/**
 * @brief The GameScene class is a QGraphicsScene based class to show graphical
 * elements in the game. It's connected to MapWindow class's GraphicsView widget
 */
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief Empty constructor
     */
    explicit GameScene();

    /**
     * @brief Calculates scene area boundaries and boundRect for it
     */
    void resize();

    /**
     * @brief Draws item
     * @param obj to draw
     */
    void drawItem(const std::shared_ptr<Course::GameObject> &obj);

    /**
     * @brief Adds claim for certain object
     * @param obj to add claim
     * @param claimColor based on player's color
     */
    void addClaim(MapItem* obj, const QColor &claimColor);

    /**
     * @brief Removes the claim
     * @param obj to remove the claim from
     */
    void removeClaim(MapItem *obj);

    /**
     * @brief Adds border for item object
     * @param obj to add it for
     * @param borderColor for border
     * @param highlight the tile
     */
    void addBorder(MapItem* obj, const QColor &borderColor,
                   const bool highlight=true);

    /**
     * @brief Removes the border
     * @param obj to remove border from
     * @param highlight the tile
     */
    void removeBorder(MapItem *obj,
                              const bool highlight=true);

    /**
     * @brief Cathces if player clicks mapitem on scene
     * @param event to catch
     * @return true if item is clicked
     */
    virtual bool event(QEvent* event) override;

    /**
     * @brief Refresh the scene items if they're stuck or won't update
     * @param Players on the game
     */
    void refreshScene(const std::vector<std::shared_ptr<Player>> &players);

signals:
    /**
     * @brief Signal for GUI about current selected item
     * @param type of the item
     * @param selectedItem pointer
     * @param itemImage for GUI info
     * @param owner of item
     */
    void tileInfo(const QString &type, Game::MapItem* &selectedItem,
                  const QPixmap &itemImage, const std::string &owner = "");

private:
    // Map size
    int mapWidth_ = 30;
    int mapHeight_ = 20;
    int mapScale_ = 18; //hardcoded
    QGraphicsItem* mapBoundRect_ = nullptr;
};
}
#endif // GAMESCENE_HH
