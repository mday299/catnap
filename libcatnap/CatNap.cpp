#include "CatNap.h"

#include "exceptions.h"
#include "HttpHandler.h"

#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <vector>
#include <thread>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;

namespace catnap {

class CatNap_Private : public std::thread {
    public:
        CatNap_Private() {}
        ~CatNap_Private() {}

    public:
        void add_socket(void)
        {
        }

    private:
        boost::asio::io_context _ioc;
};

struct ListenThreads {
    std::thread thread;
    bool stop = false;
};
std::vector<ListenThreads> g_lthreads;

template <typename SockType>
void handle_socket(SockType sock, CatNap *cn)
{
    static_assert(
            asio::is_class<asio::ip::tcp>::value
            || asio::is_class<asio::local::stream_protocol>::value,
            "Expecting local stream or IP TCP type");

    http::request<http::string_body> req;
    beast::flat_buffer buffer;
    http::read(sock, buffer, req);
}

static asio::io_context ioc;

static void listen_thread(CatNap *cn_obj, CatNap::Listen *listen_def, bool *stop)
{
    if(listen_def->isUnix) {
        asio::local::stream_protocol::endpoint ep(listen_def->addr);
        asio::local::stream_protocol::acceptor acc(ioc, ep);
        while (true) {
            asio::local::stream_protocol::socket sock(ioc);
            acc.accept(sock);

            handle_socket(std::move(sock), cn_obj);
        }
    }
    else {
        asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), listen_def->port);
        asio::ip::tcp::acceptor acc{ioc, ep};
        while (true) {
            asio::ip::tcp::socket sock{ioc};
            acc.accept(sock);

            handle_socket(std::move(sock), cn_obj);
        }
    }
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
    // TODO
}

/**
 * Create a UNIX socket to listen on.
 *
 * @param [in] path         The path/name of the unix socket.
 * @param [in] anonymous    Make the socket name anonymous (_ie no sock file_).
 */
void CatNap::add_unix_listen(std::string path, bool anonymous)
{
    // TODO
    boost::asio::ip::tcp::socket sock();
}

/**
 * Start the server.
 *
 * @param [in] mode     Run the server in blocking mode or threaded mode.
 */
void CatNap::run(CatNap::Mode mode)
{
    //TODO
    boost::asio::io_context ioc;
    std::shared_ptr<std::string> docs_root = std::make_shared<std::string>(".");
    setup_tcp_acceptor(
            ioc,
            docs_root,
            boost::asio::ip::address().from_string("127.0.0.1"),
            5000
            );
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

void test(void)
{
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket sock{ioc};
    boost::asio::ip::tcp::resolver resv{ioc};
}

} // namespace catnap
