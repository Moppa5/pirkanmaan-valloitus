#include "mapitem.hh"
#include <QDebug>

namespace Game {

MapItem::MapItem(const std::shared_ptr<Course::GameObject> &obj,
                 int size):
    itemObject_(obj),
    size_(size),
    sceneLocation_(itemObject_->getCoordinatePtr()->asQpoint())
{
    highlightPen_.setWidth(1);
    highlightPen_.setJoinStyle(Qt::MiterJoin);

    claimPen_.setWidth(2);
}

/*QRectF MapItem::boundingRect() const
{
	QPoint topleft = scenePos().toPoint();
	QPoint bottomright = scenePos().toPoint() + QPoint(size_,size_);
    return QRectF(topleft,bottomright);
}*/

bool MapItem::claimed()
{
    return claimed_;
}

void MapItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    drawTileImage(painter);
    drawBuildings(painter);
    drawWorkers(painter);

    if(claimColor_ != "") drawClaim(painter);
    if(borderColor_ != "") drawBorder(painter);
    // Draw this after other borders
    if(highlightColor_ != "") drawHighlight(painter);
}

const std::shared_ptr<Course::GameObject> &MapItem::getTileObject()
{
    return itemObject_;
}

void MapItem::setBuildingOnTile(const QString &building)
{
    // add building on tile
    QPixmap buildImage;
    QString path;

    if (building == FARM) {
        // drawFarmImage
        path = FARM_IMAGE;
    } else if (building == HQ) {
        // draw HQ image
        path = HQ_IMAGE;
    }  else if (building == OUTPOST) {
        // draw OUTPOST image
        path = OUTPOST_IMAGE;
    } else if (building == MINE) {
        // draw Mine
        path = MINE_IMAGE;
    } else if (building == LAKE_COTTAGE) {
        // draw Fishing cabin
        path = LAKE_COTTAGE_IMAGE;
    } else if (building == FISHING_BOAT) {
        // draw FISHING boat image
        path = SHIP_IMAGE;
    }

    buildImage = QPixmap(path);
    buildingImages_.push_back(buildImage);
    buildings_.push_back(building);
}

void MapItem::removeBuildingOnTile(const QString &building)
{
    for(unsigned int i=0; i<buildings_.size(); i++){
        if(buildings_.at(i) == building){
            buildings_.erase(buildings_.begin() + i);
            break;
        }
    }
}

void MapItem::setWorkerOnTile(const QString &worker)
{
    workers_.push_back(worker);
}

void MapItem::removeWorkerOnTile(const QString &worker)
{
    for(unsigned int i=0; i<workers_.size(); i++){
        if(workers_.at(i).replace(" ", "") == worker){
            workers_.erase(workers_.begin() + i);
            break;
        }
    }
}

void MapItem::addHighlight()
{
    highlightPen_.setBrush(HIGHLIGHT_COLOR);
    highlightColor_ = HIGHLIGHT_COLOR;
}

QPixmap MapItem::getItem()
{
    return currentItem_;
}

void MapItem::removeHighlight()
{
    highlightColor_ = "";
}

void MapItem::drawHighlight(QPainter *painter)
{
    if(highlightColor_ != ""){
        painter->setPen(highlightPen_);
        QRectF bounding = boundingRect();
        bounding.setRect(bounding.x()-0.5, bounding.y()-0.5,
                         bounding.width()+0.5, bounding.height()+0.5);
        painter->drawRect(bounding);
    }
}

void MapItem::drawBorder(QPainter *painter)
{
    if(borderColor_ != ""){
        painter->setPen(borderPen_);
        QRectF bounding = boundingRect();
        bounding.setRect(bounding.x()-0.5, bounding.y()-0.5,
                         bounding.width()+0.5, bounding.height()+0.5);
        painter->drawRect(bounding);
    }
}

void MapItem::setClaimColor(const QColor &color)
{
    claimPen_.setBrush(color);
    claimColor_ = color;
}

QColor MapItem::getClaimColor()
{
    return claimColor_;
}

void MapItem::drawClaim(QPainter *painter)
{
    if (claimColor_ != "") {
        // Adds marker based on player's color
        claimed_ = true;
        painter->setPen(claimPen_);
        QRectF bounding = boundingRect();
        bounding.setRect(bounding.x()+1,bounding.y()+1,1,1);
        painter->drawRect(bounding);
    }
}

void MapItem::drawTileImage(QPainter* painter)
{
    QPixmap sceneDrawing;

	// This should be done only once when creating the MapItem and
	// again only if the image needs to be refreshed
    if (itemObject_->getType() == "Forest") {
        sceneDrawing = QPixmap(FOREST_IMAGE);
    } else if (itemObject_->getType() == "Grassland") {
        sceneDrawing = QPixmap(GRASSLAND_IMAGE);
    } else if (itemObject_->getType() == "Mountain") {
        sceneDrawing = QPixmap(MOUNTAIN_IMAGE);
    } else if (itemObject_->getType() == "Lake") {
        sceneDrawing = QPixmap(LAKE_IMAGE);
    } else if (itemObject_->getType() == "Ocean") {
        sceneDrawing = QPixmap(OCEAN_IMAGE);
    }

    // Set Farmland if the tile has Farm
    if(std::find(buildings_.begin(),
                 buildings_.end(), "Farm") != buildings_.end()
            && this->getTileObject()->getType() == "Grassland"){
        sceneDrawing = QPixmap(FARMLAND_IMAGE);
    }

    currentItem_ = sceneDrawing;
	painter->drawPixmap(0, 0, size_, size_, sceneDrawing);
}

void MapItem::drawBuildings(QPainter *painter)
{
	// Draw one only
	if(buildings_.size() == 1){
		painter->drawPixmap(0, 0, size_, size_, buildingImages_.at(0));
	}
	// Draw to lower half
	else if(buildings_.size() <= 2){
		for(unsigned int i=0; i<buildings_.size(); i++){
			QPoint location(i*size_/2, size_/2);

			painter->drawPixmap(location.x(), location.y(),
								size_/2, size_/2, buildingImages_.at(i));
		}
	}
	// Draw 2x2 grid
	else {
		for(unsigned int i=0; i<buildings_.size(); i++){
			int offsetY = 1;
			Q_UNUSED(offsetY);
			if(i>1){ offsetY = 0; }

			QPoint location(size_/2 * (i%2), size_/2 * offsetY);

			painter->drawPixmap(location.x(), location.y(),
								size_/2, size_/2, buildingImages_.at(i));
		}
	}
}

void MapItem::drawWorkers(QPainter *painter)
{
    if(workers_.size() == 0){
        return;
    }

    QPixmap workerImage;

    if(workers_.size() == 1){
        workerImage = QPixmap(WORKER_SINGLE_IMAGE);
    } else if(workers_.size() == 2){
        workerImage = QPixmap(WORKER_DOUBLE_IMAGE);
    } else {
        workerImage = QPixmap(WORKER_TRIPLE_IMAGE);
    }

	painter->drawPixmap(size_/2, size_/2, size_/2, size_/2, workerImage);
}
}
