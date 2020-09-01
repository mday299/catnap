#include <gtest/gtest.h>

#include "Header.h"
#include "exceptions.h"

using namespace catnap;

static std::string g_test_req_str =
R"~(GET /multi/level/endpoint?param=1&thing=off HTTP/1.1
Host: com.mooo.corbino
User-Agent: curl/7.72.0
Accept: */*

Data starts here.

)~";

static std::string g_test_resp_str =
R"~(HTTP 200 OK
Content-Type: application/json; charset=utf-8

)~";

TEST(Headers, RequestHeaderCreation)
{
    RequestHeaders hdrs;

    EXPECT_NO_THROW(hdrs.parse(g_test_req_str));
    EXPECT_EQ(to_string(hdrs.get_method()), to_string(HttpMethods::GET));
    EXPECT_EQ(hdrs.get_route(), std::string("/multi/level/endpoint"));
    EXPECT_EQ(hdrs.get_params()["param"], std::string("1"));
    EXPECT_EQ(hdrs.get_params()["thing"], std::string("off"));
    EXPECT_EQ(hdrs.get_headers().size(), 3);
    EXPECT_EQ(hdrs.get_headers()["Host"], "com.mooo.corbino");
    EXPECT_EQ(hdrs.get_headers()["User-Agent"], "curl/7.72.0");
    EXPECT_EQ(hdrs.get_headers()["Accept"], "*/*");
}

TEST(Headers, UsingAddHeader)
{
    RequestHeaders hdrs;

    EXPECT_NO_THROW(hdrs.add_header("Content-Type", "application/json"));
    EXPECT_EQ(hdrs.get_headers().size(), 1);

    std::map<std::string, std::string> more_hdrs{
        {"x-app-foo", "doo"},
        {"x-app-bar", "dah"},
    };
    EXPECT_NO_THROW(hdrs.add_headers(more_hdrs));
    EXPECT_EQ(hdrs.get_headers().size(), 3);
}
