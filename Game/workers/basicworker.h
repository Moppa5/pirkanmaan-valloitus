#ifndef BASICWORKER_H
#define BASICWORKER_H

#include "workerbase.h"
#include "core/resourcemaps.h"


namespace Course {

/**
 * @brief The BasicWorker class represents a "normal worker" in the game.
 *
 * Worker has following production-efficiency: \n
 * * Money - 0.25 \n
 * * Food - 1.00 \n
 * * Wood - 0.75 \n
 * * Stone - 0.50 \n
 * * Ore - 0.50 \n
 *
 * BasicWorkers consume Food and money. \n
 * * 1 Food - Or BasicWorker refuses to work. \n
 * * 1 Money - Or BasicWorker works at 50% efficiency. \n
 * * Resourcefocus adds 25% efficiency for the focused resource,
 * even if the worker is refusing work.
 */
class BasicWorker : public WorkerBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    BasicWorker() = delete;

    /**
     * @brief Constructor for the class.
     *
     * @param eventhandler points to the student's GameEventHandler.
     * @param owner points to the owning player.
     * @param descriptions contains descriptions and flavor texts.
     */
    BasicWorker(const std::shared_ptr<iGameEventHandler>& eventhandler,
                const std::shared_ptr<iObjectManager>& objectmanager,
                const std::shared_ptr<PlayerBase>& owner,
                const int& tilespaces = 1,
                const ResourceMap& cost =
                    ConstResourceMaps::BW_RECRUITMENT_COST,
                const ResourceMapDouble& efficiency =
                    ConstResourceMaps::BW_WORKER_EFFICIENCY
                );

    /**
     * @brief Default destructor.
     */
    virtual ~BasicWorker() = default;

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

}; // class BasicWorker

} // namespace Course


#endif // BASICWORKER_H
