#ifndef ILLEGALACTION_H
#define ILLEGALACTION_H
#include "baseexception.h"

namespace Course {

/**
 * @brief The IllegalAction exception is usually used in cases, where
 * an illegal game action was attempted.
 */
class IllegalAction : public BaseException
{
public:
    /**
     * @copydoc BaseException::BaseException
     */
    explicit IllegalAction(const std::string& msg = ""):
        BaseException(msg){}

    /**
     * @copydoc BaseException::~BaseException
     */
    virtual ~IllegalAction() = default;
};

}
#endif // ILLEGALACTION_H
