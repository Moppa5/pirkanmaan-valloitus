#include "tilebase.h"

#include <QtGlobal> // For Q_ASSERT
#include <QDebug>

#include "exceptions/notenoughspace.h"
#include "exceptions/ownerconflict.h"
#include "exceptions/invalidpointer.h"
#include "core/playerbase.h"


namespace Course {

TileBase::TileBase(const Coordinate& location,
                   const std::shared_ptr<iGameEventHandler> &eventhandler,
                   const std::shared_ptr<iObjectManager>& objectmanager,
                   const unsigned int& max_build,
                   const unsigned int& max_work,
                   const ResourceMap& production):
    GameObject(location, eventhandler, objectmanager),
    MAX_BUILDINGS(max_build),
    MAX_WORKERS(max_work),
    BASE_PRODUCTION(production)
{
}

std::string TileBase::getType() const
{
    return "TileBase";
}

void TileBase::addBuilding(const std::shared_ptr<BuildingBase>& building)
{
    std::shared_ptr<TileBase> tile;
    tile = lockObjectManager()->getTile(ID);
    if (not tile)
    {
        throw InvalidPointer("Objectmanager didn't find Tile: " +
                             std::to_string(ID));
    }
    if (not building->canBePlacedOnTile(tile))
    {
        throw IllegalAction("Can't place Building: " +
                            std::to_string(building->ID) + " on Tile: " +
                            std::to_string(ID));
    }

    if (getBuildingCount() + building->spacesInTileCapacity() > MAX_BUILDINGS)
    {
        throw NotEnoughSpace("Tile has no more room for Buildings!");
    }
    building->setLocationTile(tile);
    m_buildings.push_back(building);
}

void TileBase::removeBuilding(const std::shared_ptr<BuildingBase>& building)
{
    for( auto it = m_buildings.begin(); it != m_buildings.end(); ++it)
    {
        if(it->lock() == building)
        {
            m_buildings.erase(it);
            building->setLocationTile(nullptr);
            return;
        }
    }
    qDebug() << "Tile " << QString::number(ID) << ": Doesn't have building "
             << QString::number(building->ID);
}

void TileBase::addWorker(const std::shared_ptr<WorkerBase>& worker)
{
    std::shared_ptr<TileBase> tile;
    tile = lockObjectManager()->getTile(ID);
    if (not tile)
    {
        throw InvalidPointer("Objectmanager didn't find Tile: " +
                             std::to_string(ID));
    }
    if (not worker->canBePlacedOnTile(tile))
    {
        throw IllegalAction("Can't place Worker: " + std::to_string(worker->ID)
                            + " on Tile: " + std::to_string(ID));
    }

    if (getWorkerCount() + worker->spacesInTileCapacity() > MAX_WORKERS)
    {
        throw NotEnoughSpace("Tile: " + std::to_string(ID) +
                             " has no more room for Workers!");
    }
    worker->setLocationTile(tile);
    m_workers.push_back(worker);
}

void TileBase::removeWorker(const std::shared_ptr<WorkerBase>& worker)
{
    for(auto it = m_workers.begin(); it != m_workers.end(); ++it)
    {
        std::shared_ptr<WorkerBase> locked = it->lock();
        if(locked == worker)
        {
            m_workers.erase(it);
            worker->setLocationTile(nullptr);
            return;
        }
    }
    qDebug() << "Tile " << QString::number(ID) << ": Doesn't have worker "
             << QString::number(worker->ID);
}

bool TileBase::generateResources()
{
    ResourceMapDouble worker_efficiency = {
        {NONE, 0},
        {MONEY, 0},
        {FOOD, 0},
        {WOOD, 0},
        {STONE, 0},
        {ORE, 0},
    };
    ResourceMap total_production;

    for( auto work_it = m_workers.begin();
         work_it != m_workers.end();
         ++work_it)
    {
        ResourceMapDouble current_efficiency = work_it->lock()->tileWorkAction();

        worker_efficiency = mergeResourceMapDoubles(worker_efficiency, current_efficiency);
    }

    total_production = multiplyResourceMap(BASE_PRODUCTION, worker_efficiency);

    for( auto build_it = m_buildings.begin();
         build_it != m_buildings.end();
         ++build_it)
    {
        ResourceMap current_production = build_it->lock()->getProduction();

        total_production = mergeResourceMaps(total_production,
                                             current_production);
    }

    return lockEventHandler()->modifyResources(getOwner(), total_production);
}


unsigned int TileBase::getBuildingCount() const
{
    unsigned taken = 0;
    for (const auto& bldn : m_buildings)
    {
        if (not bldn.expired())
        {
            taken += bldn.lock()->spacesInTileCapacity();
        }
    }
    return taken;
}

unsigned int TileBase::getWorkerCount() const
{
    unsigned taken = 0;
    for (const auto& wrkr : m_workers)
    {
        if (not wrkr.expired())
        {
            taken += wrkr.lock()->spacesInTileCapacity();
        }
    }
    return taken;
}


bool TileBase::hasSpaceForWorkers(int amount) const
{
    return amount + getWorkerCount() <= MAX_WORKERS;
}

bool TileBase::hasSpaceForBuildings(int amount) const
{
    return amount + getBuildingCount() <= MAX_BUILDINGS;
}

std::vector< std::shared_ptr<WorkerBase> > TileBase::getWorkers() const
{
    std::vector< std::shared_ptr<WorkerBase> > locked_workers;
    for( auto it = m_workers.begin(); it != m_workers.end(); ++it)
    {
        std::shared_ptr<WorkerBase> locked = it->lock();
        if(locked)
        {
            locked_workers.push_back(locked);
        }
        else
        {
            qDebug() << "Tile " << QString::number(ID) <<
                        ": Has an invalid weak_ptr to a Worker.";
        }
    }

    return locked_workers;
}

std::vector<std::shared_ptr<BuildingBase> > TileBase::getBuildings() const
{
    std::vector< std::shared_ptr<BuildingBase> > locked_buildings;
    for( auto it = m_buildings.begin(); it != m_buildings.end(); ++it)
    {
        std::shared_ptr<BuildingBase> locked = it->lock();
        if(locked)
        {
            locked_buildings.push_back(locked);
        }
        else
        {
            qDebug() << "Tile " << QString::number(ID) <<
                        ": Has an invalid weak_ptr to a Building.";
        }
    }

    return locked_buildings;
}

} // namespace Course
