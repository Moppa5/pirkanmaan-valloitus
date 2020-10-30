#ifndef LAKE_H
#define LAKE_H

#include <tiles/tilebase.h>
#include <constants/resourcemaps2.h>

namespace Game {

/**
 * @brief The Lake class is based on Course TileBase and implements game's
 * lake tile.
 */
class Lake : public Course::TileBase
{
public:
    Lake() = delete;

    Lake(const Course::Coordinate& location,
           const std::shared_ptr<Course::iGameEventHandler>& eventhandler,
           const std::shared_ptr<Course::iObjectManager>& objectmanager,
           const unsigned int& max_build = 2,
           const unsigned int& max_work = 3,
             const Course::ResourceMap& production = LAKE_BP);

    virtual ~Lake() = default;

    virtual std::string getType() const override;
};

}
#endif // LAKE_H
