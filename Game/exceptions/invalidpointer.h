#ifndef INVALIDPOINTER_H
#define INVALIDPOINTER_H

#include "baseexception.h"

namespace Course {

/**
 * @brief The InvalidPointer exception is usually used in cases, where
 * data can't be accessed through a pointer.
 */
class InvalidPointer : public BaseException
{
public:
    /**
     * @copydoc BaseException::BaseException
     */
    explicit InvalidPointer(const std::string& msg = ""):
        BaseException(msg){}

    /**
     * @copydoc BaseException::~BaseException
     */
    virtual ~InvalidPointer() = default;
};

}
#endif // INVALIDPOINTER_H
