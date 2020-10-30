#ifndef BGBASEEXCEPTION_H
#define BGBASEEXCEPTION_H

#include <exception>
#include <string>

namespace Course {
/**
 * @brief The Exception class is a base-class for custom exceptions in project.
 */
class BaseException : public std::exception
{
public:
    /**
     * @brief Exception Constructor
     * @param msg std::string describing the reason for exception.
     */
    explicit BaseException(const std::string& msg = ""): m_msg(msg){}

    /**
      * @brief ~Exception Default destructor
      */
    virtual ~BaseException() = default;

    /**
     * @brief msg Returns the message being stored in Exception.
     * @return Stored message or empty string.
     */
    virtual std::string msg() const {return m_msg;}

private:
    const std::string m_msg;
};

}

#endif // BGBASEEXCEPTION_H
