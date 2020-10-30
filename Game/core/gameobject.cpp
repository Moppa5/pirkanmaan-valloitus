#include "gameobject.h"
#include "interfaces/igameeventhandler.h"
#include "interfaces/iobjectmanager.h"
#include "exceptions/keyerror.h"
#include "exceptions/invalidpointer.h"
#include "playerbase.h"

#include <QDebug>


#include <algorithm>

namespace Course {

// Private static variables must be initialized this way.
unsigned int GameObject::c_next_id = 0;

GameObject::GameObject(const GameObject &original):
    ID(GameObject::c_next_id),
    EVENTHANDLER(original.EVENTHANDLER),
    OBJECTMANAGER(original.OBJECTMANAGER)
{
    m_owner = original.m_owner;
    if(original.m_coordinate)
    {
        m_coordinate = std::make_unique<Coordinate>(*original.m_coordinate);
    }
    else
    {
        m_coordinate = nullptr;
    }
    m_descriptions = original.m_descriptions;
    ++c_next_id;
}

GameObject::GameObject(const std::shared_ptr<iGameEventHandler>& eventhandler,
                       const std::shared_ptr<iObjectManager>& objectmanager):
    ID(GameObject::c_next_id),
    EVENTHANDLER(eventhandler),
    OBJECTMANAGER(objectmanager),
    m_owner(std::shared_ptr<PlayerBase>(nullptr)),
    m_coordinate(),
    m_descriptions({})
{
    ++c_next_id;
}

GameObject::GameObject(const std::shared_ptr<PlayerBase>& owner,
                       const std::shared_ptr<iGameEventHandler>& eventhandler,
                       const std::shared_ptr<iObjectManager>& objectmanager):
    ID(GameObject::c_next_id),
    EVENTHANDLER(eventhandler),
    OBJECTMANAGER(objectmanager),
    m_owner(owner),
    m_coordinate(),
    m_descriptions({})
{
    ++c_next_id;
}

GameObject::GameObject(const Coordinate& coordinate,
                       const std::shared_ptr<PlayerBase>& owner,
                       const std::shared_ptr<iGameEventHandler>& eventhandler,
                       const std::shared_ptr<iObjectManager>& objectmanager):
    ID(GameObject::c_next_id),
    EVENTHANDLER(eventhandler),
    OBJECTMANAGER(objectmanager),
    m_owner(owner),
    m_coordinate(),
    m_descriptions({})
{
    m_coordinate = std::make_unique<Coordinate>(coordinate);
    ++c_next_id;
}

GameObject::GameObject(const Coordinate& coordinate,
                       const std::shared_ptr<iGameEventHandler>& eventhandler,
                       const std::shared_ptr<iObjectManager>& objectmanager):
    ID(GameObject::c_next_id),
    EVENTHANDLER(eventhandler),
    OBJECTMANAGER(objectmanager),
    m_owner(std::shared_ptr<PlayerBase>(nullptr)),
    m_coordinate(),
    m_descriptions({})
{
    m_coordinate = std::make_unique<Coordinate>(coordinate);
    ++c_next_id;
}


void GameObject::setOwner(const std::shared_ptr<PlayerBase>& owner)
{
    m_owner = std::weak_ptr<PlayerBase>(owner);
}

void GameObject::setCoordinate(const std::shared_ptr<Coordinate>& coordinate)
{
    m_coordinate = std::make_unique<Coordinate>(*coordinate);
}

void GameObject::setCoordinate(const Coordinate& coordinate)
{
    m_coordinate = std::make_unique<Coordinate>(coordinate);
}

void GameObject::unsetCoordinate()
{
    m_coordinate = std::unique_ptr<Coordinate>(nullptr);
}

void GameObject::setDescriptions(const DescriptionMap& descriptions)
{
    m_descriptions = descriptions;
}



void GameObject::addDescription(const std::string& key,
                                const std::string& content)
{
    if( m_descriptions.find(key) != m_descriptions.end() )
    {
        throw KeyError("Key already in use.");
    }
    m_descriptions[key] = content;
}

void GameObject::setDescription(const std::string& key,
                                const std::string& content)
{
    m_descriptions[key] = content;
}

void GameObject::removeDescription(const std::string& key)
{
    auto removed = m_descriptions.erase(key);
    if(removed == 0)
    {
        throw KeyError("Nothing found with key: " + key);
    }
}

void GameObject::removeDescriptions()
{
    m_descriptions.clear();
}

std::shared_ptr<PlayerBase> GameObject::getOwner() const
{
    return m_owner.lock();
}

std::shared_ptr<Coordinate> GameObject::getCoordinatePtr() const
{
    if(m_coordinate)
    {
        return std::make_shared<Coordinate>(*m_coordinate);
    }
    return nullptr;
}

Coordinate GameObject::getCoordinate() const
{
    if( not m_coordinate )
    {
        throw InvalidPointer("GameObject has no Coordinate.");
    }
    return Coordinate(*m_coordinate);
}

std::string GameObject::getDescription(const std::string& key) const
{
    auto content = m_descriptions.find(key);
    if(content == m_descriptions.end())
    {
        throw KeyError("Key not found.");
    }
    return (*content).second;
}

std::map<std::string, std::string> GameObject::getDescriptions() const
{
    return m_descriptions;
}

std::string GameObject::getType() const
{
    return "GameObject";
}

bool GameObject::hasSameOwnerAs(
        const std::shared_ptr<GameObject> &other) const
{
    return (getOwner().get() == other->getOwner().get());
}


bool GameObject::hasSameCoordinateAs(
        const std::shared_ptr<GameObject> &other) const
{
    if(not other)
    {
        return false;
    }
    if(not m_coordinate)
    {
        if(not other->getCoordinatePtr())
        {
            return true;
        }
        return false;
    }

    return m_coordinate->operator ==(*(other->getCoordinatePtr().get()));
}

std::shared_ptr<iGameEventHandler> GameObject::lockEventHandler() const
{
    std::shared_ptr<iGameEventHandler> handler = EVENTHANDLER.lock();
    Q_ASSERT(handler);
    return handler;
}

std::shared_ptr<iObjectManager> GameObject::lockObjectManager() const
{
    std::shared_ptr<iObjectManager> handler = OBJECTMANAGER.lock();
    Q_ASSERT(handler);
    return handler;
}


}

