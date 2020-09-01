#include "CatNap.h"

#include "exceptions.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <vector>

namespace catnap {

/*----------------------------------------------------------------------------*/

static int _create_default_socket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw CatnapStreamError("Unable to create socket file descriptor",
                                errno);
    }
    return sock;
}

/*----------------------------------------------------------------------------*/

CatNap::CatNap(int socket)
{
    if (socket == 0) {
        this->_socket = _create_default_socket();
    }
    else if (socket > 0) {
        this->_socket = socket;
    }
}

CatNap::~CatNap()
{
    if (this->_socket > 0) {
        close(this->_socket);
    }
}

/*----------------------------------------------------------------------------*/

void CatNap::add_route(std::string route,
                       endpoint_cb cb,
                       std::string method)
{
}

/*----------------------------------------------------------------------------*/

void test(void)
{
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket sock{ioc};
    boost::asio::ip::tcp::resolver resv{ioc};
}

} // namespace catnap
