#ifndef OWNERCONFLICT_H
#define OWNERCONFLICT_H

#include "illegalaction.h"


namespace Course {

/**
 * @brief The OwnerConflict class is an Exception-class for
 * errors where an operation is conflicting with a GameObject's ownership.
 */
class OwnerConflict : public IllegalAction
{
public:
    /**
     * @copydoc BaseException::BaseException
     */
    explicit OwnerConflict(const std::string& msg = ""):
        IllegalAction(msg) {}

    /**
     * @copydoc BaseException::~BaseException
     */
    virtual ~OwnerConflict() = default;

}; // class OwnerConflict

} // namespace Course


#endif // OWNERCONFLICT_H
