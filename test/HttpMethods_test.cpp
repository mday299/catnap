#include <gtest/gtest.h>

#include "HttpMethods.h"
#include "exceptions.h"

#include <vector>

using namespace catnap;

TEST(HttpMethods, TestToString)
{
    ASSERT_EQ(to_string(HttpMethods::GET), "GET");
    ASSERT_EQ(to_string(HttpMethods::POST), "POST");
    ASSERT_EQ(to_string(HttpMethods::PUT), "PUT");
    ASSERT_EQ(to_string(HttpMethods::DELETE), "DELETE");
}

TEST(HttpMethods, TestFromString)
{
    ASSERT_EQ(from_string("GET"), HttpMethods::GET);
    ASSERT_EQ(from_string("POST"), HttpMethods::POST);
    ASSERT_EQ(from_string("PUT"), HttpMethods::PUT);
    ASSERT_EQ(from_string("DELETE"), HttpMethods::DELETE);
}

TEST(HttpMethods, TestBadString)
{
    ASSERT_THROW(from_string("BLAH"), EnumerationError);
}
