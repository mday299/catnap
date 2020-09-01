#include "RequestResponse.h"

#include "exceptions.h"

namespace catnap {

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Base Class                                                                 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

RequestResponse::RequestResponse()
{
    this->_text = std::string("OK");
}

RequestResponse::RequestResponse(const char *text)
{
    this->_text = std::string(text);
}

RequestResponse::RequestResponse(const std::string &text)
{
    this->_text = text;
}

RequestResponse::RequestResponse(const nlohmann::json &json)
{
    this->_json = new nlohmann::json(json);
    this->_text = this->_json->dump();
}

RequestResponse::~RequestResponse()
{
    if (this->_json != nullptr) {
        delete this->_json;
    }
}

/*
 * Copy
 */

RequestResponse::RequestResponse(const RequestResponse &rhs)
{
    this->_text = rhs._text;
    if (rhs._json != nullptr) {
        this->_json = new nlohmann::json(*rhs._json);
    }
}

const RequestResponse & RequestResponse::operator=(const RequestResponse &rhs)
{
    this->_text = rhs._text;
    if (rhs._json != nullptr) {
        this->_json = new nlohmann::json(*rhs._json);
    }
    return *this;
}

/*
 * Move
 */

RequestResponse::RequestResponse(RequestResponse &&rhs)
{
    this->_text = rhs._text;
    this->_json = rhs._json;

    rhs._text = "";
    rhs._json = nullptr;
}

const RequestResponse & RequestResponse::operator=(RequestResponse &&rhs)
{
    this->_text = rhs._text;
    this->_json = rhs._json;

    rhs._text = "";
    rhs._json = nullptr;

    return *this;
}

/*----------------------------------------------------------------------------*/

bool RequestResponse::data_is_json() const
{
    return this->_json != nullptr;
}

std::string RequestResponse::text() const
{
    return this->_text;
}

nlohmann::json RequestResponse::json() const
{
    if (this->_json == nullptr) {
        throw DataIsNotJson();
    }
    return *this->_json;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Request Class                                                              */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

Request::Request(const std::string &text)
    : RequestResponse(text)
{
}

Request::Request(const nlohmann::json &json)
    : RequestResponse(json)
{
}

Request::~Request()
{
}

/*----------------------------------------------------------------------------*/

HttpMethods Request::get_method() const
{
    return this->_method;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Response Class                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

Response::Response(const char *text, int status_code)
    : RequestResponse(text),_status_code(status_code)
{
}

Response::Response(const std::string &text, int status_code)
    : RequestResponse(text),_status_code(status_code)
{
}

Response::Response(const nlohmann::json &json, int status_code)
    : RequestResponse(json),_status_code(status_code)
{
}

Response::~Response()
{
}

int Response::get_status_code() const
{
    return this->_status_code;
}

} // namespace catnap
