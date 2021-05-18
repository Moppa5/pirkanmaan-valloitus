#ifndef GAMESCENE_HH
#define GAMESCENE_HH

#include "interfaces/objectmanager.hh"
#include "core/gameobject.h"
#include "constants/constants.hh"
#include "graphics/mapitem.hh"
#include <math.h>
#include "core/playerbase.h"
#include "tiles/tilebase.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QDebug>
#include <memory>

// Forward declerations
class ObjectManager;

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
	GameScene(int mapWidth, int mapHeight, int tileSize, ObjectManager* objman);

    /**
     * @brief Calculates scene area boundaries and boundRect for it
     */
    void resize();

	/**
	 * @brief Loads tiles to the scene from the objectmanager
	 */
	void loadTiles();

    /**
     * @brief Draws item
     * @param obj to draw
     */
    void drawItem(const std::shared_ptr<Course::GameObject> &obj);

	/**
	 * @brief Draws borders to tilles based on claims
	 */
	void drawClaimBorders();

	/**
	 * @brief Draws a continuos border to tiles given
	 * @param Tiles to be surrounded by border
	 * @note This assumes the area defined by the tiles is continuous
	 */
	void drawBordersToTiles(std::vector<MapItem*> tiles);

	/**
	 * @brief Does the heavy lifting of drawing borders
	 */
	void calculateBorders();

	/**
	 * @brief Highlight the selected tile
	 * @param obj the item to highlight
	 * @param highlightOn bool to highlight or not
	 */
	void highlightTile(MapItem *obj, bool highlightOn=true);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

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
	int tileScale_ = 18; //hardcoded
    QGraphicsItem* mapBoundRect_ = nullptr;

	// Event variables
	QPointF screenClickPosition_;
	QGraphicsItem* clickedItem_ = nullptr;

	// Game state stuff
	ObjectManager* objmanager_;
	std::vector<QGraphicsLineItem*>	borderLines_;
};
}
#endif // GAMESCENE_HH
