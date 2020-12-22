#include "objectmanager.hh"
#include "iostream"

namespace Game {

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

std::vector<std::shared_ptr<Course::TileBase> > ObjectManager::getTiles(
        const std::vector<Course::Coordinate> &coordinates)
{
    std::vector<std::shared_ptr<Course::TileBase>> tiles;

    for(std::shared_ptr<Course::TileBase> tile : tiles_){
        for(auto coordinate : coordinates){
            if(tile->getCoordinate() == coordinate){
                tiles.push_back(tile);
            }
        }
    }

    return tiles;
}

std::vector<std::shared_ptr<Course::TileBase> > ObjectManager::getTiles()
{
    return tiles_;
}

std::shared_ptr<Course::TileBase> ObjectManager::getTile(
        const Course::ObjectId &id)
{
    for(std::shared_ptr<Course::TileBase> tile : tiles_){
        if(tile->ID == id){
            return tile;
        }
    }
    return nullptr;
}

std::shared_ptr<Course::TileBase> ObjectManager::getTile(
        const Course::Coordinate &coordinate)
{
    for(std::shared_ptr<Course::TileBase> tile : tiles_){
        if(tile->getCoordinate() == coordinate){
            return tile;
        }
    }

    return nullptr;
}

void ObjectManager::addTiles(const std::vector
                             <std::shared_ptr<Course::TileBase> > &tiles)
{
    // Loop over tiles
    for(auto tile : tiles){
        tiles_.push_back(tile);
    }
}

void ObjectManager::addBuilding(const std::shared_ptr
                                <Course::BuildingBase> &building)
{
    buildings_.push_back(building);
}

void ObjectManager::removeBuilding(const std::shared_ptr
                                   <Course::BuildingBase> &building)
{
    std::shared_ptr<Course::TileBase> tile = getTile(building->getCoordinate());
    tile->removeBuilding(building);

    for(unsigned int i=0; i<buildings_.size(); i++){
        if(buildings_.at(i) == building) {
            buildings_.erase(buildings_.begin() + i);
            return;
        }
    }

    throw Course::KeyError("Building not found");
}

void ObjectManager::addWorker(const std::shared_ptr
                              <Course::WorkerBase> &worker)
{
    workers_.push_back(worker);
}

void ObjectManager::removeWorker(const std::shared_ptr
                                 <Course::WorkerBase> &worker)
{
    std::shared_ptr<Course::TileBase> tile = getTile(worker->getCoordinate());

    for(unsigned int i=0; i<workers_.size(); i++){
        if(workers_.at(i) == worker) {
            tile->removeWorker(worker);
            workers_.erase(workers_.begin() + i);
            return;
        }
    }

    throw Course::KeyError("Worker not found");
}

}
