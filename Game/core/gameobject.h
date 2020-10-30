#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "coordinate.h"

namespace Course {

// Forward declarations
class PlayerBase;
class iObjectManager;
class iGameEventHandler;

// Some aliases to make things easier
#ifndef COURSE_OBJECTID
#define COURSE_OBJECTID
/**
 *@brief ObjectId is an alias for unsigned int
 */
using ObjectId = unsigned int;
#endif
/**
 *@brief DescriptionMap is an alias for std::map<std::string, std::string>
 */
using DescriptionMap = std::map<std::string, std::string>;

/**
 * @brief The GameObject class is base-class that contain's general information
 * on different Objects in game.
 *
 * * ID
 * * Possible owner
 * * Possible location coordinate
 * * Metadata in a string->string map
 * * Pointers to GameEventHandler and ObjectManager
 *
 * @note The functions consist mainly of get and set -functions that are used
 * to access and store the information specified above.
 *
 */
class GameObject
{
public:
    /**
     * @brief ID is a constant value that can be used to identify
     * GameObjects through ID numbers.
     */
    const ObjectId ID;

    /**
     * @brief A simple copy-constructor for GameObject
     * @param original is the original GameObject
     */
    GameObject(const GameObject& original);

    /**
     * @brief Constructor that only specifies GameEventHandler and ObjectManager
     * @param eventhandler a shared pointer to Game's GameEventHandler.
     * @param objectmanager a shared pointer to Game's ObjectManager.
     */
    GameObject(const std::shared_ptr<iGameEventHandler>& eventhandler={nullptr},
               const std::shared_ptr<iObjectManager>& objectmanager={nullptr});

    /**
     * @brief GameObject constructor that can specify an owner.
     * @param owner a shared pointer to player that "owns" the object.
     * @param eventhandler a shared pointer to Game's GameEventHandler.
     * @param objectmanager a shared pointer to Game's ObjectManager.
     */
    GameObject(const std::shared_ptr<PlayerBase>& owner,
               const std::shared_ptr<iGameEventHandler>& eventhandler={nullptr},
               const std::shared_ptr<iObjectManager>& objectmanager={nullptr});

    /**
     * @brief GameObject constructor that can specify a coordinate and an owner.
     * @param coordinate a shared pointer to coordinates where the object is
     * located.
     * @param owner a shared pointer to player that "owns" the object.
     * @param eventhandler a shared pointer to Game's GameEventHandler.
     * @param objectmanager a shared pointer to Game's ObjectManager.
     */
    GameObject(const Coordinate& coordinate,
               const std::shared_ptr<PlayerBase>& owner,
               const std::shared_ptr<iGameEventHandler>& eventhandler={nullptr},
               const std::shared_ptr<iObjectManager>& objectmanager={nullptr});

    /**
     * @brief GameObject constructor that can specify a coordinate.
     * @param coordinate a shared pointer to coordinates where the object is
     * located.
     * @param eventhandler a shared pointer to Game's GameEventHandler.
     * @param objectmanager a shared pointer to Game's ObjectManager.
     */
    GameObject(const Coordinate& coordinate,
               const std::shared_ptr<iGameEventHandler>& eventhandler={nullptr},
               const std::shared_ptr<iObjectManager>& objectmanager={nullptr});


    /**
     * @brief ~GameObject Default destructor.
     */
    virtual ~GameObject() = default;

    /**
     * @brief Change GameObject's "owner".
     * @param owner a shared pointer to the new "owner".
     * @post Exception guarantee: No-throw
     */
    virtual void setOwner(const std::shared_ptr<PlayerBase>& owner) final;

    /**
     * @brief Change GameObject's coordinate with a shared pointer to a
     * coordinate.
     * @param coordinate A shared-pointer to the new coordinate.
     * @post Exception guarantee: No-throw
     * @note This creates new Coordinate based on the coordinate.
     * The Coordinate can't be modified from outside of the class.
     * @note Can be used to unset coordinate with null-shared-pointer.
     */
    virtual void setCoordinate(
            const std::shared_ptr<Coordinate>& coordinate) final;

    /**
     * @brief Change GameObject's coordinate.
     * @param coordinate The new coordinate.
     * @post Exception guarantee: No-throw
     */
    virtual void setCoordinate(const Coordinate& coordinate) final;

    /**
     * @brief Removes the coordinate from GameObject.
     * @post Exception guarantee: No-throw
     */
    virtual void unsetCoordinate() final;

    /**
     * @brief Change GameObject's "descriptions"-map.
     * @param desciptions The new "descriptions"-map of strings referring
     * to strings
     * @post Exception guarantee: No-throw
     */
    virtual void setDescriptions(const DescriptionMap& descriptions) final;

    /**
     * @brief Adds a new description to description map.
     * @param key Key for the content
     * @param content Content being stored
     * @post Exception guarantee: Strong
     * @exception
     * KeyError - If the key is already in use in the description-map.
     * @exception See std::map::operator[]
     */
    virtual void addDescription(const std::string& key,
                                const std::string& content) final;
    /**
     * @brief Sets a description for the specified key
     * @param key Key for the content
     * @param content Content being stored
     * @post Exception guarantee: Strong
     * @exceptions  See std::map::operator[]
     */
    virtual void setDescription(const std::string& key,
                                const std::string& content) final;
    /**
     * @brief Returns the content with specified key from the
     * descriptions.
     * @param key Key for the content
     * @post Exception guarantee: Strong
     * @exceptions KeyError - if the description for the key is not found.
     */
    virtual std::string getDescription(const std::string& key) const final;

    /**
     * @brief Removes the content with specified key from the
     * descriptions.
     * @param key Key for the content
     * @post Exception guarantee: Strong
     * @exceptions KeyError - if the description for the key is not found.
     */
    virtual void removeDescription(const std::string& key) final;

    /**
     * @brief Removes all content from descriptions.
     * @post Exception guarantee: No-throw
     */
    virtual void removeDescriptions() final;

    /**
     * @brief Returns GameObject's owner.
     * @return A shared-pointer to GameObject's owner
     * @post Exception guarantee: No-throw
     */
    virtual std::shared_ptr<PlayerBase> getOwner() const final;

    /**
     * @brief Returns a pointer to a copy of GameObject's coordinate.
     * @return Shared-pointer copy of the GameObject's coordinate,
     * if the GameObject has a coordinate.
     * Null-shared-pointer if the GameObject has no coordinate.
     * @post Exception guarantee: Strong
     * @exception See std::make_shared
     * @note To change GameObject's coordinate you must use setCoordinate.
     * This prevent unwanted alterations by accident.
     */
    virtual std::shared_ptr<Coordinate> getCoordinatePtr() const final;

    /**
     * @brief Returns GameObject's current coordinate.
     * @post Exception guaranee: Strong
     * @exception
     * InvalidPointer - If the GameObject doesn't have a coordinate.
     */
    virtual Coordinate getCoordinate() const final;

    /**
     * @brief Returns the map of descriptions in GameObject.
     * @return std::map of strings referring to strings.
     * @post Exception guarantee: No-throw
     */
    virtual std::map<std::string, std::string> getDescriptions() const final;

    /**
     * @brief getType Returns a string describing objects type.
     * This should be overriden in each inherited class.
     * Makes checking object's type easier for students.
     * @return std::string that represents Object's type.
     * @post Exception guarantee: No-throw
     * @note You can use this in e.g. debugging and similar printing.
     */
    virtual std::string getType() const;

    /**
     * @brief Function to compare if objects have same owner.
     * * @param other The other GameObject
     * @return True - If owners match or both are null
     * False - If owners don't match
     * @post Excepetion guarantee: Strong
     * @exception ExpiredPointer - If any owner weak_ptr has expired.
     */
    virtual bool hasSameOwnerAs(
            const std::shared_ptr<GameObject>& other) const final;

    /**
     * @brief has_same_coordinate
     * @param other The other GameObject
     * @return \n
     * True - If coordinates match or both are null \n
     * False - If the coordinates don't match \n
     * @post Exception guarantee: Strong
     */
    virtual bool hasSameCoordinateAs(
            const std::shared_ptr<GameObject>& other) const final;

protected:

    /**
     * @brief This is the primary method for locking GameEventHandler inside
     * different GameObject-classes.
     * @return shared pointer to the GameEventHandler
     * @post Exception guarantee: No-throw
     */
    virtual std::shared_ptr<iGameEventHandler> lockEventHandler() const final;
    /**
     * @brief This is the primary method for locking ObjectManager inside
     * different GameObject classes.
     * @return shared pointer to the ObjectManager
     * @post Exception guarantee: No-throw
     */
    virtual std::shared_ptr<iObjectManager> lockObjectManager() const final;

private:
    const std::weak_ptr<iGameEventHandler> EVENTHANDLER;
    const std::weak_ptr<iObjectManager> OBJECTMANAGER;

    std::weak_ptr<PlayerBase> m_owner;
    std::unique_ptr<Coordinate> m_coordinate;
    std::map<std::string, std::string> m_descriptions;

    static ObjectId c_next_id;
};

}
#endif // GAMEOBJECT_H
