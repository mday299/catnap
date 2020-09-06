#include "Route.h"

Route::Route(std::string route, route_cb_t cb, std::string method)
        : _route(route), _callback(cb), _method(method)
{}

Route::~Route()
{}

/*----------------------------------------------------------------------------*/

std::string Route::method() const
{
    return this->_method;
}

std::string Route::route() const
{
    return this->_route;
}

Route::route_cb_t Route::callback() const
{
    return this->_callback;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include <boost/beast.hpp>
namespace http = boost::beast::http;

auto call_callback(http::request<http::string_body> &request)
{
    // Create request wrapper to remove boost deps

    try {
        // Call callback
    }
    catch(std::exception &e) {
        http::response<http::string_body> resp {
            http::status::internal_server_error, request.version()};
        resp.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        resp.set(http::field::content_type, "text/html");
        resp.body() = e.what();
        resp.prepare_payload();
        return resp;
    }

    http::response<http::string_body> resp;
    return resp;
}

/*----------------------------------------------------------------------------*/
