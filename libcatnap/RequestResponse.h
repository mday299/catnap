#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include "HttpMethods.h"

#include <nlohmann/json.hpp>

namespace catnap {

class RequestResponse {
    public:
        RequestResponse();
        RequestResponse(const char *text);
        RequestResponse(const std::string &text);
        RequestResponse(const nlohmann::json &json);
        virtual ~RequestResponse();

        RequestResponse(const RequestResponse &rhs);
        virtual const RequestResponse & operator=(const RequestResponse &rhs);

        RequestResponse(RequestResponse &&rhs);
        virtual const RequestResponse & operator=(RequestResponse &&rhs);

    public:
        std::string text() const;
        nlohmann::json json() const;
        bool data_is_json() const;

    protected:
        std::string _text;
        nlohmann::json *_json = nullptr;
};

class Request : public RequestResponse {
    public:
        Request(const std::string &text);
        Request(const nlohmann::json &json);
        virtual ~Request();

    public:
        HttpMethods get_method() const;

    protected:
        HttpMethods _method;
};

class Response : public RequestResponse {
    public:
        Response(const char *text, int status_code = 200);
        Response(const std::string &text, int status_code = 200);
        Response(const nlohmann::json &json, int status_code = 200);
        virtual ~Response();

    public:
        int get_status_code() const;

    private:
        int _status_code;
};

} // namespace catnap

#endif /* __RESPONSE_H__ */
