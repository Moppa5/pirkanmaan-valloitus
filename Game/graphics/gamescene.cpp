#include "gamescene.hh"
#include <iostream>

namespace Game {

GameScene::GameScene(int mapWidth, int mapHeight, int tileSize, ObjectManager* objman)
	: QGraphicsScene(0, 0, mapWidth*tileSize, mapHeight*tileSize), objmanager_(objman)
{
	mapWidth_ = mapWidth;
	mapHeight_ = mapHeight;
	tileScale_ = tileSize;
}

void GameScene::resize()
{
    if ( mapBoundRect_ != nullptr ){
        QGraphicsScene::removeItem(mapBoundRect_);
    }

    // Calculates rect with middle at (0,0).
    // Basically left upper corner coords and then width and height
    const QRect &rect = QRect( mapWidth_ * tileScale_, mapHeight_ * tileScale_,
						mapWidth_ * tileScale_ - 1, mapHeight_ * tileScale_ - 1 );

    addRect(rect, QPen(Qt::black));
    setSceneRect(rect);
    mapBoundRect_ = itemAt(rect.topLeft(), QTransform());
    // Draw on the bottom of all items
	mapBoundRect_->setZValue(-1);
}

void GameScene::loadTiles()
{
    for(const auto &tile : objmanager_->getTiles()){
		drawItem(tile);
	}
}

void GameScene::drawItem(const std::shared_ptr<Course::GameObject> &obj)
{
    Game::MapItem* nItem = new Game::MapItem(
				obj, tileScale_);

    // Fixes something that prevents clicking all tiles
	QPixmap pix(tileScale_, tileScale_);

    nItem->setPixmap(pix);

    const QPoint &position = QPoint(obj->getCoordinate().x()*tileScale_,
                                    obj->getCoordinate().y()*tileScale_);
	//qDebug() << "created tile at " << obj->getCoordinate().asQpoint() << " turned into " << position;
	nItem->setPos(position);

	addItem(nItem);
}

void GameScene::drawClaimBorders()
{
	///// Delete old lines /////
    for(const auto &line : borderLines_){
		this->removeItem(line);
	}
	borderLines_.clear();

	///// Gather vertical lines /////
	// Inner y, outer x
	std::vector<std::vector<std::pair<QColor, QColor>>> lines_ver(mapWidth_+1);
	for(unsigned int i=0; i<lines_ver.size(); i++){
		lines_ver.at(i) = std::vector<std::pair<QColor, QColor>>(
							mapHeight_, std::make_pair(nullptr, nullptr));
	}
	std::vector<std::vector<std::pair<QColor, QColor>>> lines_hor(mapWidth_);
	for(unsigned int i=0; i<lines_hor.size(); i++){
		lines_hor.at(i) = std::vector<std::pair<QColor, QColor>>(
							mapHeight_+1, std::make_pair(nullptr, nullptr));
	}

	// Walk through lines between tiles row by row
	for(int y=0; y<mapHeight_; y++){
		// Map left edge
		Player* owner = static_cast<Player*>(
					objmanager_->getTile(
						Course::Coordinate(0, y))
					.get()->getOwner().get());
		if(owner != nullptr){
			lines_ver.at(0).at(y).second = owner->getColor();
			//qDebug() << "Claim found at" << y << "0";
		}

		// Middle tiles
		for(int x=1; x<mapWidth_; x++){
			// Lefter tile
			QColor first_tile_colour = nullptr;
			owner = static_cast<Player*>(
						objmanager_->getTile(
							Course::Coordinate(x-1, y))
						.get()->getOwner().get());
			if(owner != nullptr){
				first_tile_colour = owner->getColor();
			}
			// Righter tile
			QColor second_tile_colour = nullptr;
			owner = static_cast<Player*>(
						objmanager_->getTile(
							Course::Coordinate(x, y))
						.get()->getOwner().get());
			if(owner != nullptr){
				second_tile_colour = owner->getColor();
			}

			// No border necessary if the colours match
			if(first_tile_colour != second_tile_colour){
				lines_ver.at(x).at(y) = std::make_pair(first_tile_colour, second_tile_colour);
				//qDebug() << "Claim found at" << y << x;
			}
		}

		// Map right edge
		owner = static_cast<Player*>(
					objmanager_->getTile(
						Course::Coordinate(0, y))
					.get()->getOwner().get());
		if(owner != nullptr){
			lines_ver.at(mapWidth_).at(y).first = owner->getColor();
			//qDebug() << "Claim found at" << y << "31";
		}
	}

	///// Gather horizontal lines /////
	// Walk through lines between tiles column by column
	for(int x=0; x<mapWidth_; x++){
		// Map left edge
		Player* owner = static_cast<Player*>(
					objmanager_->getTile(
						Course::Coordinate(x, 0))
					.get()->getOwner().get());
		if(owner != nullptr){
			lines_hor.at(x).at(0).second = owner->getColor();
		}

		// Middle tiles
		for(int y=1; y<mapHeight_; y++){
			// Upper tile
			QColor first_tile_colour = nullptr;
			owner = static_cast<Player*>(
						objmanager_->getTile(
							Course::Coordinate(x, y-1))
						.get()->getOwner().get());
			if(owner != nullptr){
				first_tile_colour = owner->getColor();
			}
			// Lower tile
			QColor second_tile_colour = nullptr;
			owner = static_cast<Player*>(
						objmanager_->getTile(
							Course::Coordinate(x, y))
						.get()->getOwner().get());
			if(owner != nullptr){
				second_tile_colour = owner->getColor();
			}

			// No border necessary if the colours match
			if(first_tile_colour != second_tile_colour){
				lines_hor.at(x).at(y) = std::make_pair(first_tile_colour, second_tile_colour);
			}
		}

		// Map right edge
		owner = static_cast<Player*>(
					objmanager_->getTile(
						Course::Coordinate(x, 0))
					.get()->getOwner().get());
		if(owner != nullptr){
			lines_hor.at(x).at(mapHeight_).first = owner->getColor();
		}
	}

	///// Draw the lines /////
	// Use paths for this later
    for(int x=0; x<mapWidth_+1; x++){
        for(int y=0; y<mapHeight_; y++){
			QColor color = lines_ver.at(x).at(y).first;
			if(color != nullptr){
				borderLines_.push_back(addLine(QLine(x*tileScale_, y*tileScale_, x*tileScale_, (y+1)*tileScale_),
						color));
			}
			color = lines_ver.at(x).at(y).second;
			if(color != nullptr){
				borderLines_.push_back(addLine(QLine(x*tileScale_, y*tileScale_, x*tileScale_, (y+1)*tileScale_),
						color));
			}
		}
	}
    for(int y=0; y<mapHeight_+1; y++){
        for(int x=0; x<mapWidth_; x++){
			QColor color = lines_hor.at(x).at(y).first;
			if(color != nullptr){
				borderLines_.push_back(addLine(QLine(x*tileScale_, y*tileScale_, (x+1)*tileScale_, y*tileScale_),
						color));
			}
			color = lines_hor.at(x).at(y).second;
			if(color != nullptr){
				borderLines_.push_back(addLine(QLine(x*tileScale_, y*tileScale_, (x+1)*tileScale_, y*tileScale_),
						color));
			}
		}
	}
}

void GameScene::drawBordersToTiles(std::vector<MapItem *> tiles)
{
    // to be implemented
    Q_UNUSED(tiles);
}

void GameScene::calculateBorders()
{
    // to be implemented
    return;
}

void GameScene::highlightTile(MapItem *obj, const bool &highlightOn)
{
	if(obj == nullptr){
		return;
	}

	if(highlightOn){
		obj->addHighlight();
	} else {
		obj->removeHighlight();
	}
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	screenClickPosition_ = event->screenPos();
	clickedItem_ = itemAt(event->scenePos(), QTransform());
	//qDebug() << "clicked at " << screenClickPosition_ << " item " << clickedItem_;

	if(clickedItem_ == nullptr){
		QGraphicsScene::mousePressEvent(event);
	}
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(clickedItem_ == nullptr){
		return;
	}

	// Minimum amount of pixels which is considered moving and not just a twitch in clicking
    const int &minimumMovement = 10;
	//qDebug() << "released click at " << event->scenePos();
	if(abs(screenClickPosition_.x()-event->screenPos().x()) < minimumMovement &&
			abs(screenClickPosition_.y() - event->screenPos().y()) < minimumMovement){
		Game::MapItem* itemObject = static_cast<Game::MapItem*>(clickedItem_);

		std::shared_ptr<Course::GameObject> item = itemObject->getTileObject();
		std::string owner = "";

		if (item->getOwner() != nullptr) {
			owner = item->getOwner()->getName();
		}
		emit tileInfo(QString::fromStdString(item->getType()),
					  itemObject,itemObject->getItem(),owner);
	}
	QGraphicsScene::mouseReleaseEvent(event);
}

}
