# CMakeList.txt : CMake project for FootageBot, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 23)

# Add source to this project's executable.
set (FootageBotSources "FootageBot.cpp"  "includes_pch.h" "includes_pch.cpp")
#list(APPEND FootageBotSources "")

add_executable (FootageBot ${FootageBotSources})

# External libs handling
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(FootageBot PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(SqliteOrm CONFIG REQUIRED)
target_link_libraries(FootageBot PRIVATE sqlite_orm::sqlite_orm)

set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL REQUIRED)

target_include_directories(FootageBot PRIVATE ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

# For linux users it's need to be changed, i'll do it later
set(TgBotLibPath "${_VCPKG_INSTALLED_DIR}/x64-windows/lib/TgBot.lib")
target_link_libraries(FootageBot PRIVATE ${TgBotLibPath} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

# Precompiled headers
target_precompile_headers(FootageBot PRIVATE "includes_pch.h")