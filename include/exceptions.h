#ifndef __CATNAP_EXCEPTIONS_H__
#define __CATNAP_EXCEPTIONS_H__

#include <stdexcept>
#include <string>
#include <cstring>

namespace catnap {

class CatnapError: public std::exception
{
    public:
        CatnapError()
            : msg("") {}
        CatnapError(std::string msg)
            : msg(msg) {}
        CatnapError(std::string msg, int err_no)
            : msg(msg + std::strerror(err_no)) {}
        CatnapError(const char *msg)
            : msg(msg) {}
        CatnapError(const char *msg, int err_no)
            : msg(std::string(msg) + std::strerror(err_no)) {}
        virtual ~CatnapError() {}

    public:
        virtual const char * what() const noexcept
        { return this->msg.c_str(); }

    private:
        std::string msg;
};

class CatnapStreamError : public CatnapError
{
    public:
        CatnapStreamError()
            : CatnapError("") {}
        CatnapStreamError(std::string msg)
            : CatnapError(msg) {}
        CatnapStreamError(std::string msg, int err_no)
            : CatnapError(msg, err_no) {}
        CatnapStreamError(const char *msg)
            : CatnapError(msg) {}
        CatnapStreamError(const char *msg, int err_no)
            : CatnapError(msg, err_no) {}
        virtual ~CatnapStreamError() {}
};

class DataIsNotJson : public CatnapError
{
    public:
        DataIsNotJson()
            : CatnapError("") {}
        DataIsNotJson(std::string msg)
            : CatnapError(msg) {}
        DataIsNotJson(std::string msg, int err_no)
            : CatnapError(msg, err_no) {}
        DataIsNotJson(const char *msg)
            : CatnapError(msg) {}
        DataIsNotJson(const char *msg, int err_no)
            : CatnapError(msg, err_no) {}
        virtual ~DataIsNotJson() {}
};

class EnumerationError : public CatnapError
{
    public:
        EnumerationError()
            : CatnapError("") {}
        EnumerationError(std::string msg)
            : CatnapError(msg) {}
        EnumerationError(std::string msg, int err_no)
            : CatnapError(msg, err_no) {}
        EnumerationError(const char *msg)
            : CatnapError(msg) {}
        EnumerationError(const char *msg, int err_no)
            : CatnapError(msg, err_no) {}
        virtual ~EnumerationError() {}
};

} // namespace catnap

#endif /*__CATNAP_EXCEPTIONS_H__*/

