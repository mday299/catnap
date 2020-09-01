#include "HttpMethods.h"

#include "exceptions.h"

#include <map>

namespace catnap {

static const std::map<HttpMethods, std::string> g_http_methods {
    {HttpMethods::GET, "GET"},
    {HttpMethods::PUT, "PUT"},
    {HttpMethods::POST, "POST"},
    {HttpMethods::DELETE, "DELETE"},
};

} // namespace catnap

std::string to_string(catnap::HttpMethods method)
{
    std::map<catnap::HttpMethods, std::string>::const_iterator it \
        = catnap::g_http_methods.find(method);
    if (it == catnap::g_http_methods.end()) {
        return "catnap::HttpMethods::Unknown";
    }
    return it->second;
}

catnap::HttpMethods from_string(std::string method)
{
    for (std::pair<catnap::HttpMethods, std::string> item
            : catnap::g_http_methods) {
        if (item.second == method) {
            return item.first;
        }
    }
    throw catnap::EnumerationError("Unable to find " + method + " enum");
}
