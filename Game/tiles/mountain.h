#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "tiles/tilebase.h"
#include "constants/resourcemaps2.h"


namespace Game {

/**
 * @brief The Mountain class is Course TileBase based and implements Mountain
 * tile for the game.
 */
class Mountain : public Course::TileBase
{
public:
    Mountain() = delete;

    Mountain(const Course::Coordinate& location,
           const std::shared_ptr<Course::iGameEventHandler>& eventhandler,
           const std::shared_ptr<Course::iObjectManager>& objectmanager,
           const unsigned int& max_build = 2,
           const unsigned int& max_work = 3,
             const Course::ResourceMap& production = MOUNTAIN_BP);

    virtual ~Mountain() = default;

    virtual std::string getType() const override;
 };

}

#endif // MOUNTAIN_H
