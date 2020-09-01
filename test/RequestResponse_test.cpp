#include <gtest/gtest.h>

#include "RequestResponse.h"
#include "exceptions.h"

#include <vector>

using namespace catnap;

class ResponseTester : public RequestResponse
{
    public:
        ResponseTester() {}
        ResponseTester(const char *text) : RequestResponse(text) {}
        ResponseTester(const std::string &text) : RequestResponse(text) {}
        ResponseTester(const nlohmann::json &json) : RequestResponse(json) {}
        virtual ~ResponseTester() {}

    public:
        nlohmann::json * get_json() { return this->_json; }
};

TEST(RequestResponse, Constructors)
{
    {   // Using std string
        ResponseTester obj(std::string("Blah 1"));
        ASSERT_EQ(obj.text(), "Blah 1");
        ASSERT_EQ(obj.get_json(), nullptr);
    }
    {   // Using C-str
        ResponseTester obj("Blah cstr");
        ASSERT_EQ(obj.text(), "Blah cstr");
        ASSERT_EQ(obj.get_json(), nullptr);
    }
    {   // Using json object
        const nlohmann::json expect{ {"key", "value"} };
        ResponseTester obj(nlohmann::json{ {"key", "value"} });
        ASSERT_EQ(obj.text(), "{\"key\":\"value\"}");
        ASSERT_EQ(*obj.get_json(), expect);
    }
}

TEST(RequestResponse, CopyAndMove)
{
    ResponseTester obj1("{\"foo\":\"bar\"}"_json);
    ResponseTester obj2;
    // Copy constructor
    ASSERT_EQ(obj1.text(), ResponseTester(obj1).text());
    // Move assignment
    obj2 = std::move(obj1);
    ASSERT_EQ(obj1.text(), "{\"foo\":\"bar\"}");
    ASSERT_NE(obj1.get_json(), obj2.get_json());
}

TEST(RequestResponse, CheckResponseStatusCode)
{
    ResponseTester obj1("{\"foo\":\"bar\"}"_json);
    ResponseTester obj2;
    // Copy constructor
    ASSERT_EQ(obj1.text(), ResponseTester(obj1).text());
    ASSERT_EQ(obj1.text(), ResponseTester(obj1).text());
}

