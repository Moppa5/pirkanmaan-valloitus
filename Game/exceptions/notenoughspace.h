#ifndef NOTENOUGHSPACE_H
#define NOTENOUGHSPACE_H

#include "illegalaction.h"


namespace Course {

/**
 * @brief The NotEnoughSpace class is an Exception-class for
 * errors where GameObjects are being placed onto Tiles with no space available.
 */
class NotEnoughSpace : public IllegalAction
{
public:
    /**
     * @copydoc BaseException::BaseException
     */
    explicit NotEnoughSpace(const std::string& msg = ""):
        IllegalAction(msg) {}

    /**
     * @copydoc BaseException::~BaseException
     */
    virtual ~NotEnoughSpace() = default;

}; // class NotEnoughSpace

} // namespace Course


#endif // NOTENOUGHSPACE_H
