#ifndef MINER_HH
#define MINER_HH

#include "workers/workerbase.h"
#include "core/resourcemaps.h"
#include "constants/resourcemaps2.h"
#include "interfaces/gameeventhandler.hh"
#include "interfaces/objectmanager.hh"


namespace Game {

/**
 * @brief The Miner class is Course WorkerBase based worker that
 * produces stone, money and ore in the game
 */
class Miner : public Course::WorkerBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Miner() = delete;

    /**
     * @brief Constructor for the class.
     *
     * @param eventhandler points to the student's GameEventHandler.
     * @param owner points to the owning player.
     * @param descriptions contains descriptions and flavor texts.
     */
    Miner(const std::shared_ptr<GameEventHandler>& eventhandler,
                const std::shared_ptr<ObjectManager>& objectmanager,
                const std::shared_ptr<PlayerBase>& owner,
                const int& tilespaces = 1,
                const ResourceMap& cost =
                    MINER_RECRUITMENT_COST,
                const ResourceMapDouble& efficiency =
                   MINER_EFFICIENCY
                );

    /**
     * @brief Default destructor.
     */
    virtual ~Miner() = default;

    /**
     * @copydoc GameObject::getType()
     */
    virtual std::string getType() const override;

    /**
     * @brief Check if the worker can be placed on the Tile according to
     * it's placement rule. Only rule is that the Tile must have same owner
     * as the worker.
     * @param target is the Tile that worker is being placed on.
     * @return
     * True - If baseclass' method return true and target Tile has space
     * for worker.
     * False - If both conditions aren't met.
     * @note Override to change placement rules for derived worker.
     * @post Exception guarantee: Basic
     */
    virtual bool canBePlacedOnTile(
            const std::shared_ptr<TileBase> &target) const override;

    /**
     * @brief Performs the Worker's default action.
     */
    virtual void doSpecialAction() override;

    /**
     * @brief Returns Worker's efficiency at resource production.
     * Worker consumes FOOD and MONEY. Resource consumption and resource
     * focus determine final multiplier that is based on WORKER_EFFICIENCY.
     *
     * @return
     */
    virtual const ResourceMapDouble tileWorkAction() override;

private:

};

}

#endif // MINER_HH
