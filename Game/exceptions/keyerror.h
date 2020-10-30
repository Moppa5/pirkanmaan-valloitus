#ifndef BGBASEKEYERROR_H
#define BGBASEKEYERROR_H

#include "baseexception.h"

namespace Course {

/**
 * @brief The KeyError class is an Exception-class for cases where the
 * used key is invalid.
 */
class KeyError: public BaseException
{
public:
    /**
     * @copydoc BaseException::BaseException
     */
    explicit KeyError(const std::string& msg = ""):
        BaseException(msg) {}

    /**
     * @copydoc BaseException::~BaseException
     */
    virtual ~KeyError() = default;
};

}

#endif // BGBASEKEYERROR_H
