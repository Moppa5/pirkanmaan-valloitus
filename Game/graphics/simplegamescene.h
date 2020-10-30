#ifndef SIMPLEGAMESCENE_H
#define SIMPLEGAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

#include <map>
#include <memory>

#include "core/gameobject.h"

namespace Course {

const std::pair<int, int> SCENE_WIDTH_LIMITS = {1, 100};
const std::pair<int, int> SCENE_HEIGHT_LIMITS = {1, 100};
const std::pair<int, int> SCENE_SCALE_LIMITS = {1, 500};

/**
 * @brief The SimpleGameScene is a custom QGraphicsScene that shows
 * a simple rendering of the game map.
 */
class SimpleGameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for the class.
     *
     * @param qt_parent points to the parent object per Qt's parent-child-system.
     * @param width in tiles for the game map.
     * @param height in tiles for the game map.
     * @param scale is the size in pixels of a single square tile.
     *
     * @pre 0 < width <= 100 && 0 < height <= 100 && 0 < scale <= 500. Otherwise
     * default values are used for the created object.
     */
    SimpleGameScene(QWidget* qt_parent = nullptr,
                    int width = 10,
                    int height = 10,
                    int scale = 50
            );

    /**
     * @brief Default destructor.
     */
    ~SimpleGameScene() = default;

    /**
     * @brief Sets the map size and calls resize().
     * @param width in tiles.
     * @param height in tiles.
     * @pre width and height must each be between 1 and 100.
     * @post width and height are set to given sizes.
     * @post Exception guarantee: No-throw
     */
    void setSize(int width, int height);

    /**
     * @brief set the tile size, aka scale of the map and calls resize().
     * Function behaviour after objects has been drawn is not specified.
     * @param scale in pixels.
     * @pre 0 < scale <= 500
     * @post Scene scale is set to scale.
     * @post Exception guarantee: None
     */
    void setScale(int scale);

    /**
     * @brief resize recalculates the bounding rectangle
     */
    void resize();

    /**
     * @brief get the size of a single tile
     * @return the size of a tile in pixels.
     * @post Exception guarantee: No-throw
     */
    int getScale() const;

    /**
     * @brief get the size of the map.
     * @return pair<width, height> in tiles.
     * @post Exception guarantee: No-throw
     */
    std::pair<int,int> getSize() const;

    /**
     * @brief draw a new item to the map.
     * @param obj shared ptr to the object
     * @pre obj must have a valid coordinate property.
     * @post Exception guarantee: None
     */
    void drawItem( std::shared_ptr<Course::GameObject> obj);

    /**
     * @brief tries to remove drawn object at the location obj points to.
     * If there's multiple objects, will remove the one that matches obj.
     * @param obj shared ptr to the object being deleted.
     * @post Exception guarantee: None
     *
     */
    void removeItem( std::shared_ptr<Course::GameObject> obj);

    /**
     * @brief updates the position of obj.
     * @param obj shared ptr to the obj being updated.
     */
    void updateItem( std::shared_ptr<Course::GameObject> obj);

    /**
     * @brief simple event handler that notifies when objects or the play
     * area is clicked.
     * @param event that has happened.
     * @return True: if event was  handled in the handler.
     * False: if the event handling was passed over.
     */
    virtual bool event(QEvent* event) override;

signals:
    void objectClicked(unsigned int object_id);

private:
    QGraphicsItem* m_mapBoundRect;
    int m_width;
    int m_height;
    int m_scale;
};
}

#endif // SIMPLEGAMESCENE_H

