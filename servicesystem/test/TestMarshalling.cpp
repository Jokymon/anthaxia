/***********************************************************************
 *
 * This file is part of Anthaxia.
 * 
 * Anthaxia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Anthaxia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Anthaxia.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "../marshalling.h"

#include "gtest/gtest.h"

namespace {

class TestMarshalling : public ::testing::Test
{
protected:
    TestMarshalling()
    { }

    virtual void SetUp()
    { }

    virtual void TearDown()
    { }
};

TEST_F(TestMarshalling, IntMarshalling)
{
    MarshaledData* data = create_method_call();

    append_int(data, 1);
    append_int(data, 23);
    append_int(data, 454);
    append_int(data, 32);
    append_int(data, 1);

    int result;
    MarshallParser* parser = create_parser(data);

    ASSERT_TRUE(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    ASSERT_TRUE(result==1);

    ASSERT_TRUE(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    ASSERT_TRUE(result==23);

    ASSERT_TRUE(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    ASSERT_TRUE(result==454);

    ASSERT_TRUE(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    ASSERT_TRUE(result==32);

    ASSERT_TRUE(get_next_type(parser)==TYPE_INT);
    parse_int(parser, &result);
    ASSERT_TRUE(result==1);

    ASSERT_TRUE(get_next_type(parser)==TYPE_NONE);

    free_parser(parser);
    free_method_call(data);
}

TEST_F(TestMarshalling, DoubleMarshalling)
{
    MarshaledData* data = create_method_call();

    append_double(data, 3.141);
    append_double(data, 999.999);
    append_double(data, 0.0);

    double result;
    MarshallParser* parser = create_parser(data);

    ASSERT_TRUE(get_next_type(parser)==TYPE_DOUBLE);
    parse_double(parser, &result);
    ASSERT_TRUE(result==3.141);

    ASSERT_TRUE(get_next_type(parser)==TYPE_DOUBLE);
    parse_double(parser, &result);
    ASSERT_TRUE(result==999.999);

    ASSERT_TRUE(get_next_type(parser)==TYPE_DOUBLE);
    parse_double(parser, &result);
    ASSERT_TRUE(result==0.0);

    ASSERT_EQ(TYPE_NONE, get_next_type(parser));

    free_parser(parser);
    free_method_call(data);
}

TEST_F(TestMarshalling, StringMarshalling)
{
    MarshaledData* data = create_method_call();

    append_string(data, "hello");
    append_string(data, "world");
    append_string(data, "");

    MarshallParser* parser = create_parser(data);

    char* result;
    ASSERT_TRUE(get_next_type(parser)==TYPE_STRING);
    parse_string(parser, &result);
    ASSERT_TRUE(strncmp("hello", result, 5)==0);
    free(result);

    ASSERT_TRUE(get_next_type(parser)==TYPE_STRING);
    parse_string(parser, &result);
    ASSERT_TRUE(strncmp("world", result, 5)==0);
    free(result);

    ASSERT_TRUE(get_next_type(parser)==TYPE_STRING);
    parse_string(parser, &result);
    ASSERT_TRUE(strlen(result)==0);
    free(result);

    ASSERT_TRUE(get_next_type(parser)==TYPE_NONE);

    free_parser(parser);
    free_method_call(data);
}

}
