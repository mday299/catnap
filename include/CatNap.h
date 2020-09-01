#ifndef __CATNAP_H__
#define __CATNAP_H__

#include <nlohmann/json.hpp>

#include <string>
#include <map>

namespace catnap {

class Route;

typedef void * (*endpoint_cb) (void *);
typedef void (*logger_cb) (const char *msg, ...);

class CatNap {
    public:
        CatNap(int socket = 0);
        virtual ~CatNap();

    public:
        void add_route(std::string route,
                       endpoint_cb cb,
                       std::string method = "GET");

    private:
        int _socket;
        std::map<std::string, Route> routes;
};

class Route {
    public:
        explicit Route(std::string route, std::string method,
                endpoint_cb callback);

    private:
        std::string route;
        std::string method;
        endpoint_cb callback;
};

} // namespace catnap

#endif /* __CATNAP_H__ */
