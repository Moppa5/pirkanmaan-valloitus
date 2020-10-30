#ifndef PLAYERBASE_H
#define PLAYERBASE_H

#include <string>
#include <vector>
#include <memory>

#include "gameobject.h"

namespace Course {

/**
 * @class PlayerBase
 * @brief The PlayerBase class is a base class for classes used to describe
 * a player in game.
 *
 * The class can be used to store and access GameObjects.
 * Expired weak pointers are automatically removed when requesting or removing
 * objects.
 *
 * @note Objects are stored as weak pointers.
 */
class PlayerBase
{
public:
    /**
     * @brief Constructor for the class
     * @param name A std::string for player's name
     * @param objects (optional) A std::vector of shared-pointers to
     * GameObjects.
     */
    PlayerBase(const std::string& name,
               const std::vector<std::shared_ptr<GameObject> > objects ={});

    /**
     * @brief Default destructor
     */
    virtual ~PlayerBase() = default;

    /**
     * @brief Sets a new m_name value for the class.
     * @param new_name The new name.
     * @post Exception guarantee: No-throw
     */
    virtual void setName(const std::string& new_name) final;

    //
    // Add objects
    //

    /**
     * @brief Stores a weak GameObject-pointer.
     * @param object Is a weak pointer to the stored GameObject
     * @post Exception guarantee: Strong
     * @exception See std::vector::push_back()
     */
    virtual void addObject(std::shared_ptr<GameObject> object) final;

    /**
     * @brief Stores a vector of weak GameObject-pointers.
     * @param objects Is an std::vector of weak GameObject-pointers.
     * @post Exception guarantee: Strong
     * @exception See std::vector::insert()
     */
    virtual void addObjects(
            const std::vector<std::shared_ptr<GameObject> > &objects) final;

    //
    // Remove objects
    //

    /**
     * @brief Removes a weak GameObject-pointer and expired weak pointers
     * @param object a weak pointer to GameObject
     * @post Exception guarantee: Basic
     * @exception ExpiredPointer - object is expired
     * @exception KeyError - No objects match the searched object
     */
    virtual void removeObject(const std::shared_ptr<GameObject>& object) final;

    /**
     * @brief Removes a list of weak GameObject-pointers and
     * expired weak pointers.
     * @param objects A vector of weak GameObject-pointers
     * @post Exception guarantee: No-throw
     * @note If some of the provided weak pointers are expired,
     * no exceptions are thrown.
     */
    virtual void removeObjects(
            const std::vector<std::shared_ptr<GameObject> >& objects) final;

    /**
     * @brief Removes a weak GameObject-pointers based on a ObjectId
     * and removes expired weak pointers.
     * @param id An ObjectId (unsigned int) for GameObject which is removed
     * @post Exception guarantee: Basic
     * @exception KeyError - No GameObjects with given ID were found.
     * @exception See std::remove_if
     */
    virtual void removeObject(const ObjectId& id) final;

    /**
     * @brief Removes a list of weak GameObject-pointers based on a ObjectId
     * and removes expired weak pointers.
     * @param objects A vector of ObjectId (unsigned int) for GameObjects
     *  that are removed
     * @post Exception guarantee: No-throw
     * @note Even if some of the provided ID's are not found,
     * no exceptions are thrown.
     */
    virtual void removeObjects(const std::vector<ObjectId>& objects) final;

    //
    // Getters
    //

    /**
     * @brief Returns the vector of weak GameObject-pointers
     *  that are currently stored in the Player-object.
     * @return Copy of m_objects -vector
     * @post Exception guarantee: Strong
     */
    virtual std::vector<std::shared_ptr<GameObject> > getObjects() const final;

    /**
     * @brief Returns the Player-object's name
     * @return Copy of current string in m_name
     * @post Exception guarantee: No-throw
     */
    virtual std::string getName() const final;

private:
    std::string m_name;
    std::vector<std::weak_ptr<GameObject> > m_objects;

};

}
#endif // PLAYERBASE_H
