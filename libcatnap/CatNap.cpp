#include "CatNap.h"

#include "exceptions.h"
#include "HttpHandler.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <vector>
#include <thread>

namespace asio = boost::asio;

namespace catnap {

class CatNap_Private : public std::thread {
    public:
        CatNap_Private(int num_threads = 1)
            : _ioc{num_threads}
        {
        }
        ~CatNap_Private() {}

    public:
        void add_socket(void)
        {
        }

        boost::asio::io_context & ioc()
        {
            return this->_ioc;
        }

    private:
        boost::asio::io_context _ioc;
};

struct ListenThreads {
    std::thread thread;
    bool stop = false;
};
std::vector<ListenThreads> g_lthreads;

static asio::io_context ioc;

static void listen_thread(CatNap *cn_obj, CatNap::Listen *listen_def, bool *stop)
{
}

/*----------------------------------------------------------------------------*/
/*
 * https://www.boost.org/doc/libs/1_69_0/libs/beast/example/http/server/async/http_server_async.cpp
 */
/*----------------------------------------------------------------------------*/

CatNap::CatNap()
{
    this->_private = new CatNap_Private();
}

CatNap::~CatNap()
{
    delete (CatNap_Private *)this->_private;
}

/*----------------------------------------------------------------------------*/

/**
 * Add an accepted route to listen for.
 *
 * @param [in] route    Endpoint route.
 * @param [in] cb       Callback function to trigger.
 * @param [in] method   HTTP method type.
 */
void CatNap::add_route(std::string route,
                       endpoint_cb cb,
                       std::string method)
{
    // TODO
}

/**
 * Create a TCP socket to listen on.
 *
 * @param [in] host     The interface to listen on.
 * @param [in] port     The port number to use.
 */
void CatNap::add_tcp_listen(std::string host, int port)
{
    auto addr = boost::asio::ip::make_address(host);
    bool success = setup_tcp_acceptor(
        ((CatNap_Private *)this->_private)->ioc(),
        std::make_shared<std::string>("."),
        addr,
        port);
    if (!success) {
        // TODO replace with try catch?
        throw std::runtime_error("Failed to setup unix socket");
    }
}

/**
 * Create a UNIX socket to listen on.
 *
 * @param [in] path         The path/name of the unix socket.
 * @param [in] anonymous    Make the socket name anonymous (_ie no sock file_).
 */
void CatNap::add_unix_listen(std::string path, bool anonymous)
{
    bool success = setup_unix_acceptor(
        ((CatNap_Private *)this->_private)->ioc(),
        std::make_shared<std::string>("."),
        path,
        anonymous);
    if (!success) {
        // TODO replace with try catch?
        throw std::runtime_error("Failed to setup unix socket");
    }
}

/**
 * Start the server.
 *
 * @param [in] mode     Run the server in blocking mode or threaded mode.
 */
void CatNap::run(CatNap::Mode mode)
{
    //TODO
    this->add_tcp_listen("127.0.0.1", 5000);
}

/**
 * Return the status code of the exited process, could be used as output of the
 * application.
 *
 * @returns Integer status code similar to the value output by programs.
 */
int CatNap::get_status_code()
{
    // TODO
}

/*----------------------------------------------------------------------------*/


} // namespace catnap
