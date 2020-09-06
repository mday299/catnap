#ifndef __LIBCATNAP_ROUTE_H__
#define __LIBCATNAP_ROUTE_H__

#include <string>
#include <string_view>
#include <map>

template<bool isRequest>
class Headers;

template<>
class Headers<true>
{
    public:
        /**
         * Getter and Setter for the HTTP version.
         */
        unsigned version() const
        {
            return this->_version;
        }
        void version(const unsigned value)
        {
            this->_version = value;
        }

        /**
         * Getter and Setter for the HTTP request route.
         */
        std::string method() const
        {
            return this->_method;
        }
        void method(const std::string value)
        {
            this->_method = value;
        }

        /**
         * Getter and Setter for the HTTP request route.
         */
        std::string route() const
        {
            return this->_route;
        }
        void route(const std::string value)
        {
            this->_route = value;
        }

        /**
         * Add a header.
         *
         * @param [in] header
         * @param [in] value
         */
        void add_header(const std::string header, const std::string value)
        {
            std::string newheader = this->str_to_lower(header);
            this->_headers[newheader] = value;
        }
        /**
         * Get a header.
         *
         * @param [in] header
         * @param [out] value
         *
         * @returns
         *      (true) If the header was found and value was set.
         *      (false) if the header was not found.
         */
        bool get_header(const std::string header, std::string &value)
        {
            std::string newheader = this->str_to_lower(header);
            for (auto it : this->_headers) {
                if (it.first == newheader) {
                    value = it.second;
                    return true;
                }
            }
            return false;
        }

        /**
         * Add a param.
         *
         * @param [in] param
         * @param [in] value
         */
        void add_param(const std::string param, const std::string value)
        {
            std::string newparam = this->str_to_lower(param);
            this->_params[newparam] = value;
        }
        /**
         * Get a param.
         *
         * @param [in] param
         * @param [out] value
         *
         * @returns
         *      (true) If the param was found and value was set.
         *      (false) if the param was not found.
         */
        bool get_param(const std::string param, std::string &value)
        {
            std::string newparam = this->str_to_lower(param);
            for (auto it : this->_params) {
                if (it.first == newparam) {
                    value = it.second;
                    return true;
                }
            }
            return false;
        }

    private:
        std::string str_to_lower (std::string s)
        {
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) -> unsigned char {
                    return std::tolower(c);
                });
            return s;
        };

    private:
        std::map<std::string, std::string> _headers;
        unsigned _version;
        std::string _method;
        std::string _route;
        std::map<std::string, std::string> _params;
};

template<>
class Headers<false>
{
    public:
        /**
         * Getter and Setter for the HTTP version.
         */
        unsigned version() const
        {
            return this->_version;
        }
        void version(const unsigned value)
        {
            this->_version = value;
        }

        /**
         * Getter and Setter for the HTTP status code.
         */
        unsigned status_code() const
        {
            return this->_status_code;
        }
        void status_code(const unsigned value)
        {
            this->_status_code = value;
        }

        /**
         * Add a header.
         *
         * @param [in] header
         * @param [in] value
         */
        void add_header(const std::string header, const std::string value)
        {
            std::string newheader = this->str_to_lower(header);
            this->_headers[newheader] = value;
        }
        /**
         * Get a header.
         *
         * @param [in] header
         * @param [out] value
         *
         * @returns
         *      (true) If the header was found and value was set.
         *      (false) if the header was not found.
         */
        bool get_header(const std::string header, std::string &value)
        {
            std::string newheader = this->str_to_lower(header);
            for (auto it : this->_headers) {
                if (it.first == newheader) {
                    value = it.second;
                    return true;
                }
            }
            return false;
        }

    private:
        std::string str_to_lower (std::string s)
        {
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) -> unsigned char {
                    return std::tolower(c);
                });
            return s;
        };

    private:
        std::map<std::string, std::string> _headers;
        unsigned _version;
        int _status_code;
};

template <bool isRequest>
class BaseMessage {
    public:
        enum class BodyType {
            STRING, FILE_
        };

    public:

        Headers<isRequest> & headers()
        {
            return this->_headers;
        }
        void headers(const Headers<isRequest> &headers)
        {
            this->_headers = headers;;
        }

        /**
         * Set or get the body of the message
         */
        std::string body() const
        {
            return this->_body;
        }
        void body(const std::string &body)
        {
            this->_body = body;
        }

    private:
        Headers<isRequest> _headers;
        std::string _body;
};

using Request = BaseMessage<true>;
using Response = BaseMessage<false>;

class Route {
    public:
        typedef Response (*route_cb_t) (const Request &);

    public:
        explicit Route(std::string route, route_cb_t cb, std::string method = "GET");
        virtual ~Route();

    public:
        std::string method() const;
        std::string route() const;
        route_cb_t callback() const;

    protected:
        std::string _route;
        route_cb_t _callback;
        std::string _method;
};

#include <boost/beast.hpp>

class PrivateRoute {
    public:
        explicit PrivateRoute(Route route);
        virtual ~PrivateRoute();

    public:
        auto call_callback(boost::beast::http::request<
                boost::beast::http::string_body> &request);

        bool operator==(const Route &rhs) const;

    protected:
        Route _route;
};

template <typename T>
static inline Request from_boost_request(
        const boost::beast::http::request<T> &request)
{
    auto verb_to_str = [](const boost::beast::http::verb &verb)
        -> std::string { return std::string(to_string(verb)); };
    boost::beast::http::request<boost::beast::http::string_body> test_req;
    Request req;
    req.headers().version(request.version());
    req.headers().method(request.method_string().to_string());
    req.headers().route(request.target().to_string());
    return req;
}

static inline auto to_boost_response(Response &response)
{
    boost::beast::http::response<boost::beast::http::string_body> resp;
    resp.result(response.headers().status_code());
    resp.version(response.headers().version());
    resp.body() = response.body();
    return resp;
}

#endif /* __LIBCATNAP_ROUTE_H__ */
