// Tests will be included sometime soon	

#include <gtest/gtest.h>

TEST(SomeParsingTest, ParsingString) {
	using namespace std::string_literals;
	EXPECT_EQ(std::string("str"), "str"s);
}