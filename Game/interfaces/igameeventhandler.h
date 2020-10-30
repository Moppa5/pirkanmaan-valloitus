#ifndef IGAMEEVENTHANDLER_H
#define IGAMEEVENTHANDLER_H

#include <memory>
#include <vector>

#include "core/basicresources.h"

namespace Course {
class TileBase;
class PlayerBase;
class GameObject;
class Coordinate;
/**
 * @brief The iGameEventHandler class is an interface which the Course-side
 * code uses to interact with the GameEventHandler implemented by the students.
 *
 * @note The interface declares only functions required by the Course-side code.
 * The actual implementation can (and should!) contain more stuff.
 * @note In a "real" project, the GameEventHandler should be a singleton
 * and not use an abstract base class to define the interface for it.
 * <b>This design was chosen merely for pedagogical reasons and to
 * give students more freedom in their project design.</b>
 */
class iGameEventHandler
{
public:
    /**
     * @brief Default destructor.
     */
    virtual ~iGameEventHandler() = default;

    /**
     * @brief Modify Player's resource. Can be used to both sum or subtract.
     * @param player Pointer to the Player whose resource is being modified.
     * @param resource Defines the modified resource.
     * @param amount Defines the amount of change.
     * @post Exception guarantee: Basic
     * @return
     * True - Modification was succesful. \n
     * False - Modification failed. \n
     */
    virtual bool modifyResource(std::shared_ptr<PlayerBase> player,
                                BasicResource resource,
                                int amount) = 0;

    /**
     * @brief Modify Player's resources. Can be used to both sum or subtract
     * @param player Pointer to the Player whose resources are being modified.
     * @param resources ResourceMap containing change amounts.
     * @return
     * True - Modification was succesful. \n
     * False - Modification failed. \n
     */
    virtual bool modifyResources(std::shared_ptr<PlayerBase> player,
                                 ResourceMap resources) = 0;


}; // class iGameEventHandler

} // namespace Course


#endif // IGAMEEVENTHANDLER_H
