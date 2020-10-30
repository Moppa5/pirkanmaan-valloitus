#include "gamescene.hh"
#include <iostream>
namespace Game {

GameScene::GameScene()
{
}

void GameScene::resize()
{
    if ( mapBoundRect_ != nullptr ){
        QGraphicsScene::removeItem(mapBoundRect_);
    }

    // Calculates rect with middle at (0,0).
    // Basically left upper corner coords and then width and height
    QRect rect = QRect( mapWidth_ * mapScale_, mapHeight_ * mapScale_,
                        mapWidth_ * mapScale_ - 1, mapHeight_ * mapScale_ - 1 );

    addRect(rect, QPen(Qt::black));
    setSceneRect(rect);
    mapBoundRect_ = itemAt(rect.topLeft(), QTransform());
    // Draw on the bottom of all items
    mapBoundRect_->setZValue(-1);
}

void GameScene::drawItem(const std::shared_ptr<Course::GameObject> &obj)
{
    Game::MapItem* nItem = new Game::MapItem(
                obj, mapScale_);

    // Fixes something that prevents clicking all tiles
    QPixmap pix(mapWidth_*mapScale_,mapHeight_*mapHeight_);
    nItem->setPixmap(pix);
    addItem(nItem);
}

void GameScene::addClaim(MapItem* obj,const QColor &claimColor)
{
    obj->setClaimColor(claimColor);
}

void GameScene::removeClaim(MapItem* obj)
{
    obj->setClaimColor("");
}

void GameScene::addBorder(MapItem* obj, const QColor &borderColor,
                          const bool highlight)
{
    if(highlight){
        obj->addHighlight();
    } else{
        obj->addBorder(borderColor);
    }
}

void GameScene::removeBorder(MapItem* obj, const bool highlight)
{
    if(highlight){
        obj->removeHighlight();
    }
    else{
        obj->removeBorder();
    }
}

bool GameScene::event(QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMousePress){
        QGraphicsSceneMouseEvent* mouseEvent =
                dynamic_cast<QGraphicsSceneMouseEvent*>(event);

        if (sceneRect().contains(mouseEvent->scenePos())) {
            QPointF point = mouseEvent->scenePos() / mapScale_;
            qDebug() << point;
            point.rx() = floor(point.rx());
            point.ry() = floor(point.ry());

            QGraphicsItem* pressed = itemAt(point * mapScale_, QTransform());

            if ( pressed == mapBoundRect_ ){
                // Does nothing because area bound
                qDebug() << "BoundingRect";
                return false;

            } else{
                Game::MapItem* itemObject = static_cast<Game::MapItem*>(pressed);
                std::shared_ptr<Course::GameObject> item = itemObject->getTileObject();
                std::string owner = "";

                if (item->getOwner() != nullptr) {
                    owner = item->getOwner()->getName();
                }
                emit tileInfo(QString::fromStdString(item->getType()),
                              itemObject,itemObject->getItem(),owner);
            }
        }

    }
    return true;
}

void GameScene::refreshScene(const std::vector<std::shared_ptr<Player>> &players)
{
    QList<QGraphicsItem*> itemList = items();

        for (auto item: itemList) {
            MapItem* trueItem = static_cast<MapItem*>(item);

            if (trueItem->getTileObject()->getOwner() != nullptr) {
                // Find owner color
                std::shared_ptr<Player> player;
                std::string ownerName = trueItem->getTileObject()->getOwner()->getName();

                for(std::shared_ptr<Player> p : players){
                    if(p->getName() == ownerName){
                        player = p;
                    }
                }
                addClaim(trueItem,player->getColor());
                trueItem->update(trueItem->boundingRect());
            }
    }
}

}
