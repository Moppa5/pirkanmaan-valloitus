#ifndef WORKERBASE_H
#define WORKERBASE_H

#include "core/placeablegameobject.h"
#include "core/basicresources.h"


namespace Course {

/**
 * @brief The WorkerBase class is an abstract base-class for Worker-objects.
 *
 * * Workers can perform actions
 * * Tiles request production-multipliers from Workers
 */
class WorkerBase : public PlaceableGameObject
{
public:

    const ResourceMapDouble WORKER_EFFICIENCY;
    const ResourceMap RECRUITMENT_COST;

    /**
     * @brief Disabled parameterless constructor.
     */
    WorkerBase() = delete;

    WorkerBase(const std::shared_ptr<iGameEventHandler>& eventhandler,
               const std::shared_ptr<iObjectManager>& objectmanager,
               const std::shared_ptr<PlayerBase>& owner,
               const int& tilespaces = 1,
               const ResourceMap& cost = {},
               const ResourceMapDouble& efficiency = {});

    /**
     * @brief Default destructor.
     */
    virtual ~WorkerBase() = default;

    /**
     * @brief Performs worker's action when working a Tile for resources.
     * @return Returns the final working efficiency of a worker.
     * @note This is called by a Tile when it generates resources.
     */
    virtual const ResourceMapDouble tileWorkAction();

    /**
     * @brief Performs the Worker's special action. (If any)
     *
     * @note Hint: You can use game-eventhandler for more creative solutions.
     */
    virtual void doSpecialAction() = 0;

    /**
     * @brief Sets new resourcetype for the worker to focus on.
     * @param new_focus the new resource focus.
     * @post Exception guarantee: No-throw
     */
    virtual void setResourceFocus(BasicResource new_focus) final;

    /**
     * @brief Returns the currently focused resourcetype.
     * @post Exception guarantee: No-throw
     */
    virtual BasicResource getResourceFocus() const final;
    
    /**
     * @brief Default placement rule for workers:\n
     * * Tile must have space for the worker
     * * Tile must have same owner as the worker
     * @param target is the Tile that worker is being placed on.
     * @return
     * True - Only if both conditions are met.
     * @post Exception guarantee: Basic
     * @note
     * Override to change placement rules for derived worker.
     */
    virtual bool canBePlacedOnTile(
            const std::shared_ptr<TileBase> &target) const override;

    /**
     * @copydoc GameObject::getType()
     */
    virtual std::string getType() const override;

private:
    BasicResource m_resource_focus;


}; // class WorkerBase

} // namespace Course


#endif // WORKERBASE_H
