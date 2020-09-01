#include "Header.h"

#include "exceptions.h"

#include <regex>
#include <sstream>
#include <istream>

#include <iostream>

#ifdef _DEBUG
#define DBG(...)                                            \
    do {                                                    \
        std::cout << "DEBUG: " << __VA_ARGS__ << std::endl; \
    } while(0)
#else
#define DBG(x)
#endif

// General header pattern
static std::regex g_empty_line(
    R"~(^$)~",
    std::regex::extended);

// General header pattern
static std::regex g_header_pat(
    R"~(^([a-zA-Z0-9_-]+): (.*)$)~",
    std::regex::extended);

// GET /endpoint?param=value&other=value HTTP/1.1
// Host: blah.com
// User-Agent: curl/7.72.0
// Accept: */*
static std::regex g_request_pat(
    R"~(^(GET|POST|PUT|DELETE)[[:space:]]([^[:space:]]+)[[:space:]]HTTP/1\.1)~",
    std::regex::extended);

// GET /endpoint?param=value&other=value HTTP/1.1
// Host: blah.com
// User-Agent: curl/7.72.0
// Accept: */*
static std::regex g_response_pat(
    "",
    std::regex::extended);

#if 0
// HTTP/1.1 200 OK
// server: GitHub.com
// date: Mon, 31 Aug 2020 21:31:32 GMT
// content-type: application/json; charset=utf-8
// status: 200 OK
// cache-control: no-cache
// vary: Accept, Accept-Encoding, Accept, X-Requested-With
// x-github-media-type: github.v3; format=json
// link: <https://api.github.com/search/repositories?q=vim&page=2>; rel="next", <https://api.github.com/search/repositories?q=vim&page=34>; rel="last"
// access-control-expose-headers: ETag, Link, Location, Retry-After, X-GitHub-OTP, X-RateLimit-Limit, X-RateLimit-Remaining, X-RateLimit-Reset, X-OAuth-Scopes, X-Accepted-OAuth-Scopes, X-Poll-Interval, X-GitHub-Media-Type, Deprecation, Sunset
// access-control-allow-origin: *
// strict-transport-security: max-age=31536000; includeSubdomains; preload
// x-frame-options: deny
// x-content-type-options: nosniff
// x-xss-protection: 1; mode=block
// referrer-policy: origin-when-cross-origin, strict-origin-when-cross-origin
// content-security-policy: default-src 'none'
// X-Ratelimit-Limit: 10
// X-Ratelimit-Remaining: 8
// X-Ratelimit-Reset: 1598909526
// Accept-Ranges: bytes
// Transfer-Encoding: chunked
// X-GitHub-Request-Id: 8E28:6B60:39623E1:6156D1D:5F4D6C33
#endif

namespace catnap {

using headermap_t = std::map<std::string, std::string>;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/**
 * Check if the headers object contains the key.
 * @returns (true) if the key already exists in the headers object;
 *          (false) if not.
 */
bool BaseHeaders::has_header(std::string key)
{
    headermap_t::const_iterator it = this->_headers.find(key);
    return it != this->_headers.end();
}

/**
 * Return the headers map.
 * @returns Map defining all of headers.
 */
headermap_t BaseHeaders::get_headers() const
{
    return this->_headers;
}

/**
 * Add a header to the headers object.
 * @param [in] key      Header key to add.
 * @param [in] value    The value associated with the header.
 */
void BaseHeaders::add_header(std::string key, std::string value)
{
    const auto [what, success] = this->_headers.insert({key, value});
    (void) what;
    if (success == false) {
        throw CatnapError("Failed to add header: " + key);
    }
}

/**
 * Add headers to the headers object by providing a map.
 * @param [in] headers  Map of headers to add.
 */
void BaseHeaders::add_headers(headermap_t headers)
{
    this->_headers.insert(headers.begin(), headers.end());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void RequestHeaders::parse(std::string &data)
{
    int newline_count = 0;
    std::stringstream stream(data);
    std::string line;
    std::smatch smatch;
    while(std::getline(stream, line)) {
        DBG("Line: " << line);
        std::regex_match(line, g_request_pat);
        if (std::regex_match(line, g_empty_line)) {
            DBG("Match: empty line");
            newline_count += 1;
            if (newline_count == 2) {
                break;
            }
            continue;
        }
        if (std::regex_match(line, smatch, g_request_pat)) {
            DBG("Match: Request: " << smatch[0]);
            this->_method = from_string(smatch[1]); // TODO get method
            DBG("Match: Request: Method: " << smatch[1]);
            std::stringstream fullroute(smatch[2]);
            DBG("Match: Request: Full Route: " << smatch[2]);
            std::getline(fullroute, this->_route, '?');
            DBG("Match: Request: Route: " << this->_route);
            std::string part;
            while(std::getline(fullroute, part, '&')) {
                std::stringstream substream(part);
                std::string key;
                std::string value;
                std::getline(substream, key, '=');
                std::getline(substream, value, '=');
                DBG("Match: Request: Param: " << key << ", " << value);
                this->_params.insert({key, value});
            }
            continue;
        }
        if (std::regex_match(line, smatch, g_header_pat)) {
            DBG("Match: Header: " << smatch[0]);
            this->_headers.insert({smatch[1], smatch[2]});
            DBG("Match: Header: " << smatch[1] << ", " << smatch[2]);
            continue;
        }
        // What do we do here??
        throw CatnapError("Entered unknown space of headers parse");
    }
}

HttpMethods RequestHeaders::get_method() const
{
    return this->_method;
}

std::string RequestHeaders::get_route() const
{
    return this->_route;
}

std::map<std::string, std::string> RequestHeaders::get_params() const
{
    return this->_params;
}

/*----------------------------------------------------------------------------*/

} // namespace catnap


