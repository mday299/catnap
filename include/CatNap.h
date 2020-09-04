#ifndef __CATNAP_H__
#define __CATNAP_H__

#include <nlohmann/json.hpp>

#include <string>
#include <map>
#include <thread>

namespace catnap {

class Route;

typedef void * (*endpoint_cb) (void *);
typedef void (*logger_cb) (const char *msg, ...);


class CatNap {
    public:
        enum class Mode { Blocking, Threaded };
        class Listen {
            public:
                std::string addr;       // Address/path to listen on
                int port;               // Port to use for TCP connections
                bool isUnix = false;    // If true, addr is interpreted as path
                bool isAnon = false;    // If unix, is it anonymous?
        };

    public:
        CatNap();
        virtual ~CatNap();

    public:
        void add_route(std::string route,
                       endpoint_cb cb,
                       std::string method = "GET");

        void run(Mode mode = Mode::Blocking);
        int get_status_code();

        void add_tcp_listen(std::string host, int port);
        void add_unix_listen(std::string path, bool anonymous = false);

    private:
        void *_private = nullptr;
        std::map<std::string, Route> routes;
        int status_code = 0;
        std::thread _listen_thr;
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
