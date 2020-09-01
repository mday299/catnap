#ifndef __HEADER_H__
#define __HEADER_H__

#include "HttpMethods.h"

#include <nlohmann/json.hpp>

#include <map>
#include <string>

namespace catnap {

class BaseHeaders
{
    public:
        std::map<std::string, std::string> get_headers() const;

        void add_header(std::string key, std::string value);
        void add_headers(std::map<std::string, std::string> headers);

    private:
        bool has_header(std::string key);

    protected:
        std::map<std::string, std::string> _headers;
};

class RequestHeaders : public BaseHeaders
{
    public:
        void parse(std::string &);

        HttpMethods get_method() const;
        std::string get_route() const;
        std::map<std::string, std::string> get_params() const;

    protected:
        HttpMethods _method;
        std::string _route;
        std::map<std::string, std::string> _params;
};

class ResponseHeaders : public BaseHeaders
{
};

} // namespace catnap

#endif /* __HEADER_H__*/
