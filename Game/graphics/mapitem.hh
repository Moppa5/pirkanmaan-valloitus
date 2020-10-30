#ifndef MAPITEM_HH
#define MAPITEM_HH

#include "core/gameobject.h"
#include <constants/constants.hh>
#include "core/player.hh"
#include <memory>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

namespace Game {

/**
 * @brief The MapItem class is QGraphicsPixmapItem based scene item that
 * represents a single gameobject's graphics. This class is also used to
 * determine how things in a single item should be drawn under
 * certain conditions.
 */
class MapItem : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Constructor for MapItem class
     * @param obj that mapitem represents graphics for
     * @param size for item
     */
    MapItem(const std::shared_ptr<Course::GameObject> &obj, int size);

    /**
     * @brief Calculates the boundings for this item
     * @return QRectF that bounds the item
     */
    QRectF boundingRect() const;

    /**
     * @brief Check claimed status
     * @return Return true if claimed
     */
    bool claimed();

    /**
     * @brief Paints the tile, buildings and workers
     * @param painter pointer for painting
     * @param option unused style pointer
     * @param widget unused widget pointer
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    /**
     * @brief Fetches the object that this item is for
     * @return GameObject that item was construced for
     */
    const std::shared_ptr<Course::GameObject> &getTileObject();

    /**
     * @brief Sets building image path and type
     * @param building to add
     */
    void setBuildingOnTile(const QString &building);
    /**
     * @brief Removes the building path and type from graphics
     * @param building to remove
     */
    void removeBuildingOnTile(const QString &building);

    /**
     * @brief Sets worker on tile as graphics
     * @param worker to add
     */
    void setWorkerOnTile(const QString &worker);
    /**
     * @brief Removes the worker
     * @param worker to remove
     */
    void removeWorkerOnTile(const QString &worker);

    /**
     * @brief Fetches the tile QPixmap for this item
     * @return QPixmap for this item
     */
    QPixmap getItem();

    /**
     * @brief Adds highlight for this tile
     */
    void addHighlight();
    /**
     * @brief Removes the hightlight from this tile
     */
    void removeHighlight();

    /**
     * @brief Adds border for this tile
     * @param Border color
     */
    void addBorder(QColor color);

    /**
     * @brief Removes the border
     */
    void removeBorder();

    /**
     * @brief Sets claim color
     * @param Player's color
     */
    void setClaimColor(const QColor &color);

    /**
     * @brief Fetches the claim color for this item
     * @return Claim color or nullptr if not set
     */
    QColor getClaimColor();

private:
    /**
     * @brief Draws the tileImage
     * @param painter pointer
     */
    void drawTileImage(QPainter* painter);

    /**
     * @brief Draws the buildings
     * @param painter pointer
     */
    void drawBuildings(QPainter* painter);

    /**
     * @brief Draws the workers
     * @param painter pointer
     */
    void drawWorkers(QPainter* painter);

    /**
     * @brief draw the highlight
     * @param painter pointer
     */
    void drawHighlight(QPainter* painter);

    /**
     * @brief draw the border
     * @param painter pointer
     */
    void drawBorder(QPainter* painter);

    /**
     * @brief draw the claim
     * @param painter pointer
     */
    void drawClaim(QPainter* painter);

    const std::shared_ptr<Course::GameObject> itemObject_;
    int size_;
    QPoint sceneLocation_;

    QPixmap currentItem_;
    bool claimed_ = false;
    // Shows owner
    QColor claimColor_ = nullptr;
    QPen claimPen_;
    // Highlighting
    QPen highlightPen_;
    QColor highlightColor_;
    // Other border
    QPen borderPen_;
    QColor borderColor_;
    // Tile buildings and workers for efficient use
    std::vector<QString> buildings_;
    std::vector<QPixmap> buildingImages_;
    std::vector<QString> workers_;
};
}

#endif // MAPITEM_HH
