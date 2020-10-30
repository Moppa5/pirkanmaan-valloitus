#include "player.hh"

namespace Game {

Player::Player(const std::string &name,
               const std::vector<std::shared_ptr<Course::GameObject> > objects)
    : Course::PlayerBase(name, objects)
{
    resources_ = std::make_shared<Course::ResourceMap>();
}

std::shared_ptr<Course::ResourceMap> Player::getResourceMap()
{
    if(resources_ == nullptr){
        throw Course::InvalidPointer("Player has no resourcemap");
    }
    return resources_;
}

void Player::setResourceMap(const Course::ResourceMap &map)
{
    *resources_ = map;
}

void Player::setResource(const Course::BasicResource &resource, int count)
{
    (*resources_)[resource] += count;
}

QColor Player::getColor()
{
    return color_;
}

void Player::setColor(const QColor &color)
{
    color_ = color;
}

}
