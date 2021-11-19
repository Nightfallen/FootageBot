cmake_minimum_required (VERSION 3.20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Setting vars for testing binary file
#set(ParsingTests "SomeParsing.hpp")
#list(APPEND ParsingTests "new_file")

# Unit tests
# ${ParsingTests}
set(TestsSources "main.cpp")
add_executable(FootageBotTests ${TestsSources})
target_link_libraries(FootageBotTests ${GTEST_BOTH_LIBRARIES})
add_test(NAME FootageBotTests COMMAND FootageBotTests) 