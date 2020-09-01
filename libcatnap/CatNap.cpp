#include "CatNap.h"

#include "exceptions.h"

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <vector>

namespace catnap {

const std::map<HttpMethods, std::string> g_http_methods {
    {HttpMethods::GET, "GET"},
    {HttpMethods::PUT, "PUT"},
    {HttpMethods::POST, "POST"},
    {HttpMethods::DELETE, "DELETE"},
};

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
                       HttpMethods method)
{
}

} // namespace catnap
