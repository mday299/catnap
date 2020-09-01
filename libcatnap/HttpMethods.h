#ifndef __HTTPMETHODS_H__
#define __HTTPMETHODS_H__

#include <string>

namespace catnap {

enum class HttpMethods {
    GET, POST, PUT, DELETE,
};

} // namespace catnap

std::string to_string(catnap::HttpMethods method);
catnap::HttpMethods from_string(std::string method);

#endif /* __HTTPMETHODS_H__ */
