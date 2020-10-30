#include "playerbase.h"

#include <algorithm>
#include "../exceptions/keyerror.h"

namespace Course{


PlayerBase::PlayerBase(const std::string& name,
                       const std::vector<std::shared_ptr<GameObject> > objects):
    m_name(name),
    m_objects()
{
    for( auto it = objects.begin(); it != objects.end(); ++it)
    {
        m_objects.push_back(std::weak_ptr<GameObject>(*it));
    }
}

void PlayerBase::setName(const std::string& new_name)
{
    m_name = new_name;
}


void PlayerBase::addObject(std::shared_ptr<GameObject> object)
{
    m_objects.push_back(std::weak_ptr<GameObject>(object));
}


void PlayerBase::addObjects(
        const std::vector<std::shared_ptr<GameObject> >& objects)
{
    m_objects.insert(m_objects.end(), objects.begin(), objects.end());
}


void PlayerBase::removeObject(const std::shared_ptr<GameObject>& object)
{
    if( not object )
    {
        removeObject(std::numeric_limits<ObjectId>::max());
    }
    removeObject(object->ID);
}


void PlayerBase::removeObject(const ObjectId& id)
{
    bool found = false;
    // Use find if to do weak_ptr locking inside seach-function
    //  for ID-recognition.
    auto it = std::remove_if(m_objects.begin(), m_objects.end(),
                           [id, &found](std::weak_ptr<GameObject>& x){
            auto locked = x.lock();
            if( not locked )
            {
                return true;
            }
            if( locked->ID == id )
            {
                found = true;
                return true;
            }
            return false;
            });

    m_objects.erase(it, m_objects.end());

    if(not found)
    {
        throw KeyError("Object not found.");
    }
}


void PlayerBase::removeObjects(
        const std::vector<std::shared_ptr<GameObject> >& objects)
{
    for( auto it = objects.begin(); it != objects.end(); ++it)
    {
        try{
            removeObject(*it);
        }
        catch (const KeyError&){
            continue;
        }
    }
}


void PlayerBase::removeObjects(const std::vector<ObjectId>& objects)
{
    for( auto it = objects.begin(); it != objects.end(); ++it)
    {
        try{
            removeObject(*it);
        }
        catch(const KeyError&){
            continue;
        }
    }
}


std::vector<std::shared_ptr<GameObject> > PlayerBase::getObjects() const
{
    std::vector<std::shared_ptr<GameObject> > objects;
    for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
    {
        if(not it->expired())
        {
            objects.push_back(it->lock());
        }
    }

    return objects;
}

std::string PlayerBase::getName() const
{
    return m_name;
}



} // namespace BoardGameBase
