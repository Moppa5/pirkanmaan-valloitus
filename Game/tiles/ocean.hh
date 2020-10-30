#ifndef OCEAN_HH
#define OCEAN_HH

#include <tiles/tilebase.h>
#include "constants/resourcemaps2.h"

namespace Game {

/**
 * @brief The Ocean class is Course TileBase based and implements Ocean
 * tile for the game
 */
class Ocean : public Course::TileBase
{
public:
    Ocean() = delete;

    Ocean(const Course::Coordinate& location,
           const std::shared_ptr<Course::iGameEventHandler>& eventhandler,
           const std::shared_ptr<Course::iObjectManager>& objectmanager,
           const unsigned int& max_build = 4,
           const unsigned int& max_work = 0,
             const Course::ResourceMap& production = OCEAN_BP);

    virtual ~Ocean() = default;

    virtual std::string getType() const override;
};

}

#endif // OCEAN_HH
